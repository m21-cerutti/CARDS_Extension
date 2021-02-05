import os
from shutil import copy
from pathlib import Path

print("Opencv dir : "+ os.path.expandvars("%OPENCV_DIR%/"))
DEP = "./dll.txt"
DEST = {"./unity/Assets/Plugins", "./console/x64/Release", "./console/x64/Debug"}

with open(DEP) as fp:
    lines = fp.readlines()
    for s in lines:
        s = s.replace("\n","")
        if(s != ""):
            path = os.path.expandvars(s)
            for d in DEST:
                print(">"+path+"\n"+d+"\n")
                copy(path, d.replace("\n",""))
