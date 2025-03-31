# Leslie-ChessEngine network

## Абсолютное когнитивное ничтожество.

![Не Лесли](https://github.com/Eyeve/Leslie-ChessEngine/blob/learning/resourses/Jarvis.jpg)

# Network architecture

## Initial conditions
1. `L` - The amount of training data
2. `M` - The size of the input layer
3. `N` - The size of the inner layer

## Input

### Position - a set of combinations (cell, type, color)

```math
\Large Cell \in \{0,...,63\}
```

<br>

```math
\Large Type \in \{0,...,5\} \\
```

```math
Pawn = 0, Knight = 1, Bishop = 2, Rook = 3, Queen = 4, King = 5
```

<br>

```math
\Large Color \in \{0, 1\}
```
```math
White = 0,\ Black = 1
```

### The Chinese Remainder Theorem

```math
(\mathbb{N}/_{64\mathbb{N}};\ \mathbb{N}/_{6\mathbb{N}};\ \mathbb{N}/_{2\mathbb{N}}) \simeq \mathbb{N}/_{768\mathbb{N}}
```

<br>

```math
(color,\ type,\ cell) \hookrightarrow index
```

```math
index = (color \times 64 \times 6) + (type \times 64) + cell
```

### The position is a 768-bit number.

```math
\Large Position = \sum (1 << index_i)
```

## Output
```math
\Large value \in [-32000; 32000]
```

## Inner Layer

```math
\alpha = 6\ ;\ \beta = 32000
```

```math
A \in \mathcal{M}(M, N)\ of\ int16\ - matrix\ of\ inner\ layer\ weights
```
```math
B \in \mathcal{M}(N, 1)\ of\ int16\ - vector\ of\ inner\ layer\ bias
```
```math
CReLu(x, a, b) = 
\begin{cases}
a, & \text{if } x < a \\
x,     & \text{if } a \leq x \leq b \\
b,  & \text{if } x > b
\end{cases}
```
```math
\Large x \in \mathcal{M}(M, 1)\ of\ bool\ - input\ position\ vector
```
```math
z \in \mathcal{M}(N, 1)\ of\ int16\ - vector-result\ of\ inner\ layer
```
```math
\Large z = CReLu(Ax+B, -\alpha, \alpha)
```

## Output layer
```math
C \in \mathcal{M}(2N, 1)\ of\ int16\ - vector\ of\ output\ layer\ weights\\ two\ part\ for\ two\ active\ color\
```
```math
d\ (int16) - output\ layer\ bies
```
```math
\Large x`\ (bool)\ -\ active\ color\ of\ input
```
```math
z \in \mathcal{M}(N, 1)\ of\ int16\ - vector-result\ of\ inner\ layer
```
```math
CReLu(x, a, b) = 
\begin{cases}
a, & \text{if } x < a \\
x,     & \text{if } a \leq x \leq b \\
b,  & \text{if } x > b
\end{cases}
```

```math
\Large Y(z, x`) = 
\begin{cases}
CReLu(C^T[:N] \cdot z + d, -\beta, \beta) & \text{if } x` = 0 \\
CReLu(C^T[N+1:] \cdot z + d, -\beta, \beta),     & \text{if } x` = 1
\end{cases}
```

## Full function
```math
f(x, x`) = 
\begin{cases}
CReLu(C^T[:N] \cdot CReLu(Ax+B, -\alpha, \alpha) + d, -\beta, \beta) & \text{if } x` = 0 \\
CReLu(C^T[N+1:] \cdot CReLu(Ax+B, -\alpha, \alpha) + d, -\beta, \beta),     & \text{if } x` = 1
\end{cases}
```

<br><br>

# Learning

## Training data
```math
X_1 \in \mathcal{M}(L, M)\ of\ bool\ - a\ matrix\ consisting\ of\ L\ position\ entries\\ represented\ as\ M\ binary\ values
```

<br>

```math
X_2 \in \mathcal{M}(L, 1)\ of\ bool\
```

<br>

```math
Y \in \mathcal{M}(L, 1)\ of\ init16\ - a\ column\ vector\ of\ L\ expected\ values.
```

## New function
In order to optimize calculations, the entire function has been rewritten in an exclusively matrix form or matrix *numpy* operation.

1.  CRelu(x, a, b) -> NDArray.clip(a, b) 
    -  hereinafter referred to as "clip"
2. NDArray.dot(NDArray) - the matrix product
    - hereinafter referred to as (⋅)
3. C -> (C_1, C_2) two colums
4. 1<sub>L</sub> A column vector of L units
5. 1<sub>N</sub> A column vector of N units
6. The dependence on the active color is implemented as follows:
   
```math
X_2 \times {C_1}^T + (1-X_2) \times {C_2}^T \ ;\ [L \times N]
```

## Loss function

```math
\times\ -\ the usual \ matrix\ multiplication
```
```math
\odot\ -\ element\ wise\ multiplication
```

```math
\theta = \{A^T, b^T, {C_1}^T, {C_2}^T, d\}
```

```math
E(\theta) = \frac{1}{2}||Y - ([([(X_1 \times A^T + 1_L \times b^T) \odot M_Z] \odot [X_2 \times {C_1}^T + (1-X_2) \times {C_2}^T]) \times 1_N + (d \cdot 1_N)] \odot M_F)||^2
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

**For more detailed calculations, read grad.md**

## Gradient descent

```math
||\nabla E(\theta)||^2_2 = \sum_{\theta}||\nabla E_{\theta}||^2_F
```
```math
\eta = \frac{c}{||\nabla E(\theta)||^2_2}
```
```math
\theta_{i+1} = \theta_{i} - \eta \nabla E(\theta_i) \ ,\ ||\nabla E(\theta_{i+1})||^2_2 > \varepsilon 
```