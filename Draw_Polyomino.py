import math
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
# ------------ data ------------

centroid = (0,0)

non_empty = [
(1,-1) ,
(0,0) ,
(1,0) ,
(0,1) ,
]
empty = [
(1,-2) ,
(0,-1) ,
(2,-1) ,
(-1,0) ,
(2,0) ,
(-1,1) ,
(1,1) ,
(0,2) ,
]


def dist_from_origin(x, y):
    return math.hypot(x, y)

# ------------ plotting ------------

# Note: Reduced the figsize slightly so it isn't completely massive when imported to PowerPoint/Slides
fig, ax = plt.subplots(figsize=(16, 12)) 

CELL_SIZE = 1.0
HALF = CELL_SIZE / 2

for x, y in non_empty:
    rect = Rectangle(
        (x - HALF, y - HALF),
        CELL_SIZE,
        CELL_SIZE,
        facecolor='lightblue',
        # facecolor="pink",
        edgecolor='black'
    )
    ax.add_patch(rect)
    # ax.text(x, y, str(f"({x}, {y})"), ha='center', va='center', fontsize=20, color='black')

for x, y in empty:
    rect = Rectangle(
        (x - HALF, y - HALF),
        CELL_SIZE,
        CELL_SIZE,
        facecolor='white', # Note: 'white' here will stay white in the cells, which is good for contrast!
        edgecolor='black'
    )
    ax.add_patch(rect)

ax.set_xlabel("x",fontsize=24)
ax.set_ylabel("y",fontsize=24)
ax.set_title("Poly cells with IDs", fontsize=24)

ax.tick_params(axis='both', which='major', labelsize=20)

ax.set_xlim(-5, 5)
ax.set_ylim(-5, 5)
ax.set_aspect('equal', adjustable='box')
ax.grid(True, linestyle='--', linewidth=1)

plt.subplots_adjust(left=0.05, right=0.95, top=0.95, bottom=0.05)

# The magic line to save as a transparent SVG
# bbox_inches='tight' trims the excess empty space around the edges
plt.savefig('polyomino.svg', transparent=True, format='svg', bbox_inches='tight')

# plt.show() # Commented out so it just saves the file