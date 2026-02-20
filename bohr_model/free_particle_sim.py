import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# --- Settings ---
# Make sure this matches the filename in your C code (wavefct.c or wavefct.csv)
FILENAME = 'wavefct.c' 
R = 1.0  # Radius of the circle

def plot_figure_6():
    try:
        # 1. Read the Data
        # skipinitialspace=True fixes the spaces in your header like ", image_psi"
        df = pd.read_csv(FILENAME, skipinitialspace=True)
        
        # 2. Extract the first time snapshot (t=0)
        # We only want to plot the static shape first
        t0 = df['t'].iloc[0]
        data = df[df['t'] == t0].copy()
        
        # 3. Convert 'x' (arc length) to geometric coordinates
        # Angle theta = x / R
        # X = R * cos(theta)
        # Y = R * sin(theta)
        data['theta'] = data['x'] / R
        data['X_geo'] = R * np.cos(data['theta'])
        data['Y_geo'] = R * np.sin(data['theta'])
        
        # The 'Height' of the wave is the Real part of Psi
        data['Z_val'] = data['real_psi']

        # 4. Setup the 3D Plot
        fig = plt.figure(figsize=(12, 10))
        ax = fig.add_subplot(111, projection='3d')

        # -- Plot A: The Reference Circle (Gray Ring) --
        # This shows where the particle "lives" (Psi = 0)
        ax.plot(data['X_geo'], data['Y_geo'], np.zeros_like(data['Z_val']), 
                color='gray', linestyle='--', alpha=0.5, label='Circle (R=1)')

        # -- Plot B: The Wavefunction (Blue Curve) --
        # This shows the value of Psi at every point on the circle
        ax.plot(data['X_geo'], data['Y_geo'], data['Z_val'], 
                color='blue', linewidth=2, label='Re($\psi$)')

        # -- Plot C: Vertical "Stems" (The 3D effect) --
        # Connects the blue wave to the gray circle so you can see the height
        # We assume 'n' is roughly visible, plotting every 10th point to avoid clutter
        step = 10 
        for i in range(0, len(data), step):
            row = data.iloc[i]
            ax.plot([row['X_geo'], row['X_geo']],   # X line
                    [row['Y_geo'], row['Y_geo']],   # Y line
                    [0, row['Z_val']],              # Z line (from 0 to Psi)
                    color='blue', alpha=0.2)

        # 5. Make it look nice
        ax.set_title(f"Particle on a Circle (Time t={t0:.2f})", fontsize=15)
        ax.set_xlabel('X')
        ax.set_ylabel('Y')
        ax.set_zlabel('Re($\psi$)')
        
        # Set axis limits to keep the aspect ratio roughly circular
        limit = 1.5 * R
        ax.set_xlim(-limit, limit)
        ax.set_ylim(-limit, limit)
        ax.set_zlim(-1.5, 1.5)
        
        ax.legend()
        
        # Initial view angle (Elev=30 deg, Azim=45 deg)
        ax.view_init(elev=30, azim=45)
        
        plt.show()
        print("Plot generated successfully!")

    except FileNotFoundError:
        print(f"Error: Could not find '{FILENAME}'. Make sure the C code ran and saved the file.")
    except KeyError as e:
        print(f"Error: Column not found. Check your CSV header. Missing: {e}")

if __name__ == "__main__":
    plot_figure_6()
