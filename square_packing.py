# from math import isqrt

# for i in range(1000):
#     num = 2*(i*i + i) + 1
#     isqrt_num = isqrt(num)
#     if isqrt_num*isqrt_num == num:
#         print(i, num)

import numpy as np
import matplotlib.pyplot as plt

# max_r = 25
# for max_r in range(3, 300):
#     j = np.ceil((-1 + np.sqrt(1 + 2*max_r*(max_r + 1)))/2).astype(int)
#     # print(j)

#     for i in range(j - 5, j + 6):
#         row1 = sum(range(1, i+1))
#         row2 = sum(range(i+1, max_r+1))
#         diff = row1 - row2
#         # print(f"{i:>5}:{row1:>5},{row2:<5}{(1,i)},{(i+1,max_r)}", diff)


#     row1 = (1 + j)*j//2
#     row2 = (j+1 + max_r)*(max_r - j)//2
#     # print(f"{j=}, {row1=}, {row2=}")

#     grid = np.zeros((2*j + 1, row1))
#     for i in range(j):
#         imin = 0
#         imax = i
#         jmin = (1 + i)*i//2
#         jmax = jmin + i
#         grid[imin:imax+1, jmin:jmax+1] = i+1
#         # print("row1:", i, imin, imax, jmin, jmax)
#     for i in range(j, max_r):
#         imax = 2*j
#         imin = imax - i
#         jmax = row1-1 - (j+1 + i)*(i - j)//2
#         jmin = jmax - i
#         grid[imin:imax+1, jmin:jmax+1] = i+1
#         # print("row2:", i, imin, imax, jmin, jmax)

#     print(f"{max_r}: {np.count_nonzero(grid)/np.size(grid)*100:.3g}%")
#     plt.scatter(max_r, np.count_nonzero(grid)/np.size(grid)*100)

#     # plt.imshow(grid)
#     # # plt.pcolormesh(grid, edgecolors='k', linewidth=1)
#     # # plt.gca().set_aspect('equal')
#     # # plt.gca().invert_yaxis()
#     # plt.show()
# plt.show()




# # working

# max_r = 21

# idx_switch = np.ceil((-1 + np.sqrt(1 + 2*max_r*(max_r + 1)))/2).astype(int)
# total_width = (1 + idx_switch)*idx_switch//2
# total_height = 2*idx_switch + 1

# grid = np.zeros((total_height, total_width))
# for i in range(idx_switch):
#     imin = 0
#     imax = i
#     jmin = (1 + i)*i//2
#     jmax = jmin + i
#     grid[imin:imax+1, jmin:jmax+1] = i+1

# for i in range(idx_switch, max_r):
#     imax = 2*idx_switch
#     imin = imax - i
#     jmax = total_width-1 - (idx_switch+1 + i)*(i - idx_switch)//2
#     jmin = jmax - i
#     grid[imin:imax+1, jmin:jmax+1] = i+1

# plt.imshow(grid)
# plt.show()

# # working

# min_idx = 3
# max_idx = 32

# idx_switch = np.ceil((-1 + np.sqrt((1 + min_idx)*(1 + min_idx)/4 + 2*max_idx*(max_idx + 1)))/2).astype(int)
# total_width = (min_idx + idx_switch)*(idx_switch - min_idx + 1)//2
# total_height = 2*idx_switch + 1

# grid = np.zeros((total_height, total_width))
# for idx in range(min_idx - 1, idx_switch):
#     imin = 0
#     imax = idx
#     jmin = (min_idx + idx)*(idx - min_idx + 1)//2
#     jmax = jmin + idx
#     grid[imin:imax+1, jmin:jmax+1] = idx+5

# for idx in range(idx_switch, max_idx):
#     imax = 2*idx_switch
#     imin = imax - idx
#     jmax = total_width-1 - (idx_switch+1 + idx)*(idx - idx_switch)//2
#     jmin = jmax - idx
#     grid[imin:imax+1, jmin:jmax+1] = idx+5

# plt.imshow(grid)
# plt.show()




# min_idx = 1
# max_idx = 20

# idx_switch = np.ceil((-1 + np.sqrt((1 + min_idx)*(1 + min_idx)/4 + 2*max_idx*(max_idx + 1)))/2).astype(int) - 2
# total_width = (min_idx + idx_switch)*(idx_switch - min_idx + 1)//2
# total_height = 2*idx_switch + 1

# grid = np.zeros((total_height, total_width))
# for idx in range(idx_switch):
#     imin = 0
#     imax = idx+2
#     jmin = (min_idx + idx+2)*(idx+2 - min_idx + 1)//2
#     jmax = jmin + idx+2
#     grid[imin:imax+1, jmin:jmax+1] = idx+5

# for idx in range(idx_switch, max_idx - 2):
#     imax = 2*idx_switch
#     imin = imax - idx
#     jmax = total_width-1 - (idx_switch+1 + idx)*(idx - idx_switch)//2
#     jmin = jmax - idx
#     grid[imin:imax+1, jmin:jmax+1] = idx+5

# plt.imshow(grid)
# plt.show()



# max_r = 127

# idx_switch = np.ceil((-1 + np.sqrt(1 + 2*max_r*(max_r + 1)))/2).astype(int)
# total_width = (1 + idx_switch)*idx_switch//2

# grid = np.zeros(((2*idx_switch + 1)*2, total_width))
# for i in range(idx_switch):
#     imin = 0
#     imax = i
#     jmin = (1 + i)*i//2
#     jmax = jmin + i
#     grid[imin:imax+1, jmin:jmax+1] = i+1
# for i in range(idx_switch, max_r):
#     imax = 2*idx_switch
#     imin = imax - i
#     jmax = total_width-1 - (idx_switch+1 + i)*(i - idx_switch)//2
#     jmin = jmax - i
#     grid[imin:imax+1, jmin:jmax+1] = i+1
# for i in range(idx_switch):
#     imin = 0 + (2*idx_switch + 1)
#     imax = i + (2*idx_switch + 1)
#     jmin = (1 + i)*i//2
#     jmax = jmin + i
#     grid[imin:imax+1, jmin:jmax+1] = i+1
# for i in range(idx_switch, max_r):
#     imax = 2*idx_switch + (2*idx_switch + 1)
#     imin = imax - i
#     jmax = total_width-1 - (idx_switch+1 + i)*(i - idx_switch)//2
#     jmin = jmax - i
#     grid[imin:imax+1, jmin:jmax+1] = i+1
# print(grid.shape)
# plt.imshow(grid)
# plt.show()




# def draw_midpoint_circle(radius):
#     cx = radius
#     cy = radius
#     grid = np.zeros((radius*2 + 1, radius*2 + 1))
#     x = 0
#     y = radius
#     d = 1.25 - radius

#     while x <= y:
#         # plt.scatter(cx + x, cy - y, marker="s", s=60)
#         # plt.scatter(cx + y, cy - x, marker="s", s=60)
#         # plt.scatter(cx + y, cy + x, marker="s", s=60)
#         # plt.scatter(cx + x, cy + y, marker="s", s=60)
#         # plt.scatter(cx - x, cy + y, marker="s", s=60)
#         # plt.scatter(cx - y, cy + x, marker="s", s=60)
#         # plt.scatter(cx - y, cy - x, marker="s", s=60)
#         # plt.scatter(cx - x, cy - y, marker="s", s=60)
#         grid[int(cx + x), int(cy - y)] = 1
#         grid[int(cx + y), int(cy - x)] = 1
#         grid[int(cx + y), int(cy + x)] = 1
#         grid[int(cx + x), int(cy + y)] = 1
#         grid[int(cx - x), int(cy + y)] = 1
#         grid[int(cx - y), int(cy + x)] = 1
#         grid[int(cx - y), int(cy - x)] = 1
#         grid[int(cx - x), int(cy - y)] = 1
    
#         if d < 0:
#             d += 2 * x + 3
#         else:
#             d += 2 * (x - y) + 5
#             y -= 1
#         x += 1
#     return grid

# for n in range(1, 11):
#     # draw_midpoint_circle(0, 0, n)
#     # plt.grid()
#     # plt.ylim(-n/2-2, n/2+2)
#     # plt.xlim(-n/2-2, n/2+2)
#     # plt.axis("equal")
#     # plt.show()
#     grid = draw_midpoint_circle(n)
#     plt.imshow(grid)
#     plt.show()




def draw_circle(grid, r):
    cx = r
    cy = r
    x = 0
    y = r
    d = 1.25 - r
    while x <= y:
        grid[int(cx + x), int(cy - y)] = 1
        grid[int(cx + y), int(cy - x)] = 1
        grid[int(cx + y), int(cy + x)] = 1
        grid[int(cx + x), int(cy + y)] = 1
        grid[int(cx - x), int(cy + y)] = 1
        grid[int(cx - y), int(cy + x)] = 1
        grid[int(cx - y), int(cy - x)] = 1
        grid[int(cx - x), int(cy - y)] = 1
        if d < 0:
            d += 2 * x + 3
        else:
            d += 2 * (x - y) + 5
            y -= 1
        x += 1

for r in [11]:
    grid = np.zeros((r*2 + 1, r*2 + 1))
    draw_circle(grid, r)
    plt.imshow(grid)
    plt.show()




# for r in range(1, 11):
#     grid = np.zeros((r*2 + 1, r*2 + 1))

#     cx = r
#     cy = r
#     x = 0
#     y = r
#     d = 1.25 - r
#     while x <= y:
#         grid[int(cx + x), int(cy - y)] = 1
#         grid[int(cx + y), int(cy - x)] = 1
#         grid[int(cx + y), int(cy + x)] = 1
#         grid[int(cx + x), int(cy + y)] = 1
#         grid[int(cx - x), int(cy + y)] = 1
#         grid[int(cx - y), int(cy + x)] = 1
#         grid[int(cx - y), int(cy - x)] = 1
#         grid[int(cx - x), int(cy - y)] = 1
#         if d < 0:
#             d += 2 * x + 3
#         else:
#             d += 2 * (x - y) + 5
#             y -= 1
#         x += 1

#     plt.imshow(grid)
#     plt.show()


# dim = 10
# grid = np.zeros((dim, dim))

# cx = dim/2
# cy = dim/2
# x1 = 0
# y1 = dim/2
# d = 1.25 - dim
# while x <= y:
#     grid[int(cx + x), int(cy - y)] = 1
#     grid[int(cx + y), int(cy - x)] = 1
#     grid[int(cx + y), int(cy + x)] = 1
#     grid[int(cx + x), int(cy + y)] = 1
#     grid[int(cx - x), int(cy + y)] = 1
#     grid[int(cx - y), int(cy + x)] = 1
#     grid[int(cx - y), int(cy - x)] = 1
#     grid[int(cx - x), int(cy - y)] = 1
#     if d < 0:
#         d += 2 * x + 3
#     else:
#         d += 2 * (x - y) + 5
#         y -= 1
#     x += 1

# plt.imshow(grid)


