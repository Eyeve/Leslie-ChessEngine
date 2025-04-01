import numpy as np
from numpy.testing import assert_, assert_equal, assert_array_equal
import pytest
from network import Network

class TestNetwork:
    @pytest.fixture
    def network(self):
        return Network(1024, 768, 5, 'float32', 'int8', 'int32', -6, 32000, 6)
        
    def test_network_initialization(self, network):
        assert_equal(network.N, 1024)
        assert_equal(network.M, 768)
        assert_equal(network.L, 5)
        
    def test_invalid_parameters(self):
        with pytest.raises(Exception):
            Network(-1, 768, 5, 'float32', 'int8', 'int32', -6, 32000, 6)
            
    def test_weight_initialization(self, network):
        assert_(isinstance(network.A_t, np.ndarray))
        assert_equal(network.A_t.shape, (3, 768, 1024))
        
    def test_gradient_calculation(self, network):
        network.read_db("resourses/input.json")
        network.create_coefs()
        network.grad()
        assert_(isinstance(network.A_t[2], np.ndarray))
        assert_(isinstance(network.B_t[2], np.ndarray))
        assert_(isinstance(network.C_1_t[2], np.ndarray))
        
    def test_norm_calculation(self, network):
        network.read_db("resourses/input.json")
        network.create_coefs()
        network.grad()
        norm = network.norm()
        assert_(isinstance(norm, (float, np.float32, np.float64)))
        assert_(norm >= 0)
        
    def test_parameter_update(self, network):
        network.read_db("resourses/input.json")
        network.create_coefs()
        initial_A = network.A_t.copy()
        network.grad()
        network.update(0.1)
        assert_(not np.array_equal(network.A_t, initial_A))
        
    def test_weight_clipping(self, network):
        network.read_db("resourses/input.json")
        network.create_coefs()
        network.upgrade()
        assert_(np.all(network.A_t <= network.weight_board))
        assert_(np.all(network.A_t >= -network.weight_board))
