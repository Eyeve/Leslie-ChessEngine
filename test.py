import json
import numpy as np
from params import *

# weights of the inner layer, matrix NxM || old and new
A = np.zeros((3, N, M), dtype=weight_type)

# shifting the inner layer, matrix Nx1 || old and new
B = np.zeros((3, N, 1), dtype=weight_type)

# weights of the output layer for each active color, matrix Nx2 || old and new
C = np.zeros((3, N, 2), dtype=weight_type)

# shifting the output layer, integet || old and new
d = np.zeros((3, 1), dtype=weight_type)

with open(coefs, 'r') as f:
    data = json.load(f)
    A[0] = np.array(data['A'])
    B[0] = np.array(data['B'])
    C[0] = np.array(data['C'])
    d[0] = np.array(data['d'])

print(type(A[0][1][5]))