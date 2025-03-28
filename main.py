import numpy as np
import json
from numpy.typing import NDArray
from params import *

# TODO try except


# TODO network archeticture
def func(X: NDArray):
    return 0

# the norm of the gradient matrix
def norm(*matrices: NDArray) -> int:
    result = 0
    for matrix in matrices:
        result += np.linalg.norm(matrix)
    return result

# TODO calculating the optimal step of the next iteration of descent
def next_step() -> int:
    return 0

# TODO gradient matrices
def grad(x):
    return ()

# parsing training json data into 
# input layer matrices and the expected result
# X_1, X_2, Y
def read_db(file: str): 
    with open(file, 'r') as f:
        for i, line in enumerate(f):

            if i >= L:
                break

            note = json.loads(line)

            Y[i] = note['count']
            X_2[0][i] = 1 if note['color'] == "w" else 0
            X_2[1][i] = ~ X_2[0][i]

            num = note['fen']
            b = ''
            for j in range(M):
                X_1[i][M-1-j] = num & 1
                num >>= 1
    del note, line, num
    return 0

# parsing a json file with coefficients
# into matrices of the inner and output layers
# A, B, C, d
def read_coefs(file: str):
    with open(coefs, 'r') as f:
        data = json.load(f)
        A[0] = np.array(data['A'])
        B[0] = np.array(data['B'])
        C[0] = np.array(data['C'])
        d[0] = np.array(data['d'])
    del data
    return 0

if __name__ == "__main__":
    #L - count of values
    #M - input layer size
    #N - inner later size

    # weights of the inner layer, matrix NxM || old and new
    A = np.zeros((3, N, M), dtype=weight_type)

    # shifting the inner layer, matrix Nx1 || old and new
    B = np.zeros((3, N, 1), dtype=weight_type)

    # weights of the output layer for each active color, matrix Nx2 || old and new
    C = np.zeros((3, N, 2), dtype=weight_type)

    # shifting the output layer, integet || old and new
    d = np.zeros((3, 1), dtype=weight_type)

    # expected output values, matrix LX1
    Y = np.zeros((L, 1), dtype=output_type)

    # input positions, matrix MxL
    X_1 = np.zeros((M, L), dtype=output_type)

    # input active colors, matrix 2xL, each column is [color, not color]
    X_2 = np.zeros((2, L), dtype=output_type)


    # f(x) = (C` * ReLu(Ax+B)) + D - for one input vector
    # C` is 
    # C[:][1] if active color - 1 (white) 
    # or 
    # C[:][0] if active color - 0 (black) 
    # F(X) = diag[clip([X_1 * A]+[L_1 * B], -in_board, in_board) * C * X_2] fulli matrix form of network
    # Error(A, B, C, d) = ||Y - F(X)||
    # grad(er, A) =
    # grad(er, B) =
    # grad(er, C) =
    # grad(er, d) =
    
    read_db(in_f)
    read_coefs(coefs)

    while norm(A[1] - A[0], B[1] - B[0], C[1] - C[0], d[1] - d[0]) > er:
        step = next_step() #step update
        grad() #grads update

        #new matrices counting
        A[1] = A[0] - step*A[2]
        B[1] = B[0] - step*B[2]
        C[1] = C[0] - step*C[2]
        d[1] = d[0] - step*d[2]
        
        #matrices update
        A[0] = np.clip(A[1], -weight_board, weight_board)
        B[0] = np.clip(B[1], -weight_board, weight_board)
        C[0] = np.clip(C[1], -weight_board, weight_board)
        d[0] = np.clip(d[1], -weight_board, weight_board)