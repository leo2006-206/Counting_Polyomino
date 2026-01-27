import math
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
# ------------ data ------------

centroid = (0,0)

non_empty = [
(0,0) ,
(0,1) ,
(0,2) ,
(0,3) ,
(0,4) ,
(0,5) ,
(0,6) ,
]
empty = [
(0,-1) ,
(-1,0) ,
(1,0) ,
(-1,1) ,
(1,1) ,
(-1,2) ,
(1,2) ,
(-1,3) ,
(1,3) ,
(-1,4) ,
(1,4) ,
(-1,5) ,
(1,5) ,
(-1,6) ,
(1,6) ,
(0,7) ,
]

def dist_from_origin(x, y):
    return math.hypot(x, y)

# ------------ plotting ------------

fig, ax = plt.subplots(figsize=(36, 28))

CELL_SIZE = 1.0
HALF = CELL_SIZE / 2

for x, y in non_empty:
    rect = Rectangle(
        (x - HALF, y - HALF),
        CELL_SIZE,
        CELL_SIZE,
        facecolor='pink',
        edgecolor='black'
    )
    ax.add_patch(rect)
    # ax.text(x, y, str(cid), ha='center', va='center', fontsize=8, color='gray')

for x, y in empty:
    rect = Rectangle(
        (x - HALF, y - HALF),
        CELL_SIZE,
        CELL_SIZE,
        facecolor='white',
        edgecolor='black'
    )
    ax.add_patch(rect)
    # ax.text(x, y, str(cid), ha='center', va='center', fontsize=8)

for x, y in non_empty + empty:
    d = dist_from_origin(x, y)
    ax.text(x, y - 0.25, f"{d:.2f}", ha='center', va='top', fontsize=24, color='gray')


# Plot centroid
ax.scatter([centroid[0]], [centroid[1]], marker='x', color='red', s=80, label='Centroid')
ax.text(centroid[0], centroid[1], 'C', color='red', ha='left', va='bottom', fontsize=24)

ax.set_xlabel("x",fontsize=24)
ax.set_ylabel("y",fontsize=24)
ax.set_title("Poly cells with IDs", fontsize=24)

ax.tick_params(axis='both', which='major', labelsize=24)


# # Make it look like a grid of cells
# ax.set_aspect('equal', adjustable='box')
# ax.grid(True, linestyle='--', linewidth=0.5)

ax.set_xlim(-5, 5)
ax.set_ylim(-5, 5)
ax.set_aspect('equal', adjustable='box')
ax.grid(True, linestyle='--', linewidth=1)

plt.subplots_adjust(left=0.05, right=0.95, top=0.95, bottom=0.05)


ax.legend()
# plt.tight_layout()
plt.show()
