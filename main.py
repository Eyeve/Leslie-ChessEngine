from network import Network
from params import *

"""
Mathematical Overview:
The network implements gradient descent optimization for a neural network with the following structure:
- Input layer (M=768 binary neurons) represents chess position
- Hidden layer (N=1024 neurons) with clipped ReLU activation in range [-6,6]
- Output layer (1 neuron) with clipped activation in range [-32000,32000]

The loss function E(θ) is MSE between predicted and target values:
E(θ) = 1/2||Y - f(X,θ)||^2 

Where f(X,θ) is the full network function:
f(X,θ) = clip((clip(XA + B, -α, α) ⊙ C) + d, -β, β)

Gradients are computed via chain rule and updated using:
θ_new = θ - η∇E(θ) where η = c/||∇E(θ)||₂
"""

try:
    # Initialize network with specified parameters
    net = Network(N, M, L, weight_type, input_type, output_type, in_bord, out_bord, weight_board)
except Exception as e:
    print(f"Error initializing network: {e}")
    exit(1)

try:
    # Load training data and initial weights
    net.read_db(in_f)
    net.read_coefs(coefs)
except FileNotFoundError:
    print("Error: Training data or coefficient files not found")
    exit(1)
except Exception as e:
    print(f"Error loading data: {e}")
    exit(1)

try:
    # Main training loop
    while True:
        try:
            # Calculate gradients of loss wrt all parameters
            net.grad()
            
            # Calculate L2 norm of gradients for step size
            norm = net.norm()
            
            # Compute adaptive learning rate
            step = con/norm
            
            # Update parameters using gradient descent
            net.update(step)
            
            # Clip weights to prevent overflow
            net.upgrade()

            # Check convergence criterion
            if norm < er:
                break
                
        except ZeroDivisionError:
            print("Error: Division by zero in gradient calculation")
            break
        except Exception as e:
            print(f"Error in training iteration: {e}")
            break

    # Save trained network parameters
    net.result()
    
except Exception as e:
    print(f"Error saving results: {e}")
    exit(1)