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
\Large z = CReLu(Ax+B, -32767, 32767)
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
CReLu(C^T[:N] \cdot z + d, -32000, 32000) & \text{if } x` = 0 \\
CReLu(C^T[N+1:] \cdot z + d, -32000, 32000),     & \text{if } x` = 1
\end{cases}
```

## Full function
```math
f(x, x`) = 
\begin{cases}
CReLu(C^T[:N] \cdot CReLu(Ax+B, -32767, 32767) + d, -32000, 32000) & \text{if } x` = 0 \\
CReLu(C^T[N+1:] \cdot CReLu(Ax+B, -32767, 32767) + d, -32000, 32000),     & \text{if } x` = 1
\end{cases}
```

<br><br>

# Learning

