
import ctypes, sys

def is_admin():
    try:
        return ctypes.windll.shell32.IsUserAnAdmin()
    except:
        return False
        
if not is_admin():
    print("Rerun in admin mode")
    exit(1)

import os
from shutil import copy
from pathlib import Path

print("Opencv dir : "+ os.path.expandvars("%OPENCV_DIR%"))
DEP = "./dll.txt"
DEST = {"./unity/Assets/Plugins/Windows/", "./unity/Resources/Plugins/", "./console/x64/Release", "./console/x64/Debug"}

with open(DEP) as fp:
    lines = fp.readlines()
    for s in lines:
        s = s.replace("\n","")
        if(s != "" and not s.startswith(("//", "#", "\\\\"))):
            path_s = os.path.abspath(os.path.expandvars(s))
            filename = os.path.basename(path_s)
            for d in DEST:
                path_d = os.path.abspath(os.path.join(d.replace("\n",""), filename))
                print(">"+path_s+"\n"+path_d+"\n")
                if(os.path.exists(path_d)):
                    os.remove(path_d)
                try:
                    os.symlink(path_s, path_d)
                except Exception as e:
                    print(e)
                print()