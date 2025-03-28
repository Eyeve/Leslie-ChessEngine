import json
import numpy as np
from params import *


with open(coefs, "w") as f:
    A = np.random.normal(0, np.sqrt(2/M), size=(N, M))
    C = np.random.normal(0, np.sqrt(2/M), size=(N, 2))
    B = np.zeros((N, 1))
    d = 0.0
    json.dump({"A": A.tolist(), "B": B.tolist(), "C": C.tolist(), "d": d}, f)


    