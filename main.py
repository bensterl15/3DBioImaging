import matplotlib.pyplot as plt
import numpy as np
import os

# Note: This should be <YOUR_LOCATION>/3DBioImaging
os.getcwd()


dat = np.load("data/Data_042721/AC_cell_published_testset/single_blocked_cell_img.npy")

# Use this to get rid of last x1 dimension:
dat = np.squeeze(dat)

# Plot one slice:
plt.imshow(dat[50,:,:])
plt.show()
