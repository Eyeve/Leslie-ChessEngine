"""
Математический обзор:
Сеть реализует оптимизацию градиентного спуска для нейронной сети со следующей структурой:
- Входной слой (M=768 бинарных нейронов) представляет шахматную позицию
- Скрытый слой (N=1024 нейрона) с активацией ReLU, ограниченной в диапазоне [-6,6]
- Выходной слой (1 нейрон) с ограниченной активацией в диапазоне [-32000,32000]

Функция потерь E(θ) - это среднеквадратичная ошибка между предсказанными и целевыми значениями:
E(θ) = 1/2||Y - f(X,θ)||^2 

Где f(X,θ) - полная функция сети:
f(X,θ) = clip((clip(XA + B, -α, α) ⊙ C) + d, -β, β)

Градиенты вычисляются по правилу цепи и обновляются с помощью:
θ_new = θ - η∇E(θ) где η = c/||∇E(θ)||₂
"""

from datetime import datetime
from math import exp, log10, trunc
from network.cpu import *
from params import *

try:
    # Инициализация сети с заданными параметрами
    net = Network(N, M, L, weight_type, input_type, output_type, in_bord, out_bord, weight_board)
except Exception as e:
    print(f"Ошибка инициализации сети: {e}")
    exit(1)

try:
    # Загрузка обучающих данных и начальных весов
    net.read_db(in_f)
    print("Выберите режим (1 - загрузить коэффициенты, 2 - новые коэффициенты): ")
    mood = input()
    if mood == '2':
        net.create_coefs()
    elif mood == '1':
        net.read_coefs(coefs)
    else:
        print("Неверный режим")
        exit(1)
except FileNotFoundError:
    print("Ошибка: Файлы с обучающими данными или коэффициентами не найдены")
    exit(1)
except Exception as e:
    print(f"Ошибка загрузки данных: {e}")
    exit(1)

try:
    # Основной цикл обучения
    start = datetime.now()
    norm_old = 0
    er_old = 0
    er_del_old = 0
    optimizer = Adam(5, alpha)
    while True:
        try:
            # Вычисление градиентов функции потерь по всем параметрам
            a = datetime.now()
            net.grad()
            
            # Вычисление L2-нормы градиентов для размера шага
            norm = net.norm(*(coef[2] for coef in net.coefs))

            error = net.norm(net.e)/net.L
            er_del = error - er_old

            net.update(optimizer)
            
            print(f"итерация: {optimizer.t} \t изменение: {norm-norm_old:.4e} \t шаг: {net.norm(*(coef[1] - coef[0] for coef in net.coefs)):.4e} \t изм_ошибки: {er_del:.4e} \t ошибка: {error:.4e} \t alpha: {optimizer.alpha:.2e}\t время: {datetime.now() - a}с")
            
            # Ограничение весов для предотвращения переполнения
            net.upgrade()

            norm_old = norm
            er_old = error

            # Проверка критерия сходимости
            if norm < er:
                break
                
        except ZeroDivisionError:
            print("Ошибка: Деление на ноль при вычислении градиента")
            break
        except KeyboardInterrupt:
            print("\nОбучение прервано пользователем (Ctrl+C). Сохраняем модель...")
            net.result()
            print("\nЗаписано, нажмите Enter, чтобы прервать")
            if input() == '':
                break
            else:
                continue

        except Exception as e:
            print(f"Ошибка в итерации обучения: {e}")
            break

    # Сохранение параметров обученной сети
    net.result(coefs)
    print(datetime.now() - start)
    
except Exception as e:
    print(f"Ошибка сохранения результатов: {e}")
    exit(1)
