import matplotlib.pyplot as plt
import pandas as pd

df  = pd.read_csv('wavefunctions.csv')

plt.figure(figsize=(10, 6))

plt.plot(df['x'], df['potential_scaled'], 
         color='black', linestyle='--', linewidth=2, alpha=0.5, label='Potential V(x)')

plt.plot(df['x'], df['psi_0'], linewidth=2, label='$\psi_0$ (Ground State)')
plt.plot(df['x'], df['psi_1'], linewidth=2, label='$\psi_1$ (1st Excited)')
plt.plot(df['x'], df['psi_2'], linewidth=2, label='$\psi_2$ (2nd Excited)')

plt.title('Quantum Harmonic Oscillator: Wavefunctions', fontsize=16)
plt.xlabel('Position (x)', fontsize=12)
plt.ylabel('Amplitude ($\psi$)', fontsize=12)

plt.axhline(0, color='gray', linewidth=0.5)

plt.legend(loc='upper right')
plt.grid(True, alpha=0.3)

plt.show()
