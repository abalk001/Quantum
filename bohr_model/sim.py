import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from matplotlib.animation import FuncAnimation
from matplotlib.patches import Patch
import sys
import os

# --- Configuration ---
VIEW_SCOPE = 6e-11      # zoom around atom (meters)
PLAY_SPEED = 1
BOHR_RADIUS = 5.29e-11  # meters (actual Bohr radius)
SPEED_LIGHT = 2.998e8   # m/s

# --- Load Data ---
if not os.path.exists("data.csv"):
    print("Error: data.csv not found. Run main.c first!")
    sys.exit(1)

data = pd.read_csv("data.csv")
total_frames = len(data)

spectrum_data = None
if os.path.exists("spectrum.csv"):
    spectrum_data = pd.read_csv("spectrum.csv")

# --- Setup Figure ---
fig = plt.figure(figsize=(16, 8))

# --- LEFT SUBPLOT (3D Orbit & Wavefunction) ---
ax_orbit = fig.add_subplot(1, 2, 1, projection='3d')
ax_orbit.set_box_aspect((1, 1, 0.6))  # Flatten the Z-axis slightly for aesthetics
ax_orbit.grid(True, linestyle='--', alpha=0.5)
ax_orbit.set_xlabel('x (m)', fontsize=10)
ax_orbit.set_ylabel('y (m)', fontsize=10)
ax_orbit.set_zlabel(r'Re($\psi$) Amplitude', fontsize=10)

# Initialize 3D plot elements
electron, = ax_orbit.plot([], [], [], 'bo', markersize=8, label="Electron")
orbit_line, = ax_orbit.plot([], [], [], 'r--', linewidth=1, alpha=0.5, label="Orbit")
nucleus, = ax_orbit.plot([0], [0], [0], 'ro', markersize=12, label="Proton")
wave_line, = ax_orbit.plot([], [], [], color='blue', linewidth=2, label=r'de Broglie Wave')

# Initialize vertical stems for 3D effect
num_stems = 36
stems = [ax_orbit.plot([], [], [], color='blue', alpha=0.15)[0] for _ in range(num_stems)]

# Adjust the initial viewing angle
ax_orbit.view_init(elev=30, azim=45)

# 3D axes do not support transAxes well, so we place the text on the Figure directly
stats_text = fig.text(
    0.02, 0.95, '',
    fontsize=9,
    verticalalignment='top',
    bbox=dict(boxstyle='round', facecolor='white', alpha=0.9),
    fontfamily='monospace'
)

# --- RIGHT SUBPLOT (Spectrum) ---
ax_spectrum = fig.add_subplot(1, 2, 2)
ax_spectrum.set_xlim(0, 2000)
ax_spectrum.set_ylim(-14, 0)
ax_spectrum.set_xlabel('Wavelength (nm)', fontsize=12)
ax_spectrum.set_ylabel('Energy (eV)', fontsize=12)
ax_spectrum.set_title('Hydrogen Emission Spectrum', fontsize=14, fontweight='bold')
ax_spectrum.grid(True, linestyle='--', alpha=0.3)

energy_levels = [-13.6, -3.4, -1.51, -0.85, -0.54]
level_labels = ['n=1', 'n=2', 'n=3', 'n=4', 'n=5']
for i, (energy, label) in enumerate(zip(energy_levels, level_labels)):
    ax_spectrum.axhline(y=energy, color='gray', linestyle='-', linewidth=1, alpha=0.5)
    ax_spectrum.text(1950, energy, label, fontsize=9, va='center')

if spectrum_data is not None:
    series_colors = {
        'Lyman': '#8B00FF',   # Violet (UV)
        'Balmer': '#FF0000',  # Red (Visible)
        'Paschen': '#FF8C00', # Orange (IR)
        'Brackett': '#8B4513' # Brown (far IR)
    }
    
    for idx, row in spectrum_data.iterrows():
        wavelength = row['wavelength_nm']
        energy_photon = row['energy_eV']
        series = row['series']
        n1, n2 = int(row['n1']), int(row['n2'])
        
        color = series_colors.get(series, 'black')
        e1 = energy_levels[n1-1]
        e2 = energy_levels[n2-1] if n2 <= 5 else -13.6/(n2*n2)
        
        ax_spectrum.vlines(wavelength, e1, e2, colors=color, linewidth=2, alpha=0.7)
        ax_spectrum.plot(wavelength, (e1+e2)/2, 'o', color=color, markersize=4)
    
    legend_elements = [Patch(facecolor=color, label=series) 
                       for series, color in series_colors.items()]
    ax_spectrum.legend(handles=legend_elements, loc='lower right', fontsize=9)
    
    ax_spectrum.axvspan(380, 750, alpha=0.1, color='yellow', label='Visible')
    ax_spectrum.text(565, -13, 'Visible', fontsize=10, ha='center', 
                     bbox=dict(boxstyle='round', facecolor='yellow', alpha=0.3))

# --- Animation Controls ---
current_frame = 0
is_paused = False

def on_key(event):
    global current_frame, is_paused
    if event.key == ' ':
        is_paused = not is_paused
    elif event.key == 'right':
        current_frame = min(current_frame + 1, total_frames - 1)
        is_paused = True
    elif event.key == 'left':
        current_frame = max(current_frame - 1, 0)
        is_paused = True
    elif event.key == 'r':
        current_frame = 0
        is_paused = True
    elif event.key == 's':
        plt.savefig("figue1.png")
    elif event.key == 'e':
        sys.exit()

fig.canvas.mpl_connect('key_press_event', on_key)

# --- Animation Logic ---
def init():
    electron.set_data([], [])
    electron.set_3d_properties([])
    orbit_line.set_data([], [])
    orbit_line.set_3d_properties([])
    wave_line.set_data([], [])
    wave_line.set_3d_properties([])
    for stem in stems:
        stem.set_data([], [])
        stem.set_3d_properties([])
    stats_text.set_text("")
    return []

def update(frame):
    global current_frame
    if not is_paused:
        current_frame += PLAY_SPEED
        if current_frame >= total_frames:
            current_frame = total_frames - 1

    idx = int(current_frame)
    status = "PAUSED" if is_paused else "RUNNING"
    
    t = data["time"].iloc[idx]
    x = data["x"].iloc[idx]
    y = data["y"].iloc[idx]
    
    if 'n' in data.columns:
        n = int(data["n"].iloc[idx])
        radius = data["radius"].iloc[idx]
        velocity = data["velocity"].iloc[idx]
        energy = data["energy"].iloc[idx]
    else:
        n = 1
        x = x * BOHR_RADIUS
        y = y * BOHR_RADIUS
        radius = BOHR_RADIUS
        velocity = 2.19e6
        energy = -13.6
        
    ax_orbit.set_title(f"Bohr Atom & de Broglie Wave - {status}\n(n={n}, Frame {idx}/{total_frames})", 
                       fontsize=12, fontweight='bold')

    # Update viewport scale
    view_range = radius * 1.5
    ax_orbit.set_xlim(-view_range, view_range)
    ax_orbit.set_ylim(-view_range, view_range)
    ax_orbit.set_zlim(-view_range * 0.5, view_range * 0.5)

    # 1. Update 3D Electron
    electron.set_data([x], [y])
    electron.set_3d_properties([0])
    
    # 2. Update Orbit Circle
    theta_vals = np.linspace(0, 2*np.pi, 200)
    orbit_x = radius * np.cos(theta_vals)
    orbit_y = radius * np.sin(theta_vals)
    orbit_line.set_data(orbit_x, orbit_y)
    orbit_line.set_3d_properties(np.zeros_like(orbit_x))
    
    # 3. Calculate and Update Wavefunction
    amp = radius * 0.3  # Scale amplitude based on radius for visibility
    phi_e = np.arctan2(y, x) # Angle of the electron
    
    # Synchronize the wave so a crest always perfectly tracks the electron!
    Z_w = amp * np.cos(n * (theta_vals - phi_e))
    
    wave_line.set_data(orbit_x, orbit_y)
    wave_line.set_3d_properties(Z_w)
    
    # 4. Update Stems
    step = len(theta_vals) // num_stems
    for i, stem in enumerate(stems):
        s_idx = (i * step) % len(theta_vals)
        stem.set_data([orbit_x[s_idx], orbit_x[s_idx]], [orbit_y[s_idx], orbit_y[s_idx]])
        stem.set_3d_properties([0, Z_w[s_idx]])

    # Update text stats
    r = np.sqrt(x**2 + y**2)
    omega = velocity / radius if radius > 0 else 0
    period = 2 * np.pi / omega if omega > 0 else 0
    frequency = 1 / period if period > 0 else 0

    status_str = (
        f"CONTROLS: [Space] Pause  [<->] Step  [R] Reset\n"
        f"───────────────────────────────────────────\n"
        f"Level     : n = {n}\n"
        f"Time      : {t:>10.3e} s\n"
        f"Radius    : {r:>10.3e} m\n"
        f"Velocity  : {velocity:>10.3e} m/s\n"
        f"Energy    : {energy:>10.4f} eV\n"
        f"Period    : {period:>10.3e} s\n"
        f"Frequency : {frequency:>10.3e} Hz"
    )
    stats_text.set_text(status_str)

    return []

ani = FuncAnimation(
    fig, update, frames=range(total_frames),
    init_func=init, blit=False, interval=20
)

ax_orbit.legend(loc='upper right', fontsize=9)
plt.tight_layout()
plt.show()
