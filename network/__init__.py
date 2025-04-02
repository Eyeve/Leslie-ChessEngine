
from .base import NetworkError, BaseNetwork, BaseAdam



def create_network(backend='cpu', *args, **kwargs):
    """
    Factory function to create a neural network instance.
    
    Args:
        backend: 'cpu' or 'gpu'
        *args, **kwargs: Arguments passed to network constructor
        
    Returns:
        BaseNetwork: Instance of CPUNetwork or GPUNetwork
        
    Raises:
        ValueError: If backend is invalid
    """
    if backend.lower() == 'cpu':
        from .cpu import Network

        return Network(*args, **kwargs)
    elif backend.lower() == 'gpu':
        from .gpu import Network

        return Network(*args, **kwargs)
    else:
        raise ValueError(f"Invalid backend: {backend}. Use 'cpu' or 'gpu'")


def create_adam(backend='cpu', *args, **kwargs):
    """
    Factory function to create an Adam optimizer instance.
    
    Args:
        backend: 'cpu' or 'gpu'
        *args, **kwargs: Arguments passed to optimizer constructor
        
    Returns:
        BaseAdam: Instance of CPUAdam or GPUAdam
        
    Raises:
        ValueError: If backend is invalid
    """
    if backend.lower() == 'cpu':
        from .cpu import Adam

        return Adam(*args, **kwargs)
    elif backend.lower() == 'gpu':
        from .gpu import Adam

        return Adam(*args, **kwargs)
    else:
        raise ValueError(f"Invalid backend: {backend}. Use 'cpu' or 'gpu'")
