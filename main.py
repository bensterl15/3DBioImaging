import matplotlib.pyplot as plt
import numpy as np
import pyvista as pv
# import pyvistaqt as pvqt
import os

# Note: This should be <YOUR_LOCATION>/3DBioImaging
os.getcwd()


dat = np.load("data/Data_042721/AC_cell_published_testset/single_blocked_cell_img.npy")

#dat = np.load("abs_mask.npy")

# Use this to get rid of last x1 dimension:
dat = np.squeeze(dat)

np.shape(dat)

pc = []
scale = 2
for n in range(len(img_array)):
    tmp = np.where(img_array[n] > 0)
    for i in range(len(tmp[0])):
        pc.append([tmp[0][i],tmp[1][i], n/scale])
pc = np.array(pc)



# Plot one slice:
plt.imshow(dat[0:24,120,:])
plt.show()

plt.imshow(dat[25,:,:])
plt.show()

from ripser import Rips

def rgb2gray(rgb):
    return np.dot(rgb[...,:3], [0.2989, 0.5870, 0.1140])


rips = Rips()
#data = np.load("")
data = plt.imread("18.PNG")
data = rgb2gray(data)
data = data - np.min(data)
data = 1 - np.abs(data)
plt.imshow(data)
plt.colorbar()

diagrams = rips.fit_transform(data)
rips.plot(diagrams)

# For now, set threshold equal to 4:
tau = 4

J = np.zeros(np.shape(data))
# Array for lifetimes of H0 and H1 holes:
lifetimes = np.zeros(np.shape(diagrams[0])[0] + np.shape(diagrams[1])[0])

for i in range(np.shape(diagrams[1])[0]):
