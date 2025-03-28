import numpy as np
import json
from numpy.typing import NDArray

# TODO network archeticture
def func(X: NDArray):
    return 0

# TODO the norm of the gradient matrix
def norm(*matrices: NDArray) -> int:
    return 0

# TODO calculating the optimal step of the next iteration of descent
def next_step() -> int:
    return 0

# TODO gradient matrices
def grad(x) -> tuple[NDArray]:
    return ()

# parsing training json data into 
# input layer matrices and the expected result
# X_1, X_2, Y
def read_db(file: str): 
    with open("./formed.json") as f:
        for i, line in enumerate(f):

            if i >= L:
                break

            note = json.loads(line)

            Y[0][i] = note['count']
            X_2[0][0][i] = 1 if note['color'] == "w" else 0
            X_2[0][1][i] = ~ X_2[0][i]

            num = note['fen']
            b = ''
            for j in range(M):
                X_1[0][i][M-1-j] = num & 1
                num >>= 1
    return 0

# TODO parsing a json file with coefficients
# into matrices of the inner and output layers
# A, B, C, d
def read_coefs(file: str) -> tuple[NDArray]: #3 matrix Nx(M+2)X3 from int_16
    return 0

# TODO updating the coefficients of 
# the inner and output layers
# writing to a json file
def update_coefs(file_path: str) -> None:
    with open(file_path, 'w') as f:
        pass
    A[0] = A[1]
    B[0] = B[1]
    C[0] = C[1]
    d[0] = d[1]
    return 0


if __name__ == "__main__":
    in_f = './input.json'    # input file path            
    coefs = './network.json' # coefs file path              
    L = 10                   # data count
    M = 768                  # input layer size 
    N = 2048                 # inner layer size  
    er = 10**(-3)            # error rate        
    step = 1                 # iteration step of gradient descent

    # weights of the inner layer, matrix NxM || old and new
    A = np.zeros((2, N, M), dtype='int16')

    # shifting the inner layer, matrix Nx1 || old and new
    B = np.zeros((2, N, 1), dtype='int16')

    # weights of the output layer for each active color, matrix Nx2 || old and new
    C = np.zeros((2, N, 2), dtype='int16')

    # shifting the output layer, integet || old and new
    d = np.zeros((2, 1), dtype='int16')

    # expected output values, matrix LX1 || old and new
    Y = np.zeros((2, L, 1), dtype='int16')

    # input positions, matrix MxL || old and new
    X_1 = np.zeros((2, M, L), dtype='bool')

    # input active colors, matrix 2xL, each column is [color, not color] || old and new
    X_2 = np.zeros((2, 2, L), dtype='bool')


    # f(x) = (C` * ReLu(Ax+B)) + D - for one input vector
    # C` is 
    # C[:][1] if active color - 1 (white) 
    # or 
    # C[:][0] if active color - 0 (black) 
    # F(X) = diag[max(0, [X_1 * A]+[L_1 * B]) * C * X_2] fulli matrix form of network
    # Error(A, B, C, d) = ||Y - F(X)||
    # grad(er, A) =
    # grad(er, B) =
    # grad(er, C) =
    # grad(er, d) =
    
    read_db(in_f)
    read_coefs(coefs)
    while norm(A[1] - A[0], B[1] - B[0], C[1] - C[0], d[1] - d[0]):
        step = next_step()
        A[1] = A[0] - step*A[1]
        B[1] = B[0] - step*B[1]
        C[1] = C[0] - step*C[1]
        d[1] = d[0] - step*d[1]
        