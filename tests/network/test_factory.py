import pytest
import numpy as np
from network import create_network, create_adam
from network.cpu import CPUNetwork, CPUAdam

class TestNetworkFactory:
    def test_create_network_cpu(self):
        net = create_network('cpu', N=10, M=20, L=5, weight_type=np.float32, 
                           input_type=np.float32, output_type=np.float32,
                           in_bord=1.0, out_bord=1.0, weight_board=1.0)
        assert isinstance(net, CPUNetwork)
        assert net.N == 10
        assert net.M == 20
        assert net.L == 5

    def test_create_network_case_insensitive(self):
        net1 = create_network('CPU', N=5, M=5, L=2, weight_type=np.float32,
                            input_type=np.float32, output_type=np.float32,
                            in_bord=1.0, out_bord=1.0, weight_board=1.0)
        net2 = create_network('cpu', N=5, M=5, L=2, weight_type=np.float32,
                            input_type=np.float32, output_type=np.float32,
                            in_bord=1.0, out_bord=1.0, weight_board=1.0)
        assert type(net1) == type(net2)

    def test_create_network_invalid_backend(self):
        with pytest.raises(ValueError) as exc_info:
            create_network('invalid')
        assert "Invalid backend: invalid" in str(exc_info.value)

class TestAdamFactory:
    def test_create_adam_cpu(self):
        optimizer = create_adam('cpu', count=5)
        assert isinstance(optimizer, CPUAdam)

    def test_create_adam_case_insensitive(self):
        opt1 = create_adam('CPU', count=3)
        opt2 = create_adam('cpu', count=3)
        assert type(opt1) == type(opt2)

    def test_create_adam_invalid_backend(self):
        with pytest.raises(ValueError) as exc_info:
            create_adam('invalid')

        assert "Invalid backend: invalid" in str(exc_info.value)