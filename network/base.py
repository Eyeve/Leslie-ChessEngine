
from abc import ABC, abstractmethod
from typing import Any
import logging


class NetworkError(Exception):
    pass


class BaseAdam(ABC):
    def __init__(self, count: int, alpha: float = 0.001, beta1: float = 0.9, 
                 beta2: float = 0.999, eps: float = 1e-8):
        try:
            if not isinstance(count, int) or count <= 0:
                raise ValueError("Count must be a positive integer")
            if not all(isinstance(x, (int, float)) for x in [alpha, beta1, beta2, eps]):
                raise ValueError("Parameters must be numeric values")
            if not (0 < alpha <= 1 and 0 < beta1 < 1 and 0 < beta2 < 1 and eps > 0):
                raise ValueError("Invalid parameter values")
                
            self.alpha = alpha
            self.beta1 = beta1
            self.beta2 = beta2
            self.steps = ()
            self.eps = eps
            self.m = [None] * count
            self.v = [None] * count
            self.t = 0
        except Exception as e:
            logging.error(f"Failed to initialize Adam optimizer: {str(e)}")
            raise NetworkError("Adam initialization failed") from e


class BaseNetwork(ABC):
    def __init__(self, N: int, M: int, L: int, weight_type: Any, input_type: Any, 
                 output_type: Any, in_bord: float, out_bord: float, weight_board: float):
        if not all(isinstance(x, int) for x in [N, M, L]) or not all(x > 0 for x in [N, M, L]):
            raise ValueError("N, M, L must be positive integers")
        if not all(isinstance(x, (int, float)) for x in [in_bord, out_bord, weight_board]):
            raise ValueError("Boundaries must be numeric values")
            
        self.N = N
        self.M = M
        self.L = L
        self.alpha = in_bord
        self.beta = out_bord
        self.weight_board = weight_board
        self.weight_type = weight_type
        self.input_type = input_type
        self.output_type = output_type

    @abstractmethod
    def read_db(self, in_f: str) -> None:
        pass

    @abstractmethod
    def read_coefs(self, coefs: str) -> None:
        pass

    @abstractmethod
    def create_coefs(self) -> None:
        pass

    @abstractmethod
    def norm(self, *arrays) -> float:
        pass

    @abstractmethod
    def Z(self) -> Any:
        pass

    @abstractmethod
    def T(self) -> Any:
        pass

    @abstractmethod
    def F(self) -> Any:
        pass

    @abstractmethod
    def E(self) -> None:
        pass

    @abstractmethod
    def grad(self) -> None:
        pass

    @abstractmethod
    def update(self, optim: BaseAdam) -> None:
        pass

    @abstractmethod
    def upgrade(self) -> None:
        pass

    @abstractmethod
    def result(self, coefs: str) -> None:
        pass
