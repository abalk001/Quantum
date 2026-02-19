import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from matplotlib.animation import FuncAnimation
import sys

# ==========================
# CONFIG
# ==========================
VIEW_SCOPE = 6e-11      # zoom around atom (meters)
PLAY_SPEED = 1
BOHR_RADIUS = 5.29e-11  # meters (actual Bohr radius)

# ==========================
# LOAD DATA
# ==========================
data = pd.read_csv("data.csv")
total_frames = len(data)

# ==========================
# FIGURE SETUP
# ==========================
fig, ax = plt.subplots(figsize=(8, 8))
ax.set_aspect('equal')
ax.grid(True, linestyle='--', alpha=0.5)

electron, = ax.plot([], [], 'bo', markersize=6, label="Electron")
orbit_line, = ax.plot([], [], 'r--', linewidth=1, alpha=0.5)
nucleus, = ax.plot([0], [0], 'ro', markersize=10, label="Proton")

title_text = ax.set_title("Bohr Model (Paused)")

stats_text = ax.text(
    0.02, 0.95, '',
    transform=ax.transAxes,
    fontsize=10,
    verticalalignment='top',
    bbox=dict(boxstyle='round', facecolor='white', alpha=0.9),
    fontfamily='monospace'
)

# Precompute orbit circle
theta_vals = np.linspace(0, 2*np.pi, 400)
orbit_x = BOHR_RADIUS * np.cos(theta_vals)
orbit_y = BOHR_RADIUS * np.sin(theta_vals)

orbit_line.set_data(orbit_x, orbit_y)

# ==========================
# CONTROLS
# ==========================
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

# ==========================
# ANIMATION
# ==========================
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
    ax.set_title(f"Bohr Hydrogen Atom - {status} (Frame {idx}/{total_frames})")

    t = data["time"].iloc[idx]
    x = data["x"].iloc[idx] * BOHR_RADIUS  # Scale to Bohr radius
    y = data["y"].iloc[idx] * BOHR_RADIUS  # Scale to Bohr radius

    # Centered fixed view on nucleus
    ax.set_xlim(-VIEW_SCOPE, VIEW_SCOPE)
    ax.set_ylim(-VIEW_SCOPE, VIEW_SCOPE)

    electron.set_data([x], [y])

    # Physics stats
    r = np.sqrt(x**2 + y**2)
    omega = 2.19e6 / BOHR_RADIUS
    speed = omega * BOHR_RADIUS

    status_str = (
        f"CONTROLS: [Space] Pause  [<->] Step  [R] Reset\n"
        f"---------------------------------------------\n"
        f"Time : {t:>10.3e} s\n"
        f"Radius : {r:>10.3e} m\n"
        f"Speed  : {speed:>10.3e} m/s\n"
        f"Level  : n = 1"
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

plt.legend()
plt.show()

