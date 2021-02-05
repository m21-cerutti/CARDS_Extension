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
        if(s != "" and not s.startswith(("//", "#", "\\\\"))):
            path_s = os.path.expandvars(s)
            for d in DEST:
                path_d = os.path.relpath(d.replace("\n",""))
                print(">"+path_s+"\n"+path_d+"\n")
                copy(path_s, path_d)
