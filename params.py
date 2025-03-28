in_f = './resourses/input.json'    # input file path            
coefs = './resourses/network.json' # coefs file path              
L = 10                   # data count d.4*10^6
M = 768                  # input layer size d.768
N = 256                 # inner layer size d.2048
er = 10**(-3)            # error rate        
step = 1                 # iteration step of gradient descent

inner_bord = 32767
out_bord = 32000
weight_board = 65504

input_type = 'bool'
output_type = 'int16'
weight_type = 'float16'
