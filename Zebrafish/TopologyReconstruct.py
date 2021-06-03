import matplotlib.pyplot as plt
import numpy as np
import pyvista as pv
import pyvistaqt as pvqt
from ripser import Rips, lower_star_img
import scipy
from scipy import sparse
import os

class TopologyReconstruct:
    def __init__(self, data, tau = 0.1):
        self.rips = Rips(maxdim = 2)
        self.data = data
        self.tau = tau
        
    def outline_boundary(self):
        self.X, self.Y = np.meshgrid(np.arange(self.data.shape[1]), 
                                 np.arange(self.data.shape[2]))
        self.X = self.X.flatten()
        self.Y = self.Y.flatten()
                
        
        # Note: This for loop can be COMPLETELY parallel processed:
        for layer in np.arange(self.data.shape[0]):
            print(layer)
            self.__outline_slice__(layer)
            
    # Private method to outline (topologically) each slice of the zebrafish:
    def __outline_slice__(self, layer):
        # Work with the current 2D slice:
        data = self.data[layer, :, :]

        # TEMPORARY: Invert pixels:
        data = data / np.max(data)
        data = 1 - np.abs(data)
        data = data / np.max(data)
        data = np.abs(data)

        # Combine H0 and H1 holes into here:
        #dgm = np.concatenate((diagrams[0], diagrams[1], diagrams[2]))
        dgm = lower_star_img(data)

        # Array for lifetimes of H0 and H1 holes:
        lifetimes = np.zeros(np.shape(dgm)[0])
        births = np.zeros(np.shape(lifetimes))
        deaths = np.zeros(np.shape(lifetimes))

        for i in range(np.shape(dgm)[0]):
            births[i] = dgm[i,0]
            deaths[i] = dgm[i,1]
            
        lifetimes = deaths - births
    
        # Select the indices below threshold value:
        indices = np.argwhere((self.tau < lifetimes) & (lifetimes != np.Inf))
        desc_order = np.argsort(np.squeeze(deaths[indices]))[::-1]
        indices = indices[desc_order]

        birth_coords = list()
        death_coords = list()

        # Binary output indicating boundary:
        J = np.zeros(np.shape(data))

        for idx in indices:
            # Birth pixel coordinate:
            bidx = np.argmin(np.abs(data + dgm[idx, 0]))
            birth_coord = np.array([self.X[bidx], self.Y[bidx]])
            birth_coords.append(birth_coord)

            # Death pixel coordinate:
            bidx = np.argmin(np.abs(data + dgm[idx, 1]))
            death_coord = np.array([self.X[bidx], self.Y[bidx]])
            death_coords.append(death_coord)
            
            # Get all pixels connected to the birth pixel right before its death:
            birth_val = data[birth_coord[0],birth_coord[1]]
            C = (birth_val <= data) & (data < dgm[idx, 1]) # dgm[idx, 1] is death value at current index from persistence diagram
            C_ = list()
            for i in range(np.shape(C)[0]):
                for j in range(np.shape(C)[1]):
                    if(C[i,j]):
                        C_.append(np.array([i,j]))

            # Check if C contains death pixels of previous components:
            #intersection = list() #list(set(C_) & set(death_coords))
            # Find intersection of ds and C:
            pts = list()
            for i1 in C_:
                for i2 in death_coords:
                    if(np.array_equal(i1, i2)):
                        #intersection.append(i1)
                        for pt in data[i1].flatten().tolist():
                            pts.append(pt)
            #print(intersection)
            #pts = (data[intersection].flatten()).tolist()
            pts.append(dgm[idx, 1][0])
            #print(pts)
            new_dval = min(pts)
            
            # Ensure C does not overlap with any previous component:
            #C = (birth_val <= data) & (data < new_dval)
            
            # Mark the component for this diagram point in the output:
            J[C] = 1
            
        # Smooth the mask with 2D convolution:
        J_smoothed = scipy.ndimage.gaussian_filter(J, 2)
        J_smoothed = np.round(J_smoothed)
            
        # Apply the mask to our image:
        self.data[layer, :, :] = J_smoothed