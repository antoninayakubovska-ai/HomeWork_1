import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.optimize import nnls

# 1. Дані з експерименту
try:
    df = pd.read_csv('results.csv')
except FileNotFoundError:
    print("Файл results.csv не знайдено! Спочатку запусти ./run_experiment")
    exit()

n = df['N'].values.astype(float)
G = df['G'].values.astype(float)          # кількість різних груп

# 2. Найповільніша операція - Op3 (перевіряємо за даними)
slowest = max(['Op1', 'Op2', 'Op3'], key=lambda op: df[op + '_Mean_us'].iloc[-1])
y = df[slowest + '_Mean_us'].values
sd = df[slowest + '_Std_us'].values
print("Найповільніша операція:", slowest)

# 3. Лінійна регресія T(n) = c1*f1(n) + c2*f2(n) + c3*f3(n)
#    Op3 = один прохід по G групах -> f1 = G(n), f2 = n (зростання таблиць у пам'яті), f3 = 1
#    nnls = найменші квадрати з умовою c >= 0 (час не може бути від'ємним)
A = np.column_stack([G, n, np.ones_like(n)])
c, _ = nnls(A, y)
pred = A @ c
r2 = 1 - ((y - pred) ** 2).sum() / ((y - y.mean()) ** 2).sum()
print("c1 (G) = %.3e, c2 (n) = %.3e, c3 (1) = %.3e, R^2 = %.4f" % (c[0], c[1], c[2], r2))

# 4. Графіки
fig, axs = plt.subplots(2, 2, figsize=(14, 9))
fig.suptitle('Продуктивність варіанту C (N від 100 до 100 000)', fontsize=15, fontweight='bold')

# 4.1 Регресія для найповільнішої операції (з середнім і відхиленням)
ax = axs[0, 0]
ax.errorbar(n, y, yerr=sd, fmt='o', color='#2a6fdb', ecolor='gray', capsize=4,
            label='Експеримент (середнє ± σ)')
xs = np.linspace(n.min(), n.max(), 400)
Gs = np.interp(xs, n, G)
ax.plot(xs, c[0] * Gs + c[1] * xs + c[2], '--k',
        label='T(n) = %.2e·G(n) + %.2e·n + %.2e\nR² = %.4f' % (c[0], c[1], c[2], r2))
ax.set_title('Операція 3 (найповільніша): регресія')
ax.set_xlabel('Кількість записів (N)'); ax.set_ylabel('Час, мкс')
ax.legend(fontsize=9); ax.grid(True, linestyle='--', alpha=0.6)

# 4.2 Op1
ax = axs[0, 1]
ax.errorbar(n, df['Op1_Mean_us'], yerr=df['Op1_Std_us'], fmt='o-', color='#ff7f0e',
            ecolor='gray', capsize=4)
ax.set_title('Операція 1: Top-100'); ax.set_xlabel('Кількість записів (N)'); ax.set_ylabel('Час, мкс')
ax.grid(True, linestyle='--', alpha=0.6)

# 4.3 Op2
ax = axs[1, 0]
ax.errorbar(n, df['Op2_Mean_us'], yerr=df['Op2_Std_us'], fmt='o-', color='#2ca02c',
            ecolor='gray', capsize=4)
ax.set_title('Операція 2: оновлення рейтингу'); ax.set_xlabel('Кількість записів (N)'); ax.set_ylabel('Час, мкс')
ax.grid(True, linestyle='--', alpha=0.6)

# 4.4 Пам'ять (виміряна)
ax = axs[1, 1]
ax.plot(n, df['Memory_MB'], 'o-', color='#d62728')
ax.set_title("Пам'ять (виміряна, mallinfo2)"); ax.set_xlabel('Кількість записів (N)'); ax.set_ylabel('МБ')
ax.grid(True, linestyle='--', alpha=0.6)

plt.tight_layout()
plt.subplots_adjust(top=0.92)
plt.savefig('performance_graphs.png', dpi=200, bbox_inches='tight')
print("Графіки збережено у performance_graphs.png")
