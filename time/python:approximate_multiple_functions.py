import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
import re

import warnings
warnings.filterwarnings('ignore', category=RuntimeWarning)

# Parse data from the file
def parse_data_file(filename):
    x_values = []  # Для значений Iter
    y_values = []  # Для значений er
    
    with open(filename, 'r', encoding='utf-8') as file:
        for line in file:
            # Ищем значения Iter и er в строке лога
            iter_match = re.search(r'Iter:\s*(\d+)', line)
            er_match = re.search(r'er:\s*(\S+)', line)
            
            if iter_match and er_match:
                iter_val = int(iter_match.group(1))
                er_val = float(er_match.group(1))
                x_values.append(iter_val)
                y_values.append(er_val)
    
    return np.array(x_values), np.array(y_values)

# Загружаем данные из файла
try:
    x, y = parse_data_file('loggi.txt')
    print(f"Loaded {len(x)} data points from file 'и'")
except Exception as e:
    print(f"Error loading data from file: {e}")
    # Используем данные по умолчанию, если файл не удалось прочитать
    x = np.array([i for i in range(90, 197)])
    y = np.array([
        2.5436e+03, 2.5436e+03, 2.5435e+03, 2.5435e+03, 2.5435e+03,
        2.5435e+03, 2.5435e+03, 2.5435e+03, 2.5435e+03, 2.5435e+03,
        2.5435e+03, 2.5435e+03, 2.5434e+03, 2.5434e+03, 2.5434e+03,
        2.5434e+03, 2.5433e+03, 2.5433e+03, 2.5433e+03, 2.5432e+03,
        2.5432e+03, 2.5432e+03, 2.5431e+03, 2.5431e+03, 2.5430e+03,
        2.5430e+03, 2.5429e+03, 2.5429e+03, 2.5428e+03, 2.5427e+03,
        2.5426e+03, 2.5425e+03, 2.5425e+03, 2.5424e+03, 2.5422e+03,
        2.5422e+03, 2.5420e+03, 2.5419e+03, 2.5418e+03, 2.5416e+03,
        2.5415e+03, 2.5413e+03, 2.5412e+03, 2.5410e+03, 2.5408e+03,
        2.5407e+03, 2.5404e+03, 2.5403e+03, 2.5401e+03, 2.5398e+03,
        2.5396e+03, 2.5394e+03, 2.5391e+03, 2.5389e+03, 2.5387e+03,
        2.5384e+03, 2.5382e+03, 2.5379e+03, 2.5376e+03, 2.5373e+03,
        2.5370e+03, 2.5368e+03, 2.5364e+03, 2.5361e+03, 2.5358e+03,
        2.5355e+03, 2.5352e+03, 2.5349e+03, 2.5345e+03, 2.5342e+03,
        2.5338e+03, 2.5335e+03, 2.5331e+03, 2.5327e+03, 2.5324e+03,
        2.5320e+03, 2.5316e+03, 2.5312e+03, 2.5308e+03, 2.5304e+03,
        2.5300e+03, 2.5296e+03, 2.5292e+03, 2.5287e+03, 2.5283e+03,
        2.5279e+03, 2.5274e+03, 2.5269e+03, 2.5264e+03, 2.5259e+03,
        2.5254e+03, 2.5249e+03, 2.5244e+03, 2.5238e+03, 2.5233e+03,
        2.5227e+03, 2.5221e+03, 2.5215e+03, 2.5209e+03, 2.5202e+03,
        2.5195e+03, 2.5189e+03, 2.5182e+03, 2.5174e+03, 2.5167e+03,
        2.5159e+03, 2.5152e+03
    ])
    print("Using default data")

# Define the functions for fitting
def linear(x, a, b):
    return a * x + b

def quadratic(x, a, b, c):
    return a * x**2 + b * x + c

def cubic(x, a, b, c, d):
    return a * x**3 + b * x**2 + c * x + d

def exponential(x, a, b):
    return a * np.exp(b * x)

def logarithmic(x, a, b):
    return a * np.log(b * x)

# Fit the models with adjusted parameters
params_linear, _ = curve_fit(linear, x, y)
params_quadratic, _ = curve_fit(quadratic, x, y)
params_cubic, _ = curve_fit(cubic, x, y)

# Adjusted initial guess for exponential fit
params_exponential, _ = curve_fit(exponential, x, y, p0=(1, 0.01))

# Adjusted initial guess and increased maxfev for logarithmic fit
params_logarithmic, _ = curve_fit(logarithmic, x, y, p0=(1, 1), maxfev=10000)

# Generate fitted values
y_linear = linear(x, *params_linear)
y_quadratic = quadratic(x, *params_quadratic)
y_cubic = cubic(x, *params_cubic)
y_exponential = exponential(x, *params_exponential)
y_logarithmic = logarithmic(x, *params_logarithmic)

# Print the coefficients of each function
print("\nLinear function coefficients (a*x + b):")
print(f"a = {params_linear[0]:.6e}")
print(f"b = {params_linear[1]:.6e}")
print("\nQuadratic function coefficients (a*x^2 + b*x + c):")
print(f"a = {params_quadratic[0]:.6e}")
print(f"b = {params_quadratic[1]:.6e}")
print(f"c = {params_quadratic[2]:.6e}")
print("\nCubic function coefficients (a*x^3 + b*x^2 + c*x + d):")
print(f"a = {params_cubic[0]:.6e}")
print(f"b = {params_cubic[1]:.6e}")
print(f"c = {params_cubic[2]:.6e}")
print(f"d = {params_cubic[3]:.6e}")
print("\nExponential function coefficients (a*exp(b*x)):")
print(f"a = {params_exponential[0]:.6e}")
print(f"b = {params_exponential[1]:.6e}")
print("\nLogarithmic function coefficients (a*log(b*x)):")
print(f"a = {params_logarithmic[0]:.6e}")
print(f"b = {params_logarithmic[1]:.6e}")

# Calculate R-squared for each model to determine goodness of fit
def r_squared(y_true, y_pred):
    ss_total = np.sum((y_true - np.mean(y_true))**2)
    ss_residual = np.sum((y_true - y_pred)**2)
    return 1 - (ss_residual / ss_total)

r2_linear = r_squared(y, y_linear)
r2_quadratic = r_squared(y, y_quadratic)
r2_cubic = r_squared(y, y_cubic)
r2_exponential = r_squared(y, y_exponential)
r2_logarithmic = r_squared(y, y_logarithmic)

print("\nR-squared values (higher is better, 1.0 is perfect fit):")
print(f"Linear: {r2_linear:.6f}")
print(f"Quadratic: {r2_quadratic:.6f}")
print(f"Cubic: {r2_cubic:.6f}")
print(f"Exponential: {r2_exponential:.6f}")
print(f"Logarithmic: {r2_logarithmic:.6f}")

# Plotting
plt.figure(figsize=(12, 8))
plt.scatter(x, y, color='blue', label='Original Data', s=10)
plt.plot(x, y_linear, color='red', label=f'Linear Fit (R² = {r2_linear:.4f})')
plt.plot(x, y_quadratic, color='green', label=f'Quadratic Fit (R² = {r2_quadratic:.4f})')
plt.plot(x, y_cubic, color='orange', label=f'Cubic Fit (R² = {r2_cubic:.4f})')
plt.plot(x, y_exponential, color='purple', label=f'Exponential Fit (R² = {r2_exponential:.4f})')
plt.plot(x, y_logarithmic, color='brown', label=f'Logarithmic Fit (R² = {r2_logarithmic:.4f})')

# Save the figure instead of showing it
plt.xlabel('Iteration')
plt.ylabel('Error')
plt.title('Function Approximations of Error vs Iteration')
plt.legend()
plt.grid()
plt.savefig('function_approximations.png')  # Save the plot as a PNG file

# Вывод данных в формате, удобном для копирования
print("\nData in format suitable for copying:")
print("x = np.array([", end="")
print(", ".join(map(str, x)), end="")
print("])")
print("y = np.array([")
for i in range(0, len(y), 5):
    if i + 5 < len(y):
        print("    " + ", ".join([f"{val:.4e}" for val in y[i:i+5]]) + ",")
    else:
        print("    " + ", ".join([f"{val:.4e}" for val in y[i:]]))
print("])")
