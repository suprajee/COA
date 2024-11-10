# Updated data based on the provided values
thread_counts = [2, 4, 8, 12, 24]  # Threads
matrix_sizes = [10, 100, 10000]  # Matrix sizes (N values)
times = [
    [0.000284, 0.005473, 29.765233],   # 2 threads
    [0.000363, 0.003278, 17.629962],   # 4 threads
    [0.008583, 0.018115, 14.739962],   # 8 threads
    [0.000495, 0.001731, 14.438564],   # 12 threads
    [0.004307, 0.005755, 13.020620],   # 24 threads
]
sequential_times = [0.000036, 0.005342, 46.765555]  # Sequential times for comparison

# Creating a mesh grid for 3D plotting
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

X, Y = np.meshgrid(matrix_sizes, thread_counts)
Z = np.array(times)

# Create the 3D plot
fig = plt.figure(figsize=(12, 8))
ax = fig.add_subplot(111, projection='3d')

# Plot surface
surf = ax.plot_surface(X, Y, Z, cmap='viridis', edgecolor='k', alpha=0.8)

# Plot sequential times as a line for reference
ax.plot(matrix_sizes, [0] * len(matrix_sizes), sequential_times, color='r', marker='o', label='Sequential Time')

# Labels and title
ax.set_xlabel("Matrix Size (N)")
ax.set_ylabel("Threads")
ax.set_zlabel("Time (seconds)")
ax.set_title("Jacobi 2D Parallel Execution Time vs. Threads and Matrix Size")

# Add color bar
fig.colorbar(surf, ax=ax, shrink=0.5, aspect=5)
ax.legend()

plt.show()
