import cupy as cp
from numpy.typing import NDArray
from json import dump, load, loads, JSONDecodeError
import logging
from .base import BaseNetwork, BaseAdam, NetworkError


class Adam(BaseAdam):
    """Оптимизатор Adam для GPU."""
    def __init__(self, count: int, alpha: float = 0.001, beta1: float = 0.9, 
                  beta2: float = 0.999, eps: float = 1e-8):
        super().__init__(count, alpha, beta1, beta2, eps)


class Network(BaseNetwork):
    """Нейронная сеть для GPU реализации."""
    def __init__(self, N: int, M: int, L: int, weight_type, input_type, output_type, 
                  in_bord: float, out_bord: float, weight_board: float):
        super().__init__(N, M, L, weight_type, input_type, output_type, 
                        in_bord, out_bord, weight_board)
        
        # Инициализация матриц весов
        self.A_t = cp.zeros((3, M, N), dtype=weight_type)
        self.B_t = cp.zeros((3, 1, N), dtype=weight_type)
        self.C_1_t = cp.zeros((3, 1, N), dtype=weight_type)
        self.C_2_t = cp.zeros((3, 1, N), dtype=weight_type)
        self.d = cp.zeros((3, 1), dtype=weight_type)
        self.coefs = (self.A_t, self.B_t, self.C_1_t, self.C_2_t, self.d)
        
        # Инициализация входных данных и меток
        self.X_1 = cp.zeros((L, M), dtype=input_type)
        self.X_2 = cp.zeros((L, 1), dtype=input_type)
        self.Y = cp.zeros((L, 1), dtype=output_type)
        
        # Вспомогательные векторы
        self.L_1 = cp.ones((L, 1), dtype=weight_type)
        self.N_1 = cp.ones((N, 1), dtype=weight_type)
        
        # Промежуточные значения
        self.z = None
        self.t = None
        self.f = None
        self.e = None

    def read_db(self, in_f: str) -> None:
        try:
            if not isinstance(in_f, str) or not in_f:
                raise ValueError("Invalid input file path")
                
            with open(in_f, 'r') as f:
                for i, line in enumerate(f):
                    if i >= self.L:
                        break

                    note = loads(line)
                    if not all(k in note for k in ['count', 'color', 'fen']):
                        raise ValueError(f"Invalid JSON format at line {i+1}")
                        
                    self.Y[i] = note['count']
                    self.X_2[i] = 1 if note['color'] == "w" else 0
                    
                    try:
                        fen_int = int(note['fen'])
                        num_bytes = fen_int.to_bytes(96, byteorder='big')
                        num_array = cp.array(cp.unpackbits(cp.frombuffer(num_bytes, dtype=cp.uint8), bitorder='big'))
                        self.X_1[i] = num_array[-self.M:]
                    except (ValueError, OverflowError) as e:
                        raise ValueError(f"Invalid FEN value at line {i+1}: {str(e)}")

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
                
            with open(coefs, 'r') as f:
                data = load(f)
                if not all(k in data for k in ['A', 'B', 'C_1', 'C_2', 'd']):
                    raise ValueError("Missing required coefficients in file")
                    
                self.A_t[0] = cp.array(data['A'])
                self.B_t[0] = cp.array(data['B'])
                self.C_1_t[0] = cp.array(data['C_1'])
                self.C_2_t[0] = cp.array(data['C_2'])
                self.d[0] = cp.array(data['d'])
        except FileNotFoundError:
            logging.error(f"Coefficients file not found: {coefs}")
            raise
        except Exception as e:
            logging.error(f"Failed to read coefficients: {str(e)}")
            raise NetworkError("Failed to load coefficients") from e

    def create_coefs(self) -> None:
        try:
            self.A_t[0] = cp.random.normal(0, cp.sqrt(2.0 / (self.M + self.N)), size=(self.M, self.N))
            self.B_t[0] = cp.random.normal(0, 0.10**(-3), (1, self.N))
            self.C_1_t[0] = cp.random.uniform(-0.1, 0.1, size=(1, self.N))
            self.C_2_t[0] = cp.random.uniform(-0.1, 0.1, size=(1, self.N))
            self.d[0] = cp.random.normal(0, 0.10**(-3), (1,))
        except Exception as e:
            logging.error(f"Failed to create coefficients: {str(e)}")
            raise NetworkError("Coefficient initialization failed") from e

    def norm(self, *arrays) -> float:
        try:
            return float(cp.sqrt(sum(cp.linalg.norm(array)**2 for array in arrays)))
        except Exception as e:
            logging.error(f"Failed to calculate norm: {str(e)}")
            raise NetworkError("Norm calculation failed") from e

    def Z(self) -> NDArray:
        try:
            return cp.clip(cp.matmul(self.X_1, self.A_t[0]) + cp.matmul(self.L_1, self.B_t[0]), 
                          -self.alpha, self.alpha)
        except Exception as e:
            logging.error(f"Failed to calculate Z: {str(e)}")
            raise NetworkError("Z calculation failed") from e

    def T(self) -> NDArray:
        try:
            return cp.matmul(self.X_2, self.C_1_t[0]) + cp.matmul((1 - self.X_2), self.C_2_t[0])
        except Exception as e:
            logging.error(f"Failed to calculate T: {str(e)}")
            raise NetworkError("T calculation failed") from e

    def F(self) -> NDArray:
        try:
            self.t = self.T()
            self.z = self.Z()
            return cp.clip(cp.matmul(cp.multiply(self.z, self.t), self.N_1)
                        + (self.d[0] * self.L_1), 
                        -self.beta, self.beta)
        except Exception as e:
            logging.error(f"Failed to calculate F: {str(e)}")
            raise NetworkError("F calculation failed") from e

    def E(self) -> None:
        try:
            self.f = self.F()
            self.e = cp.matmul(
                cp.multiply(self.Y - self.f, ((self.f >= -self.beta) & (self.f <= self.beta))),
                self.N_1.T)
        except Exception as e:
            logging.error(f"Failed to calculate E: {str(e)}")
            raise NetworkError("E calculation failed") from e

    def grad(self) -> None:
        try:
            self.E()
            mask_Z = ((self.z >= -self.alpha) & (self.z <= self.alpha))
            mask_F = ((self.f >= -self.beta) & (self.f <= self.beta))

            self.A_t[2] = self.X_1.T @ (self.e * self.t * mask_Z)
            self.B_t[2] = self.L_1.T @ (self.e * self.t * mask_Z)
            self.C_1_t[2] = self.X_2.T @ (self.e * self.z)
            self.C_2_t[2] = (1 - self.X_2).T @ (self.e * self.z)
            self.d[2] = self.L_1.T @ ((self.Y - self.f) * mask_F)

        except Exception as e:
            logging.error(f"Failed to calculate gradients: {str(e)}")
            raise NetworkError("Gradient calculation failed") from e

    def update(self, optim: BaseAdam) -> None:
        try:
            if not isinstance(optim, BaseAdam):
                raise ValueError("Invalid optimizer type")
                
            optim.t += 1
            for i, coef in enumerate(self.coefs):
                if optim.m[i] is None:
                    optim.m[i] = cp.zeros_like(coef[1])
                    optim.v[i] = cp.zeros_like(coef[1])
                
                optim.m[i] = optim.beta1 * optim.m[i] + (1 - optim.beta1) * coef[2]
                optim.v[i] = optim.beta2 * optim.v[i] + (1 - optim.beta2) * cp.multiply(coef[2], coef[2])
                
                m_hat = optim.m[i] / (1 - optim.beta1 ** optim.t)
                v_hat = optim.v[i] / (1 - optim.beta2 ** optim.t)
                
                coef[1] += optim.alpha * m_hat / (cp.sqrt(v_hat) + optim.eps)
                
        except Exception as e:
            logging.error(f"Failed to update weights: {str(e)}")
            raise NetworkError("Weight update failed") from e

    def upgrade(self) -> None:
        try:
            for coef in self.coefs:
                coef[0] = cp.clip(coef[1], -self.weight_board, self.weight_board)
        except Exception as e:
            logging.error(f"Failed to upgrade weights: {str(e)}")
            raise NetworkError("Weight upgrade failed") from e

    def result(self, coefs: str) -> None:
        try:
            with open(coefs, 'w') as f:
                dump({
                    "A": cp.asnumpy(self.A_t[0]).tolist(),
                    "B": cp.asnumpy(self.B_t[0]).tolist(),
                    "C_1": cp.asnumpy(self.C_1_t[0]).tolist(),
                    "C_2": cp.asnumpy(self.C_2_t[0]).tolist(),
                    "d": cp.asnumpy(self.d[0]).tolist()
                }, f, indent=4)
        except Exception as e:
            logging.error(f"Failed to save results: {str(e)}")
            raise NetworkError("Failed to save network weights") from e