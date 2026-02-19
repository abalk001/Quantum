import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from matplotlib.animation import FuncAnimation
import sys
import os

VIEW_SCOPE = 6e-11      # zoom around atom (meters)
PLAY_SPEED = 1
BOHR_RADIUS = 5.29e-11  # meters (actual Bohr radius)
SPEED_LIGHT = 2.998e8   # m/s


if not os.path.exists("data.csv"):
    print("Error: data.csv not found. Run main.c first!")
    sys.exit(1)

data = pd.read_csv("data.csv")
total_frames = len(data)

spectrum_data = None
if os.path.exists("spectrum.csv"):
    spectrum_data = pd.read_csv("spectrum.csv")


fig = plt.figure(figsize=(16, 8))


ax_orbit = plt.subplot(1, 2, 1)
ax_orbit.set_aspect('equal')
ax_orbit.grid(True, linestyle='--', alpha=0.5)
ax_orbit.set_xlabel('x (m)', fontsize=10)
ax_orbit.set_ylabel('y (m)', fontsize=10)

electron, = ax_orbit.plot([], [], 'bo', markersize=8, label="Electron", zorder=5)
orbit_line, = ax_orbit.plot([], [], 'r--', linewidth=1, alpha=0.5, label="Orbit")
nucleus, = ax_orbit.plot([0], [0], 'ro', markersize=12, label="Proton", zorder=10)

title_text = ax_orbit.set_title("Bohr Model (n=1)")

stats_text = ax_orbit.text(
    0.02, 0.98, '',
    transform=ax_orbit.transAxes,
    fontsize=9,
    verticalalignment='top',
    bbox=dict(boxstyle='round', facecolor='white', alpha=0.9),
    fontfamily='monospace'
)

ax_spectrum = plt.subplot(1, 2, 2)
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
    

    from matplotlib.patches import Patch
    legend_elements = [Patch(facecolor=color, label=series) 
                      for series, color in series_colors.items()]
    ax_spectrum.legend(handles=legend_elements, loc='lower right', fontsize=9)
    

    ax_spectrum.axvspan(380, 750, alpha=0.1, color='yellow', label='Visible')
    ax_spectrum.text(565, -13, 'Visible', fontsize=10, ha='center', 
                    bbox=dict(boxstyle='round', facecolor='yellow', alpha=0.3))

theta_vals = np.linspace(0, 2*np.pi, 400)
if 'radius' in data.columns:
    orbit_radius = data['radius'].iloc[0]
else:
    orbit_radius = BOHR_RADIUS
    
orbit_x = orbit_radius * np.cos(theta_vals)
orbit_y = orbit_radius * np.sin(theta_vals)
orbit_line.set_data(orbit_x, orbit_y)


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

    elif event.key == 'e':
        sys.exit()

fig.canvas.mpl_connect('key_press_event', on_key)


def init():
    return electron, stats_text

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
    
    ax_orbit.set_title(f"Bohr Hydrogen Atom - {status} (n={n}, Frame {idx}/{total_frames})", 
                      fontsize=12, fontweight='bold')


    view_range = radius * 1.5
    ax_orbit.set_xlim(-view_range, view_range)
    ax_orbit.set_ylim(-view_range, view_range)

    electron.set_data([x], [y])

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

    return electron, stats_text

ani = FuncAnimation(
    fig,
    update,
    frames=range(total_frames),
    init_func=init,
    blit=False,
    interval=20
)

ax_orbit.legend(loc='upper right', fontsize=9)
plt.tight_layout()
plt.show()

