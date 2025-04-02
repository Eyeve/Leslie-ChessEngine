## Network input and parameter dimensions:
```math
X_1 \text{ - } L \times M \text{ boolean matrix representing input positions}
```
```math
X_2, Y_1 \text{ - } L \times 1 \text{ boolean vectors for active color and target values}
```
```math
A^T \text{ - } M \times N \text{ float16 matrix of hidden layer weights}
```
```math
b^T, C_1^T, C_2^T \text{ - } 1 \times N \text{ float16 vectors for biases and output weights}
```
```math
d \text{ - } 1 \times 1 \text{ float16 scalar output bias}
```
```math
1_L \text{ - } L \times 1 \text{ vector of ones for broadcasting}
```
```math
1_N \text{ - } N \times 1 \text{ vector of ones for summation}
```

## Constants and operators:
```math
\alpha = 6, \beta = 32000 \text{ (clipping bounds)}
```
```math
\times \text{ denotes matrix multiplication}
```
```math
\odot \text{ denotes element-wise multiplication}
```

## Forward pass equations:
```math
S = X_1 \times A^T + 1_L \times b^T \text{ (hidden layer pre-activation)} \\ Z = clip(S, -\alpha, \alpha) \text{ (clipped activation)} [L \times N]
```
```math
M_Z = I_{\{-\alpha \leq Z \leq \alpha\}} \text{ (clipping mask)}, Z = S \odot M_Z \text{ (masked activation)}
```
```math
T = X_2 \times {C_1}^T + (1-X_2) \times {C_2}^T \text{ (color-dependent weights)} [L \times N]
```
```math
U = (Z \odot T) \times 1_N + (d \cdot 1_L) \text{ (output pre-activation)} [L \times 1]
```
```math
F = clip(U, -\beta, \beta) \text{ (clipped output)} [L \times 1], M_F = I_{\{-\beta \leq F \leq \beta\}} \text{ (output mask)}
```
```math
F = U \odot M_F \text{ (masked output)}
```

## Loss function:
```math
E = \frac{1}{2}||Y - F||^2 \text{ (mean squared error loss)}
```
```math
\theta = \{A^T, b^T, {C_1}^T, {C_2}^T, d\} \text{ (network parameters)}
```
```math
E(\theta) = \frac{1}{2}||Y - ([([(X_1 \times A^T + 1_L \times b^T) \odot M_Z] \odot [X_2 \times {C_1}^T + (1-X_2) \times {C_2}^T]) \times 1_N + (d \cdot 1_N)] \odot M_F)||^2 \text{ (full loss expression)}
```

## Chain rule expressions:
```math
\text{Chain rule for hidden weights:} \\ \frac{\partial E}{\partial A^T} = \frac{\partial E}{\partial F} \frac{\partial F}{\partial U} \frac{\partial U}{\partial Z} \frac{\partial Z}{\partial S} \frac{\partial S}{\partial A^T}
```
```math
\text{Chain rule for hidden bias:} \\ \frac{\partial E}{\partial b^T} = \frac{\partial E}{\partial F} \frac{\partial F}{\partial U} \frac{\partial U}{\partial Z} \frac{\partial Z}{\partial S} \frac{\partial S}{\partial b^T}
```
```math
\text{Chain rule for white output weights:} \\ \frac{\partial E}{\partial {C_1}^T} = \frac{\partial E}{\partial F} \frac{\partial F}{\partial U} \frac{\partial U}{\partial T} \frac{\partial T}{\partial {C_1}^T}
```
```math
\text{Chain rule for black output weights:} \\ \frac{\partial E}{\partial {C_2}^T} = \frac{\partial E}{\partial F} \frac{\partial F}{\partial U} \frac{\partial U}{\partial T} \frac{\partial T}{\partial {C_2}^T}
```
```math
\text{Chain rule for output bias:} \\ \frac{\partial E}{\partial d} = \frac{\partial E}{\partial F} \frac{\partial F}{\partial U} \frac{\partial U}{\partial d}
```

## Intermediate gradients:
```math
\text{Output error gradient:} \\ \frac{\partial E}{\partial F} = F - Y = \Delta [L \times 1]
```
```math
\text{Output clipping gradient:} \\ \frac{\partial F}{\partial U} = M_F [L \times 1]
```
```math
\text{Hidden-to-output gradient:} \\ \frac{\partial U}{\partial Z} = ({1_N}^T) \odot T [L \times N]
```
```math
\text{Weight selection gradient:} \\ \frac{\partial U}{\partial T} = ({1_N}^T) \odot Z [L \times N]
```
```math
\text{Hidden layer clipping gradient:} \\ \frac{\partial Z}{\partial S} = M_Z [L \times N]
```
```math
\text{Input-to-hidden gradient:} \\ \frac{\partial S}{\partial A^T} = {X_1}^T [M \times L]
```
```math
\text{Hidden bias gradient:} \\ \frac{\partial S}{\partial b^T} = {1_N}^T [1 \times N]
```
```math
\text{White output weight gradient:} \\ \frac{\partial T}{\partial {C_1}^T} = {X_2}^T [L \times N]
```
```math
\text{Black output weight gradient:} \\ \frac{\partial T}{\partial {C_2}^T} = (1 - X_2)^T [L \times N]
```
```math
\text{Output bias gradient:} \\ \frac{\partial U}{\partial d} = {1_L}^T [1 \times L]
```

## Final gradients:
```math
\text{Final hidden weight gradient:} \\ \nabla E_{A^T} = {X^T} \times ((\Delta \odot M_F \times {1_N}^T) \odot T \odot M_Z) [M \times L]
```
```math
\text{Final hidden bias gradient:} \\ \nabla E_{b^T} = {1_L}^T \times ((\Delta \odot M_F \times {1_N}^T) \odot T \odot M_Z) [1 \times N]
```
```math
\text{Final white output weight gradient:} \\ \nabla E_{C_1}^T = {X_2}^T \times ((\Delta \odot M_F \times {1_N}^T) \odot Z \odot M_Z) [1 \times N]
```
```math
\text{Final black output weight gradient:} \\ \nabla E_{C_2}^T = (1 - X_2)^T \times ((\Delta \odot M_F \times {1_N}^T) \odot Z \odot M_Z) [1 \times N]
```
```math
\text{Final output bias gradient:} \\ \nabla E_d = {1_L}^T \times (\Delta \odot M_F) [1 \times 1]
```

## Gradient norm and learning:
```math
\text{Total gradient norm (sum of squares):} \\ ||\nabla E(\theta)||^2_2 = \sum_{i=1}^{L} \sum_{j=1}^{M} (\frac{\partial E}{\partial A^T})^2_{ij} + \sum_{i=1}^{L} (\frac{\partial E}{\partial b^T})^2_{i} + \sum_{i=1}^{N} (\frac{\partial E}{\partial {C_1}^T})^2_{i} + \sum_{i=1}^{N} (\frac{\partial E}{\partial {C_2}^T})^2_{i} + (\frac{\partial E}{\partial d})^2
```
```math
\text{Equivalent norm expression:} \\ ||\nabla E(\theta)||^2_2 = \sum_{\theta}||\nabla E_{\theta}||^2_F
```
```math
\text{Adaptive learning rate:} \\ \eta = \frac{c}{||\nabla E(\theta)||^2_2}
```
```math
\text{Parameter update rule (continue while gradient norm > ε):} \\ \theta_{i+1} = \theta_{i} - \eta \nabla E(\theta_i), ||\nabla E(\theta_{i+1})||^2_2 > \varepsilon
```