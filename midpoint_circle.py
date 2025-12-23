import numpy as np
import matplotlib.pyplot as plt


def draw_rect(x, y):
    plt.plot(
        [x-.5,x+.5,x+.5,x-.5,x-.5],
        [y+.5,y+.5,y-.5,y-.5,y+.5],
        c="k",
        lw=2,
        zorder=-1,
    )

# dim = 10

# for dim in range(1, 10):
#     plt.figure(figsize=(8,8))
#     r = (dim-1)/2
#     cx, cy = r, r
#     plt.scatter(cx, cy, marker="x")
#     a = np.linspace(0, np.pi*2, 100)
#     plt.plot(cx + r * np.cos(a), cy + r * np.sin(a))

#     x1, y1 = 0.5 if dim%2 == 0 else 0, r
#     x2, y2 = x1, y1 - 1
#     xm, ym = x1, y1 - 0.5
#     while (x1 < y1):
#         plt.scatter(cx + x1, cy + y1)
#         plt.scatter(cx + x2, cy + y2)
#         plt.scatter(cx + xm, cy + ym, marker="*")

#         p = (xm)**2 + (ym)**2 - (r)**2
#         if p < 0:
#             xd, yd = x1, y1
#         else:
#             xd, yd = x2, y2
#             y1 -= 1
#             y2 -= 1
#             ym -= 1
#         x1 += 1
#         x2 += 1
#         xm += 1

#         draw_rect(cx + xd, cy + yd)
#         draw_rect(cx + xd, cy - yd)
#         draw_rect(cx - xd, cy + yd)
#         draw_rect(cx - xd, cy - yd)
#         draw_rect(cx + yd, cy + xd)
#         draw_rect(cx + yd, cy - xd)
#         draw_rect(cx - yd, cy + xd)
#         draw_rect(cx - yd, cy - xd)

#     plt.grid()
#     plt.yticks(np.arange(-.5, dim+.5))
#     plt.xticks(np.arange(-.5, dim+.5))
#     plt.ylim(-1, dim)
#     plt.xlim(-1, dim)
#     plt.show()




# dim = 20

# plt.figure(figsize=(8,8))
# r = (dim-1)/2
# cx, cy = r, r
# x, y = 0.5 if dim%2 == 0 else 0, r
# p1 = 0.5 - r if dim%2 == 0 else 0.25 - r
# while (x < y):
#     plt.scatter(cx + x, cy + y)
#     plt.scatter(cx + x, cy + y - 1)
#     plt.scatter(cx + x, cy + y - .5, marker="x")

#     p = (x)**2 + (y - 0.5)**2 - (r)**2
#     print(p, p1)
#     if p < 0:
#         p1 += 2*x + 1
#         yd = y
#     else:
#         p1 += 2*(x - y) + 3
#         yd = y - 1
#         y -= 1

#     draw_rect(cx + x, cy + yd)
#     draw_rect(cx + x, cy - yd)
#     draw_rect(cx - x, cy + yd)
#     draw_rect(cx - x, cy - yd)
#     draw_rect(cx + yd, cy + x)
#     draw_rect(cx + yd, cy - x)
#     draw_rect(cx - yd, cy + x)
#     draw_rect(cx - yd, cy - x)
    
#     x += 1

# plt.scatter(cx, cy, marker="x")
# a = np.linspace(0, np.pi*2, 100)
# plt.plot(cx + r * np.cos(a), cy + r * np.sin(a))
# plt.grid()
# plt.yticks(np.arange(-.5, dim+.5), [""]*(dim+1))
# plt.xticks(np.arange(-.5, dim+.5), [""]*(dim+1))
# plt.ylim(-1, dim)
# plt.xlim(-1, dim)
# plt.show()



# dim = 12

# plt.figure(figsize=(8,8))
# r = (dim-1)/2
# cx, cy = r, r
# x, y = 0.5 if dim%2 == 0 else 0, r
# p = 1.5 - r if dim%2 == 0 else 1.25 - r
# while (x <= y):
#     draw_rect(cx + x, cy + y)
#     draw_rect(cx + x, cy - y)
#     draw_rect(cx - x, cy + y)
#     draw_rect(cx - x, cy - y)
#     draw_rect(cx + y, cy + x)
#     draw_rect(cx + y, cy - x)
#     draw_rect(cx - y, cy + x)
#     draw_rect(cx - y, cy - x)

#     if p < 0:
#         p += 2*x + 3
#     else:
#         p += 2*(x - y) + 5
#         y -= 1
#     x += 1

# plt.scatter(cx, cy, marker="x")
# a = np.linspace(0, np.pi*2, 100)
# plt.plot(cx + r * np.cos(a), cy + r * np.sin(a))
# plt.grid()
# plt.yticks(np.arange(-.5, dim+.5))
# plt.xticks(np.arange(-.5, dim+.5))
# plt.ylim(-1, dim)
# plt.xlim(-1, dim)
# plt.show()





dim = 12

plt.figure(figsize=(8,8))
grid = np.zeros((dim, dim))
r = (dim-1)/2
cx, cy = r, r
x, y = 0.5 if dim%2 == 0 else 0, r
p = 1.5 - r if dim%2 == 0 else 1.25 - r
while (x <= y):
    print(cx, cy, x, y, int(cx + x), int(cy + y))
    grid[int(cx + x), int(cy + y)] = 1
    grid[int(cx + x), int(cy - y)] = 1
    grid[int(cx - x), int(cy + y)] = 1
    grid[int(cx - x), int(cy - y)] = 1
    grid[int(cx + y), int(cy + x)] = 1
    grid[int(cx + y), int(cy - x)] = 1
    grid[int(cx - y), int(cy + x)] = 1
    grid[int(cx - y), int(cy - x)] = 1
    if p < 0:
        p += 2*x + 3
    else:
        p += 2*(x - y) + 5
        y -= 1
    x += 1

plt.imshow(grid)
plt.scatter(cx, cy, marker="x")
a = np.linspace(0, np.pi*2, 100)
plt.plot(cx + r * np.cos(a), cy + r * np.sin(a))
plt.grid()
plt.yticks(np.arange(-.5, dim+.5))
plt.xticks(np.arange(-.5, dim+.5))
plt.ylim(-1, dim)
plt.xlim(-1, dim)
plt.show()
