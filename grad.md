```math
Y\ -\ Lx1\ из\ int16,\ плотность\ 100\%
```math
X_1\ -\ L×M\ из\ bool,\ плотность\ 2-4\%
```
```math
X_2\ ;\ Y_1\ -\ L×1\ из\ bool,\ плотность\ 50\%
```
```math
A^T\ -\ M×N\ из\ float16,\ плотность\ 100\%
```
```math
b^T\ ;\ C_1^T\ ;\ C_2^T\ -\ 1×N\ из\ float16,\ плотность\ 100\%
```
```math
d\ -\ 1×1\ из\ float16
```
```math
1_L\ -\ L×1\ из\ единиц
```
```math
1_N\ -\ N×1\ из\ единиц
```
```math
\alpha = 6\ ;\ \beta = 32000
```
```math
\times\ -\ обычное\ матричное\ умножение
```
```math
\odot\ -\ поэлементное\ умножение
```
```math
S = X_1 \times A^T + 1_L \times b^T\ \ \ и\ \ \ Z = clip(S, -\alpha, \alpha) \ \ [L  \times N]]
```
```math
M_Z = I_{\{-\alpha \leq Z \leq \alpha\}} \ ;\ Z = S \odot M_Z
```
```math
T = X_2 \times {C_1}^T + (1-X_2) \times {C_2}^T \ ;\ [L \times N]
```
```math
U = (Z \odot T) \times 1_N + (d \cdot 1_L) \ ;\ [L \times 1]
```
```math
F = clip(U, -\beta, \beta) \ \ [L \times 1] \ ;\ M_F = I_{\{-\beta \leq F \leq \beta\}}
```
```math
F = U \odot M_F
```
```math
E = \frac{1}{2}||Y - F||^2
```
```math
\theta = \{A^T, b^T, {C_1}^T, {C_2}^T, d\}
```
```math
E(\theta) = \frac{1}{2}||Y - ([([(X_1 \times A^T + 1_L \times b^T) \odot M_Z] \odot [X_2 \times {C_1}^T + (1-X_2) \times {C_2}^T]) \times 1_N + (d \cdot 1_N)] \odot M_F)||^2
```

```math
\frac{\partial E}{\partial A^T} = \frac{\partial E}{\partial F} \frac{\partial F}{\partial U} \frac{\partial U}{\partial Z} \frac{\partial Z}{\partial S} \frac{\partial S}{\partial A^T}
```
```math
\frac{\partial E}{\partial b^T} = \frac{\partial E}{\partial F} \frac{\partial F}{\partial U} \frac{\partial U}{\partial Z} \frac{\partial Z}{\partial S} \frac{\partial S}{\partial b^T}
```
```math
\frac{\partial E}{\partial {C_1}^T} = \frac{\partial E}{\partial F} \frac{\partial F}{\partial U} \frac{\partial U}{\partial T} \frac{\partial T}{\partial {C_1}^T}
```
```math
\frac{\partial E}{\partial {C_2}^T} = \frac{\partial E}{\partial F} \frac{\partial F}{\partial U} \frac{\partial U}{\partial T} \frac{\partial T}{\partial {C_2}^T}
```
```math
\frac{\partial E}{\partial d} = \frac{\partial E}{\partial F} \frac{\partial F}{\partial U} \frac{\partial U}{\partial d}
```
```math
\frac{\partial E}{\partial F} = F - Y = \Delta \ ;\ [L \times 1]
```
```math
\frac{\partial F}{\partial U} =\ \ ) \odot M_F \ ;\ [L \times 1]
```
```math
\frac{\partial U}{\partial Z} =\ \  ) \times {1_N}^T) \odot T \ ;\ [L \times N]
```
```math
\frac{\partial U}{\partial T} =\ \  ) \times {1_N}^T) \odot Z \ ;\ [L \times N]
```
```math
\frac{\partial Z}{\partial S} =\ \ ) \odot M_Z \ ;\ [L \times N]
```
```math
\frac{\partial S}{\partial A^T} =\ \ {X_1}^T \times (\ ;\ [M \times L]
```
```math
\frac{\partial S}{\partial b^T} =\ \ {1_N}^T \times (\ ;\ [1 \times N]
```
```math
\frac{\partial T}{\partial {C_1}^T} =\ \ {X_2}^T \times (\ ;\ [L \times N]
```
```math
\frac{\partial T}{\partial {C_2}^T} =\ \ (1 - X_2)^T \times (\ ;\ [L \times N]
```
```math
\frac{\partial U}{\partial d} =\ \ {1_L}^T \times (\ ;\ [1 \times L]
```
```math
\nabla E_{A^T} =\ \ {X^T} \times ((\Delta \odot M_F \times {1_N}^T) \odot T \odot M_Z) \ ;\ [M \times L]
```
```math
\nabla E_{b^T} =\ \ {1_L}^T \times ((\Delta \odot M_F \times {1_N}^T) \odot T \odot M_Z) \ ;\ [1 \times N]
```
```math
\nabla E_{C_1}^T =\ \ {X_2}^T \times ((\Delta \odot M_F \times {1_N}^T) \odot Z \odot M_Z) \ ;\ [1 \times N]
```
```math
\nabla E_{C_2}^T =\ \ (1 - X_2)^T \times ((\Delta \odot M_F \times {1_N}^T) \odot Z \odot M_Z) \ ;\ [1 \times N]
```
```math
\nabla E_d =\ \ {1_L}^T \times (\Delta \odot M_F ) \ ;\ [1 \times 1]
```
```math
||\nabla E(\theta)||^2_2 = \sum_{i=1}^{L} \sum_{j=1}^{M} (\frac{\partial E}{\partial A^T})^2_{ij} + \sum_{i=1}^{L} (\frac{\partial E}{\partial b^T})^2_{i} + \sum_{i=1}^{N} (\frac{\partial E}{\partial {C_1}^T})^2_{i} + \sum_{i=1}^{N} (\frac{\partial E}{\partial {C_2}^T})^2_{i} + (\frac{\partial E}{\partial d})^2
```
```math
||\nabla E(\theta)||^2_2 = \sum_{\theta}||\nabla E_{\theta}||^2_F
```
```math
\eta = \frac{c}{||\nabla E(\theta)||^2_2}
```
```math
\theta_{i+1} = \theta_{i} - \eta \nabla E(\theta_i) \ ,\ ||\nabla E(\theta_{i+1})||^2_2 > \varepsilon 
```