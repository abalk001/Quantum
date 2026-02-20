import matplotlib.pyplot as plt 
import numpy as np 
import pandas as pd 

data = pd.read_csv("potentiel.csv")


plt.plot(data["x"], data["psi(x)"], color = 'b', alpha=0.8)
plt.xlabel("x")
plt.ylabel("$psi(x)$")
plt.grid(True, linestyle='--', alpha=0.5)
plt.title("The wavefunction for an infinite potential well")
plt.axline((0, 0), slope=0, color='black', linestyle='--')
plt.xlim(0,1)
plt.show()
