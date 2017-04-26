import matplotlib.pyplot as plt
import numpy as np

# as of right now, this needs to be converted into "abcdefg" format and pasted
# into the c++. This can be fixed if this file could be read into c++ then
# converted to char for the PyRun_SimpleString command.

A = np.loadtxt('build/rt.txt')
#A = np.loadtxt('rt.txt') # <<< <<< this is the one to use for the C++ # <<<<<<<<<<<<<<<<<<<<<<<<<<<
print(A.shape)
print(A.shape[0])

# now A is a Nx12, the 12 of 1 row create R and T

for i in range(0, A.shape[0]):
    # A[i] is the current row

    # generate current transformation
    Tk = np.reshape(A[i], (3,4), 0) # 0 means fill row first
    last = np.zeros((1,4))
    last[0][3] = 1 # for completing Tk [0 0 0 1]
    Tk = np.vstack((Tk, last))

    # get the current R
    R = Tk[0:3, 0:3]

    # get the current T
    T = Tk[0:3, 3]
    T = np.reshape(T, (3,1), 1) # 1 means fill column first
    print('---')
    print(T)

# it appears that the first value of T is the x in the truth plot
# it appears that the 3rd value of T is the y in the truth plot
# further, it seems that the translations are accumulative, not frame-to-frame
# so each Tk is the transformation from the beginning to current



# use an initial point (0, 0) and R and T to propagate the trajectory
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
