in_f = './data/test.json'    # input file path            
coefs = './data/network.json' # coefs file path              
L = 1000                # data count d.4*10^6
M = 768                  # input layer size d.768
N = 1024                 # inner layer size d.2048
er = 10**(-3)            # error rate        
con = 100                # iteration step of gradient descent
gamma = 0.9  # коэффициент инерции
velocity = 0  # начальная скорость

in_bord = 6
out_bord = 32000
weight_board = 65504

input_type = 'bool'
output_type = 'int16'
weight_type = 'float32'
