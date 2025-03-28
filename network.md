# Network architecture

## Initial conditions
1. `L` - The amount of training data
2. `M` - The size of the input layer
3. `N` - The size of the inner layer

## Input

### Position - a set of combinations (cell, type, color)

```math
Cell \in \{0,...,63\}
```

<br>

```math
Type \in \{0,...,5\} \\
```

```math
Pawn = 0, Knight = 1, Bishop = 2, Rook = 3, Queen = 4, King = 5
```

<br>

```math
Color \in \{0, 1\}
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
Position = \sum (1 << index_i)
```

## Output
```math
value \in [-32000; 32000]
```
