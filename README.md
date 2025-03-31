# Leslie-ChessEngine network

## Абсолютное когнитивное ничтожество.

![Не Лесли](https://github.com/Eyeve/Leslie-ChessEngine/blob/learning/resourses/Jarvis.jpg)

## Mathematical Description

This project implements a neural network for chess position evaluation using gradient descent optimization.

### Network Architecture

The network consists of:

```math
\Large Input\ layer: X_1 \in \{0,1\}^{L×M}, M=768
```
```math
\Large Hidden\ layer: Z \in \mathbb{R}^{L×N}, N=1024
```
```math
\Large Output\ layer: F \in \mathbb{R}^{L×1}
```

### Input Layer Encoding

The input layer represents chess positions as 768-bit vectors:

1. Each piece is encoded by (position, type, color):
   - Position ∈ {0,...,63} (chess board squares)
   - Type ∈ {0,...,5} (Pawn=0, Knight=1, Bishop=2, Rook=3, Queen=4, King=5)
   - Color ∈ {0,1} (White=0, Black=1)

2. Using Chinese Remainder Theorem, each piece maps to a unique index:
```math
\Large index = (color × 64 × 6) + (type × 64) + position
```

3. The final input vector X is created by setting bits:
```math
X[index] = 1 \text{ for each piece on board}
```

### Hidden Layer Computation

The hidden layer transforms the input using:

1. Linear transformation with weights A and bias b:
```math
\Large S = X_1 \times A^T + 1_L \times b^T
```

2. Activation with clipping function:
```math
Z = clip(S, -\alpha, \alpha), \alpha = 6
```
```math
clip(x,a,b) = max(min(x,b),a)
```

### Output Layer Computation

The output layer produces evaluations based on active color:

1. Color-dependent weight selection:
```math
T = X_2 \times C_1^T + (1-X_2) \times C_2^T
```
Where C₁ is used for white's move, C₂ for black's move

2. Final computation with clipping:
```math
\Large U = (Z \odot T) \times 1_N + (d \cdot 1_L)
```
```math
\Large F = clip(U, -\beta, \beta), \beta = 32000
```

The output F represents position evaluation in centipawns (-32000 to +32000)

### Parameters

Network parameters θ include:
```math
A^T \in \mathbb{R}^{M×N} - \text{Hidden layer weights}
```
```math
b^T \in \mathbb{R}^{1×N} - \text{Hidden layer bias}
```
```math
C_1^T, C_2^T \in \mathbb{R}^{1×N} - \text{Output weights for white/black}
```
```math
d \in \mathbb{R} - \text{Output bias}
```

### Loss Function

Mean squared error between predictions and targets:
```math
\Large E = \frac{1}{2}||Y - F||^2
```

### Gradient Descent

Parameters are updated using:
```math
\Large \theta_{i+1} = \theta_i - \eta \nabla E(\theta_i)
```

Where learning rate η is adaptive:
```math
\Large \eta = \frac{c}{||\nabla E(\theta)||^2_2}
```

### Gradient Computation

Gradients are computed via chain rule:
```math
\frac{\partial E}{\partial A^T} = \frac{\partial E}{\partial F} \frac{\partial F}{\partial U} \frac{\partial U}{\partial Z} \frac{\partial Z}{\partial S} \frac{\partial S}{\partial A^T}
```
```math
\frac{\partial E}{\partial b^T} = \frac{\partial E}{\partial F} \frac{\partial F}{\partial U} \frac{\partial U}{\partial Z} \frac{\partial Z}{\partial S} \frac{\partial S}{\partial b^T}
```
```math
\frac{\partial E}{\partial C_1^T} = \frac{\partial E}{\partial F} \frac{\partial F}{\partial U} \frac{\partial U}{\partial T} \frac{\partial T}{\partial C_1^T}
```
```math
\frac{\partial E}{\partial C_2^T} = \frac{\partial E}{\partial F} \frac{\partial F}{\partial U} \frac{\partial U}{\partial T} \frac{\partial T}{\partial C_2^T}
```
```math
\frac{\partial E}{\partial d} = \frac{\partial E}{\partial F} \frac{\partial F}{\partial U} \frac{\partial U}{\partial d}
```

Final gradient expressions:
```math
\nabla E_{A^T} = X_1^T \times ((\Delta \odot M_F \times {1_N}^T) \odot T \odot M_Z)
```
```math
\nabla E_{b^T} = {1_L}^T \times ((\Delta \odot M_F \times {1_N}^T) \odot T \odot M_Z)
```
```math
\nabla E_{C_1}^T = {X_2}^T \times ((\Delta \odot M_F \times {1_N}^T) \odot Z \odot M_Z)
```
```math
\nabla E_{C_2}^T = (1 - X_2)^T \times ((\Delta \odot M_F \times {1_N}^T) \odot Z \odot M_Z)
```
```math
\nabla E_d = {1_L}^T \times (\Delta \odot M_F)
```

Where:
- Δ = F - Y is the output error
- M_F is the output clipping mask (-β ≤ F ≤ β)
- M_Z is the hidden layer clipping mask (-α ≤ Z ≤ α)

### Implementation Details

- All weights use float16 precision
- Input positions are 768-bit binary vectors
- Training continues until gradient norm < ε
- Weights are clipped to prevent overflow
- Training data is read from JSON files containing FEN positions and evaluations

### Usage

1. Configure parameters in params.py
2. Prepare training data in JSON format
3. Run main.py to train the network
4. Results are saved to result.json