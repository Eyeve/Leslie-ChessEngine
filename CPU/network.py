import numpy as np
from numpy.typing import NDArray
from json import dump, load, loads, JSONDecodeError
import logging


# Пользовательское исключение для обработки ошибок нейронной сети
class NetworkError(Exception):
    pass


# Оптимизатор Adam для градиентного спуска
class Adam():
    def __init__(self, count, alpha=0.001, beta1=0.9, beta2=0.999, eps=1e-8):
        try:
            # Проверка корректности входных параметров
            if not isinstance(count, int) or count <= 0:
                raise ValueError("Count must be a positive integer")
            if not all(isinstance(x, (int, float)) for x in [alpha, beta1, beta2, eps]):
                raise ValueError("Parameters must be numeric values")
            if not (0 < alpha <= 1 and 0 < beta1 < 1 and 0 < beta2 < 1 and eps > 0):
                raise ValueError("Invalid parameter values")
                
            # Инициализация параметров оптимизатора
            self.alpha = alpha  # Скорость обучения
            self.beta1 = beta1  # Коэффициент затухания для первого момента
            self.beta2 = beta2  # Коэффициент затухания для второго момента
            self.steps = ()     # Шаги оптимизации
            self.eps = eps      # Малая константа для численной стабильности
            self.m = [None] * count  # Первый момент (среднее градиентов)
            self.v = [None] * count  # Второй момент (среднее квадратов градиентов)
            self.t = 0     # Счетчик итераций
        except Exception as e:
            logging.error(f"Failed to initialize Adam optimizer: {str(e)}")
            raise NetworkError("Adam initialization failed") from e


# Основной класс нейронной сети
class Network:
    def __init__(self, N: int, M: int, L: int, weight_type, input_type, output_type, in_bord: float, out_bord: float, weight_board: float):
        try:
            # Проверка корректности входных параметров
            if not all(isinstance(x, int) for x in [N, M, L]) or not all(x > 0 for x in [N, M, L]):
                raise ValueError("N, M, L must be positive integers")
            if not all(isinstance(x, (int, float)) for x in [in_bord, out_bord, weight_board]):
                raise ValueError("Boundaries must be numeric values")
            
            # N - размер внутреннего слоя
            # M - размер входного слоя (768 бит для представления позиции)
            # L - количество обучающих данных
            # in_bord (alpha) - граница отсечения внутреннего слоя (±6)
            # out_bord (beta) - граница отсечения выходного слоя (±32000)
            self.N = N
            self.M = M
            self.L = L
            self.alpha = in_bord
            self.beta = out_bord
            self.weight_board = weight_board
            
            # Веса и смещения сети хранятся в 3 состояниях: текущее[0], следующее[1], градиент[2]
            self.A_t = np.zeros((3, M, N), dtype=weight_type)  # Матрица весов внутреннего слоя
            self.B_t = np.zeros((3, 1, N), dtype=weight_type)  # Вектор смещения внутреннего слоя
            self.C_1_t = np.zeros((3, 1, N), dtype=weight_type)  # Веса выходного слоя для белых
            self.C_2_t = np.zeros((3, 1, N), dtype=weight_type)  # Веса выходного слоя для черных
            self.d = np.zeros((3, 1), dtype=weight_type)       # Смещение выходного слоя
            self.coefs = (self.A_t, self.B_t, self.C_1_t, self.C_2_t, self.d)
            
            # Матрицы обучающих данных
            self.X_1 = np.zeros((L, M), dtype=input_type)      # Векторы позиций
            self.X_2 = np.zeros((L, 1), dtype=input_type)      # Активные цвета
            self.Y = np.zeros((L, 1), dtype=output_type)       # Ожидаемые значения
            
            # Вспомогательные векторы единиц
            self.L_1 = np.ones((L, 1), dtype=weight_type)      # Вектор единиц L×1
            self.N_1 = np.ones((N, 1), dtype=weight_type)      # Вектор единиц N×1
        except Exception as e:
            logging.error(f"Failed to initialize network: {str(e)}")
            raise NetworkError("Network initialization failed") from e
    
    # Чтение обучающих данных из JSON файла
    def read_db(self, in_f: str) -> None:
        try:
            if not isinstance(in_f, str) or not in_f:
                raise ValueError("Invalid input file path")
                
            # Каждая строка содержит FEN позиции, активный цвет и ожидаемое значение
            with open(in_f, 'r') as f:
                for i, line in enumerate(f):
                    if i >= self.L:
                        break

                    note = loads(line)
                    if not all(k in note for k in ['count', 'color', 'fen']):
                        raise ValueError(f"Invalid JSON format at line {i+1}")
                        
                    self.Y[i] = note['count']  # Ожидаемое значение
                    self.X_2[i] = 1 if note['color'] == "w" else 0  # Активный цвет (1-белые, 0-черные)
                    # Преобразование FEN в битовый массив
                    num_bytes = note['fen'].to_bytes(96, byteorder='big')
                    num_array = np.unpackbits(np.frombuffer(num_bytes, dtype=np.uint8), bitorder='big')
                    self.X_1[i] = num_array[-self.M:]

        except FileNotFoundError:
            logging.error(f"Database file not found: {in_f}")
            raise
        except JSONDecodeError as e:
            logging.error(f"Invalid JSON in database: {str(e)}")
            raise NetworkError("Failed to parse database") from e
        except Exception as e:
            logging.error(f"Failed to read database: {str(e)}")
            raise NetworkError("Database reading failed") from e
            
    # Чтение весов сети из JSON файла
    def read_coefs(self, coefs: str) -> None:
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

    # Инициализация весов сети случайными значениями
    def create_coefs(self) -> None:
        try:
            # Инициализация по методу He для A_t
            self.A_t[0] = np.random.normal(0, np.sqrt(2.0 / (self.M + self.N)), size=(self.M, self.N)).astype(np.float16)
            self.B_t[0] = np.random.normal(0, 0.10**(-3), (1, self.N))
            self.C_1_t[0] = np.random.uniform(-0.1, 0.1, size=(1, self.N)).astype(np.float16)
            self.C_2_t[0] = np.random.uniform(-0.1, 0.1, size=(1, self.N)).astype(np.float16)
            self.d[0] = np.random.normal(0, 0.10**(-3), 1)
        except Exception as e:
            logging.error(f"Failed to create coefficients: {str(e)}")
            raise NetworkError("Coefficient initialization failed") from e

    # Вычисление L2 нормы градиентов для определения размера шага градиентного спуска
    def norm(self, *arrays) -> float:
        try:
            return sum(np.linalg.norm(array)**2 for array in arrays)**0.5
        except Exception as e:
            logging.error(f"Failed to calculate norm: {str(e)}")
            raise NetworkError("Norm calculation failed") from e
    
    # Вычисление выхода внутреннего слоя Z = clip(AX + B, -α, α)
    def Z(self) -> NDArray:
        try:
            return np.clip(np.matmul(self.X_1, self.A_t[0]) + np.matmul(self.L_1, self.B_t[0]), 
                           -self.alpha, self.alpha)
        except Exception as e:
            logging.error(f"Failed to calculate Z: {str(e)}")
            raise NetworkError("Z calculation failed") from e
    
    # Вычисление весов выхода на основе активного цвета: T = X₂C₁ᵀ + (1-X₂)C₂ᵀ
    def T(self) -> NDArray:
        try:
            return np.matmul(self.X_2, self.C_1_t[0]) + np.matmul((1 - self.X_2), self.C_2_t[0])
        except Exception as e:
            logging.error(f"Failed to calculate T: {str(e)}")
            raise NetworkError("T calculation failed") from e

    # Вычисление выхода сети F = clip((Z⊙T)1ₙ + d1ₗ, -β, β)
    def F(self) -> NDArray:
        try:
            self.t = self.T()
            self.z = self.Z()
            return np.clip(np.matmul(np.multiply(self.z, self.t), self.N_1)
                          + (self.d[0] * self.L_1), 
                            -self.beta, self.beta)
        except Exception as e:
            logging.error(f"Failed to calculate F: {str(e)}")
            raise NetworkError("F calculation failed") from e

    # Вычисление ошибки E = (Y-F)⊙M_F где M_F - маска отсечения выхода
    def E(self) -> NDArray:
        try:
            self.f = self.F()
            self.e = np.matmul(
                np.multiply(self.Y - self.f, ((self.f >= -self.beta) & (self.f <= self.beta))),
                self.N_1.T)
        except Exception as e:
            logging.error(f"Failed to calculate E: {str(e)}")
            raise NetworkError("E calculation failed") from e

    def grad(self) -> None:
        try:
            self.E()
            # Маски отсечения для внутреннего и выходного слоев
            mask_Z = ((self.z >= -self.alpha) & (self.z <= self.alpha))
            mask_F = ((self.f >= -self.beta) & (self.f <= self.beta))

            # Вычисление градиентов для каждого набора весов
            self.A_t[2] = self.X_1.T @ (self.e * self.t * mask_Z)
            self.B_t[2] = self.L_1.T @ (self.e * self.t * mask_Z)
            self.C_1_t[2] = self.X_2.T @ (self.e * self.z)
            self.C_2_t[2] = (1 - self.X_2).T @ (self.e * self.z)
            self.d[2] = self.L_1.T @ ((self.Y - self.f) * mask_F)

        except Exception as e:
            logging.error(f"Failed to calculate gradients: {str(e)}")
            raise NetworkError("Gradient calculation failed") from e
    
    # Обновление весов с помощью оптимизатора Adam
    def update(self, optim: Adam) -> None:
        try:
            if not isinstance(optim, Adam):
                raise ValueError("Invalid optimizer type")
                
            optim.t += 1
            for i, coef in enumerate(self.coefs):
                # Инициализация моментов, если это первая итерация
                if optim.m[i] is None:
                    optim.m[i] = np.zeros_like(coef[1])
                    optim.v[i] = np.zeros_like(coef[1])
                
                # Обновление первого и второго моментов
                optim.m[i] = (optim.beta1 * optim.m[i]) + (1 - optim.beta1) * coef[2]
                optim.v[i] = optim.beta2 * optim.v[i] + (1 - optim.beta2) * np.multiply(coef[2], coef[2])
                
                # Коррекция смещения моментов
                m_hat = optim.m[i] / (1 - optim.beta1 ** optim.t)
                v_hat = optim.v[i] / (1 - optim.beta2 ** optim.t)

                # Обновление параметров
                coef[1] += optim.alpha * m_hat / (np.sqrt(v_hat) + optim.eps)
        except Exception as e:
            logging.error(f"Failed to update weights: {str(e)}")
            raise NetworkError("Weight update failed") from e
    def upgrade(self) -> None:
        try:
            # Ограничение обновленных весов для предотвращения переполнения
            # Для каждого набора коэффициентов применяем функцию clip,
            # которая обрезает значения, выходящие за пределы [-weight_board, weight_board]
            for coef in self.coefs:
                coef[0] = np.clip(coef[1], -self.weight_board, self.weight_board)
        except Exception as e:
            logging.error(f"Failed to upgrade weights: {str(e)}")
            raise NetworkError("Weight upgrade failed") from e

    def result(self, coefs) -> None:
        try:
            # Сохранение обученных весов нейронной сети в JSON файл
            # Открываем файл result.json для записи
            # Преобразуем numpy массивы в списки Python для сериализации в JSON
            # Сохраняем веса A, B, C_1, C_2 и d с отступом в 4 пробела
            with open(coefs, 'w') as f:
                dump({"A": self.A_t[0].tolist(),
                        "B": self.B_t[0].tolist(),
                        "C_1": self.C_1_t[0].tolist(),
                        "C_2": self.C_2_t[0].tolist(),
                        "d": self.d[0].tolist()}, f, indent=4)
        except Exception as e:
            logging.error(f"Failed to save results: {str(e)}")
            raise NetworkError("Failed to save network weights") from e
