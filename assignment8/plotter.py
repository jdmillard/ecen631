import matplotlib.pyplot as plt
import numpy as np


A = np.loadtxt("build/rt.txt")
#A = np.loadtxt('rt.txt') # <<< <<< this is the one to use for the C++
print(A.shape)

# now A is a Nx12, the 12 of 1 row create R and T

print(A[0][0])
print(A[0][1])

# use an initial point and R and T to propagate the trajectory
# then plot

t = np.arange(0.0, 2.0, 0.01)
s = 1 + np.sin(2*np.pi*t)
plt.plot(t, s)

plt.xlabel('time (s)')
plt.ylabel('voltage (mV)')
plt.title('About as simple as it gets, folks')
plt.grid(True)
#plt.savefig("test.png")
plt.show()
