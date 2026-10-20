import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# 1. Зчитуємо дані з CSV
try:
    df = pd.read_csv('results.csv')
except FileNotFoundError:
    print("Файл results.csv не знайдено! Перевірте, чи успішно завершився експеримент C++.")
    exit()

N = df['N']

# 2. Налаштовуємо загальний вигляд полотна
fig, axs = plt.subplots(2, 2, figsize=(15, 10))
fig.suptitle('Аналіз продуктивності Варіанту C (Масштабування до 100k)', fontsize=16, fontweight='bold')

# Універсальна функція для малювання одного графіка з регресією
def plot_with_regression(ax, x, y, yerr, title, ylabel, color):
    # Малюємо експериментальні точки з вусами похибки
    ax.errorbar(x, y, yerr=yerr, fmt='o', color=color, ecolor='gray', capsize=4, label='Експериментальні дані', markersize=5)

    # Розрахунок лінійної регресії (y = ax + b) поліномом 1-го степеня
    coef = np.polyfit(x, y, 1)
    poly1d_fn = np.poly1d(coef)

    # Малюємо лінію тренду
    trend_label = f'Лінійна регресія:\nT(n) = {coef[0]:.2e} * n + {coef[1]:.2e}'
    ax.plot(x, poly1d_fn(x), '--k', alpha=0.8, label=trend_label)

    # Оформлення
    ax.set_title(title, fontsize=12)
    ax.set_xlabel('Кількість записів (N)', fontsize=10)
    ax.set_ylabel(ylabel, fontsize=10)
    ax.legend(fontsize=9)
    ax.grid(True, linestyle='--', alpha=0.6)

# 3. Будуємо 4 графіки
# Графік 1: Операція 1 (Top-100)
plot_with_regression(axs[0, 0], N, df['Op1_Mean_us'], df['Op1_Std_us'],
                     'Операція 1: Отримання Top-100', 'Час виконання (мкс)', '#1f77b4')

# Графік 2: Операція 2 (Оновлення рейтингу)
plot_with_regression(axs[0, 1], N, df['Op2_Mean_us'], df['Op2_Std_us'],
                     'Операція 2: Оновлення рейтингу за email', 'Час виконання (мкс)', '#ff7f0e')

# Графік 3: Операція 3 (Найкраща група)
plot_with_regression(axs[1, 0], N, df['Op3_Mean_us'], df['Op3_Std_us'],
                     'Операція 3: Пошук найкращої групи', 'Час виконання (мкс)', '#2ca02c')

# Графік 4: Використання пам'яті
plot_with_regression(axs[1, 1], N, df['Memory_MB'], np.zeros(len(N)),
                     'Використання пам\'яті контейнерами', 'Об\'єм (МБ)', '#d62728')

# Зберігаємо результат
plt.tight_layout()
plt.subplots_adjust(top=0.92) # Трохи відступаємо зверху для головного заголовка
plt.savefig('performance_graphs.png', dpi=300, bbox_inches='tight')
print("Графіки успішно побудовано та збережено у файл 'performance_graphs.png'!")
