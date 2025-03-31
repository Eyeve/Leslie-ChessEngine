import numpy as np


a = np.array([[1, 2], [3, 4]], dtype=np.float16)
a = a*64000
print(a)
a = np.clip(a, -32768, 32767)
print(a)