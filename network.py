import numpy as np
from numpy.typing import NDArray
from json import dump, load, loads, JSONDecodeError
import logging


class NetworkError(Exception):
    pass


class Network:
    def __init__(self, N: int, M: int, L: int, weight_type, input_type, output_type, in_bord: float, out_bord: float, weight_board: float):
        try:
            if not all(isinstance(x, int) for x in [N, M, L]) or not all(x > 0 for x in [N, M, L]):
                raise ValueError("N, M, L must be positive integers")
            if not all(isinstance(x, (int, float)) for x in [in_bord, out_bord, weight_board]):
                raise ValueError("Boundaries must be numeric values")
            
            # N - size of inner layer
            # M - size of input layer (768 bits position representation)
            # L - amount of training data
            # in_bord (alpha) - inner layer clip boundary (±6)
            # out_bord (beta) - output layer clip boundary (±32000)
            self.N = N
            self.M = M
            self.L = L
            self.alpha = in_bord
            self.beta = out_bord
            self.weight_board = weight_board
            
            # Network weights and biases stored in 3 states: current[0], next[1], gradient[2]
            self.A_t = np.zeros((3, M, N), dtype=weight_type)  # Inner layer weights matrix
            self.B_t = np.zeros((3, 1, N), dtype=weight_type)  # Inner layer bias vector
            self.C_1_t = np.zeros((3, 1, N), dtype=weight_type)  # Output layer weights for white
            self.C_2_t = np.zeros((3, 1, N), dtype=weight_type)  # Output layer weights for black
            self.d = np.zeros((3, 1), dtype=weight_type)       # Output layer bias
            
            # Training data matrices
            self.X_1 = np.zeros((L, M), dtype=input_type)      # Position vectors
            self.X_2 = np.zeros((L, 1), dtype=input_type)      # Active colors
            self.Y = np.zeros((L, 1), dtype=output_type)       # Expected values
            
            # Helper vectors of ones
            self.L_1 = np.ones((L, 1), dtype=weight_type)      # L×1 ones vector
            self.N_1 = np.ones((N, 1), dtype=weight_type)      # N×1 ones vector
        except Exception as e:
            logging.error(f"Failed to initialize network: {str(e)}")
            raise NetworkError("Network initialization failed") from e
    
    def read_db(self, in_f: str) -> None:
        try:
            if not isinstance(in_f, str) or not in_f:
                raise ValueError("Invalid input file path")
                
            # Read training data from JSON file
            # Each line contains position FEN, active color and expected value
            with open(in_f, 'r') as f:
                for i, line in enumerate(f):
                    if i >= self.L:
                        break

                    note = loads(line)
                    if not all(k in note for k in ['count', 'color', 'fen']):
                        raise ValueError(f"Invalid JSON format at line {i+1}")
                        
                    self.Y[i] = note['count']  # Expected value
                    self.X_2[i] = 1 if note['color'] == "w" else 0  # Active color (1-white, 0-black)
                    num = note['fen']  # Position as 768-bit number
                    
                    for j in range(self.M):
                        self.X_1[i][self.M-1-j] = num & 1  # Convert to binary vector
                        num >>= 1
        except FileNotFoundError:
            logging.error(f"Database file not found: {in_f}")
            raise
        except JSONDecodeError as e:
            logging.error(f"Invalid JSON in database: {str(e)}")
            raise NetworkError("Failed to parse database") from e
        except Exception as e:
            logging.error(f"Failed to read database: {str(e)}")
            raise NetworkError("Database reading failed") from e
            
    def read_coefs(self, coefs: str) -> None:
        try:
            if not isinstance(coefs, str) or not coefs:
                raise ValueError("Invalid coefficients file path")
                
            # Read network weights from JSON file
            with open(coefs, 'r') as f:
                data = load(f)
                if not all(k in data for k in ['A', 'B', 'C_1', 'C_2', 'd']):
                    raise ValueError("Missing required coefficients in file")
                    
                self.A_t[0] = np.array(data['A'])
                self.B_t[0] = np.array(data['B'])
                self.C_1_t[0] = np.array(data['C_1'])
                self.C_2_t[0] = np.array(data['C_2'])
                self.d[0] = np.array(data['d'])
        except FileNotFoundError:
            logging.error(f"Coefficients file not found: {coefs}")
            raise
        except Exception as e:
            logging.error(f"Failed to read coefficients: {str(e)}")
            raise NetworkError("Failed to load coefficients") from e

    def create_coefs(self) -> None:
        try:
            # Initialize network weights with random values
            self.A_t[0] = np.random.normal(0, np.sqrt(2.0 / (self.M + self.N)), size=(self.M, self.N)).astype(np.float16)
            self.B_t[0] = np.random.normal(0, 0.10**(-3), (1, self.N))
            self.C_1_t[0] = np.random.uniform(-0.1, 0.1, size=(1, self.N)).astype(np.float16)
            self.C_2_t[0] = np.random.uniform(-0.1, 0.1, size=(1, self.N)).astype(np.float16)
            self.d[0] = np.random.normal(0, 0.10**(-3), 1)
        except Exception as e:
            logging.error(f"Failed to create coefficients: {str(e)}")
            raise NetworkError("Coefficient initialization failed") from e

    def norm(self, *arrays) -> float:
        try:
            # Calculate L2 norm of gradients for gradient descent step size
            return sum(np.linalg.norm(array)**2 for array in arrays)**0.5
        except Exception as e:
            logging.error(f"Failed to calculate norm: {str(e)}")
            raise NetworkError("Norm calculation failed") from e
    
    def Z(self) -> NDArray:
        try:
            # Calculate inner layer output Z = clip(AX + B, -α, α)
            return np.clip(np.matmul(self.X_1, self.A_t[0]) + np.matmul(self.L_1, self.B_t[0]), 
                           -self.alpha, self.alpha)
        except Exception as e:
            logging.error(f"Failed to calculate Z: {str(e)}")
            raise NetworkError("Z calculation failed") from e
    
    def T(self) -> NDArray:
        try:
            # Calculate output weights based on active color: T = X₂C₁ᵀ + (1-X₂)C₂ᵀ
            return np.matmul(self.X_2, self.C_1_t[0]) + np.matmul(np.logical_not(self.X_2), self.C_2_t[0])
        except Exception as e:
            logging.error(f"Failed to calculate T: {str(e)}")
            raise NetworkError("T calculation failed") from e

    def F(self) -> NDArray:
        try:
            # Calculate network output F = clip((Z⊙T)1ₙ + d1ₗ, -β, β)
            return np.clip(np.matmul(np.multiply(self.Z(), self.T()), self.N_1)
                          + (self.d[0] * self.L_1), 
                            -self.beta, self.beta)
        except Exception as e:
            logging.error(f"Failed to calculate F: {str(e)}")
            raise NetworkError("F calculation failed") from e

    def E(self) -> NDArray:
        try:
            # Calculate error E = (Y-F)⊙M_F where M_F is output clip mask
            return np.matmul(
                np.multiply(self.Y - self.F(), ((self.F() >= -self.beta) & (self.F() <= self.beta))),
                self.N_1.T)
        except Exception as e:
            logging.error(f"Failed to calculate E: {str(e)}")
            raise NetworkError("E calculation failed") from e

    def grad(self) -> tuple[NDArray]:
        try:
            # Calculate gradients for all network parameters
            # dA = X₁ᵀ(E⊙T⊙M_Z)
            self.A_t[2] = np.matmul(self.X_1.T, 
                           (self.E() * self.T() * ((self.Z() >= -self.alpha) &(self.Z() <= self.alpha))))
            
            # dB = 1ₗᵀ(E⊙T⊙M_Z)
            self.B_t[2] = np.matmul(self.L_1.T, 
                           (self.E() * self.T() * ((self.Z() >= -self.alpha) &(self.Z() <= self.alpha))))

            # dC₁ = X₂ᵀ(E⊙Z)
            self.C_1_t[2] = np.matmul(self.X_2.T, 
                             (self.E() * self.Z()))
            
            # dC₂ = (1-X₂)ᵀ(E⊙Z)
            self.C_2_t[2] = np.matmul(np.logical_not(self.X_2).T, 
                             (self.E() * self.Z()))

            # dd = 1ₗᵀ(E⊙M_F)
            self.d[2] = np.matmul(self.L_1.T, 
                           np.multiply(self.Y - self.F(), ((self.F() >= -self.beta) &(self.F() <= self.beta))))
        except Exception as e:
            logging.error(f"Failed to calculate gradients: {str(e)}")
            raise NetworkError("Gradient calculation failed") from e
    
    def update(self, step: float) -> None:
        try:
            # if not isinstance(step, (int, float)) or step <= 0:
            #     raise ValueError("Step size must be a positive number")
                
            # Update weights using gradient descent: θₜ₊₁ = θₜ - η∇E(θₜ)
            self.A_t[1] = self.A_t[0] - step * self.A_t[2]
            self.B_t[1] = self.B_t[0] - step * self.B_t[2]
            self.C_1_t[1] = self.C_1_t[0] - step * self.C_1_t[2]
            self.C_2_t[1] = self.C_2_t[0] - step * self.C_2_t[2]
            self.d[1] = self.d[0] - step * self.d[2]
        except Exception as e:
            logging.error(f"Failed to update weights: {str(e)}")
            raise NetworkError("Weight update failed") from e

    def upgrade(self) -> None:
        try:
            # Clip updated weights to prevent overflow
            self.A_t[0] = np.clip(self.A_t[1], -self.weight_board, self.weight_board)
            self.B_t[0] = np.clip(self.B_t[1], -self.weight_board, self.weight_board)
            self.C_1_t[0] = np.clip(self.C_1_t[1], -self.weight_board, self.weight_board)
            self.C_2_t[0] = np.clip(self.C_2_t[1], -self.weight_board, self.weight_board)
            self.d[0] = np.clip(self.d[1], -self.weight_board, self.weight_board)
        except Exception as e:
            logging.error(f"Failed to upgrade weights: {str(e)}")
            raise NetworkError("Weight upgrade failed") from e

    def result(self) -> None:
        try:
            # Save trained network weights to JSON file
            with open('./result.json', 'w') as f:
                dump({"A": self.A_t[0].tolist(),
                       "B": self.B_t[0].tolist(),
                       "C_1": self.C_1_t[0].tolist(),
                       "C_2": self.C_2_t[0].tolist(),
                       "d": self.d[0].tolist()}, f, indent=4)
        except Exception as e:
            logging.error(f"Failed to save results: {str(e)}")
            raise NetworkError("Failed to save network weights") from e
