import matplotlib.pyplot as plt
import numpy as np

# as of right now, this needs to be converted into "abcdefg" format and pasted
# into the c++. This can be fixed if this file could be read into c++ then
# converted to char for the PyRun_SimpleString command.

A = np.loadtxt('build/rt.txt')
#A = np.loadtxt('rt.txt') # <<< <<< this is the one to use for the C++ # <<<<<<<<<<<<<<<<<<<<<<<<<<<
# now A is a Nx12, the 12 of 1 row create R and T

T_all = np.zeros((3,1))

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
    #print('---')
    #print(T)

    # update the full set of T_all
    T_all = np.hstack((T_all, T))
    #print('---')
    #print(T_all)

# the 1st value of T is the x in the truth plot
# the 3rd value of T is the y in the truth plot
# translations are accumulative, not frame-to-frame
# each Tk is the transformation from the beginning to current

plt.plot(T_all[0,:], T_all[2,:])

plt.xlabel('x')
plt.ylabel('y')
plt.title('visual odometry trajectory')
plt.grid(True)
plt.axis('equal')
#plt.savefig('trajectory.png')
plt.show()
