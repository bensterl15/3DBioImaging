vertices = []
vt = []
f2d = []
f3d = []

#import rhinoscriptsyntax as rs
import numpy as np

from scipy.io import savemat


def ImportPoints():
    #prompt the user for a file to import
    filename = 'C:\\Users\\bsterling\\Desktop\\3DBioImaging\\Zebrafish\\Tracking\\zebrafish_flattened.obj'

    if not filename: return

    #read each line from the file
    file = open(filename, "r")
    contents = file.readlines()
    file.close()

    def processLine(line):
        lineData = line.split(' ')
        type = lineData[0]
        if type == 'v':
            x = float(lineData[1])
            y = float(lineData[2])
            z = float(lineData[3])
            #print "vertex: {},{},{}".format(x,y,z)
            global vertices
            vertices.append((x,y,z))
        elif type =='vt':
            u = float(lineData[1])
            v = float(lineData[2])
            #print "texture: {},{}".format(u,v)
            global vt
            vt.append((u,v,0))
        elif type =='f':
            v0, vt0 = map(int,lineData[1].split('/',1))
            v1, vt1 = map(int,lineData[2].split('/',1))
            v2, vt2 = map(int,lineData[3].split('/',1))
            #print "face: {},{},{}".format(v0,v1,v2)
            global f
            f2d.append((vt0, vt1, vt2))
            f3d.append((v0, v1, v2))

    contents = [processLine(line) for line in contents]

###
def ImportPoints2():
    #prompt the user for a file to import
    filename = 'C:\\Users\\bsterling\\Desktop\\3DBioImaging\\Zebrafish\\Tracking\\zebrafish.obj'

    if not filename: return

    #read each line from the file
    file = open(filename, "r")
    contents = file.readlines()
    file.close()

    def processLine(line):
        lineData = line.split(' ')
        type = lineData[0]
        if type == 'v':
            x = float(lineData[1])
            y = float(lineData[2])
            z = float(lineData[3])
            #print "vertex: {},{},{}".format(x,y,z)
            global vertices
            vertices.append((x,y,z))
        elif type =='f':
            v0 = float(lineData[1])
            v1 = float(lineData[2])
            v2 = float(lineData[3])
            global f
            f2d.append((v0, v1, v2))

    contents = [processLine(line) for line in contents]
###

#ImportPoints2()
ImportPoints()

print(np.asarray(vt))
print(np.shape(vt))


mdic = {
"zebrafish_vertices" : np.asarray(vertices),
"zebrafish_flat_vertices": np.asarray(vt),
"zebrafish_flat_faces": np.asarray(f2d), 
"zebrafish_flat_faces_3D": np.asarray(f3d)
}

savemat('zebrafish_flat.mat', mdic)