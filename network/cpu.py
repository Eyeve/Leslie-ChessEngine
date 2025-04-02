import numpy as np
from numpy.typing import NDArray
from json import dump, load, loads, JSONDecodeError
import logging
from .base import BaseNetwork, BaseAdam, NetworkError


class Adam(BaseAdam):
    """Оптимизатор Adam для CPU."""
    def __init__(self, count: int, alpha: float = 0.001, beta1: float = 0.9, 
                  beta2: float = 0.999, eps: float = 1e-8):
        """
        Инициализация оптимизатора Adam.
        
        Args:
            count: Количество параметров для оптимизации
            alpha: Скорость обучения
            beta1: Коэффициент экспоненциального затухания для оценок первого момента
            beta2: Коэффициент экспоненциального затухания для оценок второго момента
            eps: Малая константа для численной стабильности
        """
        super().__init__(count, alpha, beta1, beta2, eps)


class Network(BaseNetwork):
    """Нейронная сеть для CPU реализации."""
    def __init__(self, N: int, M: int, L: int, weight_type, input_type, output_type, 
                  in_bord: float, out_bord: float, weight_board: float):
        """
        Инициализация нейронной сети.
        
        Args:
            N: Размерность внутреннего слоя
            M: Размерность входного слоя
            L: Размер батча
            weight_type: Тип данных для весов
            input_type: Тип данных для входных данных
            output_type: Тип данных для выходных данных
            in_bord: Граница для входных значений
            out_bord: Граница для выходных значений
            weight_board: Граница для весов
        """
        super().__init__(N, M, L, weight_type, input_type, output_type, 
                        in_bord, out_bord, weight_board)
        
        # Инициализация матриц весов
        self.A_t = np.zeros((3, M, N), dtype=weight_type)  # Матрица весов первого слоя
        self.B_t = np.zeros((3, 1, N), dtype=weight_type)  # Вектор смещения первого слоя
        self.C_1_t = np.zeros((3, 1, N), dtype=weight_type)  # Веса для белых фигур
        self.C_2_t = np.zeros((3, 1, N), dtype=weight_type)  # Веса для черных фигур
        self.d = np.zeros((3, 1), dtype=weight_type)  # Финальное смещение
        self.coefs = (self.A_t, self.B_t, self.C_1_t, self.C_2_t, self.d)
        
        # Инициализация входных данных и меток
        self.X_1 = np.zeros((L, M), dtype=input_type)  # Матрица позиций
        self.X_2 = np.zeros((L, 1), dtype=input_type)  # Вектор цвета
        self.Y = np.zeros((L, 1), dtype=output_type)  # Целевые значения
        
        # Вспомогательные векторы
        self.L_1 = np.ones((L, 1), dtype=weight_type)  # Вектор единиц размера L
        self.N_1 = np.ones((N, 1), dtype=weight_type)  # Вектор единиц размера N
        
        # Промежуточные значения
        self.z = None  # Выход первого слоя
        self.t = None  # Выход второго слоя
        self.f = None  # Финальный выход
        self.e = None  # Ошибка

    def read_db(self, in_f: str) -> None:
        """
        Чтение обучающих данных из файла.
        
        Args:
            in_f: Путь к файлу с данными
        """
        try:
            if not isinstance(in_f, str) or not in_f:
                raise ValueError("Invalid input file path")
                
            with open(in_f, 'r') as f:
                for i, line in enumerate(f):
                    if i >= self.L:
                        break

                    note = loads(line)
                    if not all(k in note for k in ['count', 'color', 'fen']):
                        raise ValueError(f"Invalid JSON format at line {i+1}")
                        
                    self.Y[i] = note['count']  # Загрузка целевого значения
                    self.X_2[i] = 1 if note['color'] == "w" else 0  # Загрузка цвета
                    
                    # Преобразование FEN в бинарный вектор
                    try:
                        fen_int = int(note['fen'])
                        num_bytes = fen_int.to_bytes(96, byteorder='big')
                        num_array = np.unpackbits(np.frombuffer(num_bytes, dtype=np.uint8), bitorder='big')
                        self.X_1[i] = num_array[-self.M:]
                    except (ValueError, OverflowError) as e:
                        raise ValueError(f"Invalid FEN value at line {i+1}: {str(e)}")

        except FileNotFoundError:
            logging.error(f"Database file not found: {in_f}")
            raise
        except JSONDecodeError as e:
            logging.error(f"Invalid JSON in database: {str(e)}")
            raise NetworkError("Failed to parse database") from e
        except Exception as e:
            logging.error(f"Failed to read database: {str(e)}")
            raise NetworkError("Database reading failed") from e

    def read_coefs(self, coefs: str) -> None:
        """
        Загрузка коэффициентов сети из файла.
        
        Args:
            coefs: Путь к файлу с коэффициентами
        """
        try:
            if not isinstance(coefs, str) or not coefs:
                raise ValueError("Invalid coefficients file path")
                
            with open(coefs, 'r') as f:
                data = load(f)
                if not all(k in data for k in ['A', 'B', 'C_1', 'C_2', 'd']):
                    raise ValueError("Missing required coefficients in file")
                    
                # Загрузка весов из файла
                self.A_t[0] = np.array(data['A'])
                self.B_t[0] = np.array(data['B'])
                self.C_1_t[0] = np.array(data['C_1'])
                self.C_2_t[0] = np.array(data['C_2'])
                self.d[0] = np.array(data['d'])
        except FileNotFoundError:
            logging.error(f"Coefficients file not found: {coefs}")
            raise
        except Exception as e:
            logging.error(f"Failed to read coefficients: {str(e)}")
            raise NetworkError("Failed to load coefficients") from e

    def create_coefs(self) -> None:
        """Инициализация коэффициентов сети случайными значениями."""
        try:
            # Инициализация весов с использованием различных распределений
            self.A_t[0] = np.random.normal(0, np.sqrt(2.0 / (self.M + self.N)), size=(self.M, self.N))
            self.B_t[0] = np.random.normal(0, 0.10**(-3), (1, self.N))
            self.C_1_t[0] = np.random.uniform(-0.1, 0.1, size=(1, self.N))
            self.C_2_t[0] = np.random.uniform(-0.1, 0.1, size=(1, self.N))
            self.d[0] = np.random.normal(0, 0.10**(-3), (1,))
        except Exception as e:
            logging.error(f"Failed to create coefficients: {str(e)}")
            raise NetworkError("Coefficient initialization failed") from e

    def norm(self, *arrays) -> float:
        """
        Вычисление L2-нормы для набора массивов.
        
        Args:
            arrays: Массивы для вычисления нормы
            
        Returns:
            float: Значение нормы
        """
        try:
            return float(np.sqrt(sum(np.linalg.norm(array)**2 for array in arrays)))
        except Exception as e:
            logging.error(f"Failed to calculate norm: {str(e)}")
            raise NetworkError("Norm calculation failed") from e

    def Z(self) -> NDArray:
        """
        Вычисление выхода первого слоя сети.
        
        Returns:
            NDArray: Выход первого слоя
        """
        try:
            return np.clip(np.matmul(self.X_1, self.A_t[0]) + np.matmul(self.L_1, self.B_t[0]), 
                          -self.alpha, self.alpha)
        except Exception as e:
            logging.error(f"Failed to calculate Z: {str(e)}")
            raise NetworkError("Z calculation failed") from e

    def T(self) -> NDArray:
        """
        Вычисление выхода второго слоя сети.
        
        Returns:
            NDArray: Выход второго слоя
        """
        try:
            return np.matmul(self.X_2, self.C_1_t[0]) + np.matmul((1 - self.X_2), self.C_2_t[0])
        except Exception as e:
            logging.error(f"Failed to calculate T: {str(e)}")
            raise NetworkError("T calculation failed") from e

    def F(self) -> NDArray:
        """
        Вычисление финального выхода сети.
        
        Returns:
            NDArray: Финальный выход сети
        """
        try:
            self.t = self.T()
            self.z = self.Z()
            return np.clip(np.matmul(np.multiply(self.z, self.t), self.N_1)
                        + (self.d[0] * self.L_1), 
                        -self.beta, self.beta)
        except Exception as e:
            logging.error(f"Failed to calculate F: {str(e)}")
            raise NetworkError("F calculation failed") from e

    def E(self) -> None:
        """Вычисление ошибки сети."""
        try:
            self.f = self.F()
            self.e = np.matmul(
                np.multiply(self.Y - self.f, ((self.f >= -self.beta) & (self.f <= self.beta))),
                self.N_1.T)
        except Exception as e:
            logging.error(f"Failed to calculate E: {str(e)}")
            raise NetworkError("E calculation failed") from e

    def grad(self) -> None:
        """Вычисление градиентов для всех параметров сети."""
        try:
            self.E()
            # Маски для ограничения градиентов
            mask_Z = ((self.z >= -self.alpha) & (self.z <= self.alpha))
            mask_F = ((self.f >= -self.beta) & (self.f <= self.beta))

            # Вычисление градиентов для каждого параметра
            self.A_t[2] = self.X_1.T @ (self.e * self.t * mask_Z)
            self.B_t[2] = self.L_1.T @ (self.e * self.t * mask_Z)
            self.C_1_t[2] = self.X_2.T @ (self.e * self.z)
            self.C_2_t[2] = (1 - self.X_2).T @ (self.e * self.z)
            self.d[2] = self.L_1.T @ ((self.Y - self.f) * mask_F)

        except Exception as e:
            logging.error(f"Failed to calculate gradients: {str(e)}")
            raise NetworkError("Gradient calculation failed") from e

    def update(self, optim: BaseAdam) -> None:
        """
        Обновление весов сети с помощью оптимизатора Adam.
        
        Args:
            optim: Экземпляр оптимизатора Adam
        """
        try:
            if not isinstance(optim, BaseAdam):
                raise ValueError("Invalid optimizer type")
                
            optim.t += 1
            for i, coef in enumerate(self.coefs):
                # Инициализация моментов если необходимо
                if optim.m[i] is None:
                    optim.m[i] = np.zeros_like(coef[1])
                    optim.v[i] = np.zeros_like(coef[1])
                
                # Обновление моментов
                optim.m[i] = optim.beta1 * optim.m[i] + (1 - optim.beta1) * coef[2]
                optim.v[i] = optim.beta2 * optim.v[i] + (1 - optim.beta2) * np.multiply(coef[2], coef[2])
                
                # Коррекция смещения
                m_hat = optim.m[i] / (1 - optim.beta1 ** optim.t)
                v_hat = optim.v[i] / (1 - optim.beta2 ** optim.t)
                
                # Обновление параметров
                coef[1] += optim.alpha * m_hat / (np.sqrt(v_hat) + optim.eps)
                
        except Exception as e:
            logging.error(f"Failed to update weights: {str(e)}")
            raise NetworkError("Weight update failed") from e

    def upgrade(self) -> None:
        """Обновление весов с ограничением их значений."""
        try:
            for coef in self.coefs:
                coef[0] = np.clip(coef[1], -self.weight_board, self.weight_board)
        except Exception as e:
            logging.error(f"Failed to upgrade weights: {str(e)}")
            raise NetworkError("Weight upgrade failed") from e

    def result(self, coefs: str) -> None:
        """
        Сохранение результатов обучения в файл.
        
        Args:
            coefs: Путь к файлу для сохранения весов
        """
        try:
            with open(coefs, 'w') as f:
                dump({
                    "A": self.A_t[0].tolist(),
                    "B": self.B_t[0].tolist(),
                    "C_1": self.C_1_t[0].tolist(),
                    "C_2": self.C_2_t[0].tolist(),
                    "d": self.d[0].tolist()
                }, f, indent=4)
        except Exception as e:
            logging.error(f"Failed to save results: {str(e)}")
            raise NetworkError("Failed to save network weights") from e
