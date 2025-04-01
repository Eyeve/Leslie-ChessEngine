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

from math import exp, log10, trunc
from network import Adam, Network
from params import *

try:
    # Initialize network with specified parameters
    net = Network(N, M, L, weight_type, input_type, output_type, in_bord, out_bord, weight_board)
except Exception as e:
    print(f"Error initializing network: {e}")
    exit(1)

try:
    # Load training data and initial weights
    net.read_db(in_f)
    print("Choose the mood (1 - read coefs, 2 - new coefs): ")
    mood = input()
    if mood == '2':
        net.create_coefs()
    elif mood == '1':
        net.read_coefs(coefs)
    else:
        print("Wrong mood")
        exit(1)
except FileNotFoundError:
    print("Error: Training data or coefficient files not found")
    exit(1)
except Exception as e:
    print(f"Error loading data: {e}")
    exit(1)

try:
    # Main training loop
    norm_old = 0
    er_old = 0
    er_del_old = 0
    optimizer = Adam(5, alpha)
    while True:
        try:
            # Calculate gradients of loss wrt all parameters
            net.grad()
            
            # Calculate L2 norm of gradients for step size
            norm = net.norm(*(coef[2] for coef in net.coefs))

            error = net.norm(net.e)/net.L
            er_del = error - er_old

            net.update(optimizer)
            
            print(f"iter: {optimizer.t} \t del: {norm-norm_old:.4e} \t st: {net.norm(*(coef[1] - coef[0] for coef in net.coefs)):.4e} \t erdel: {er_del:.4e} \t er: {error:.4e} \t alpha: {optimizer.alpha:.2e}")
            
            # Clip weights to prevent overflow
            net.upgrade()

            norm_old = norm
            er_old = error

            # Check convergence criterion
            if norm < er:
                break
                
        except ZeroDivisionError:
            print("Error: Division by zero in gradient calculation")
            break
        except KeyboardInterrupt:
            print("\nОбучение прервано пользователем (Ctrl+C). Сохраняем модель...")
            net.result()
            print("\nЗаписано, нажмите Enter, чтобы прервать")
            if input() == '':
                break
            else:
                continue

        except Exception as e:
            print(f"Error in training iteration: {e}")
            break

    # Save trained network parameters
    net.result()
    
except Exception as e:
    print(f"Error saving results: {e}")
    exit(1)
