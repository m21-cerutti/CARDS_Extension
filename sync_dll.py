
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

if(len(sys.argv) > 1):
    print("DEBUG SYNCHRONISATION")
    ENV_CONFIG_VAR  = "Debug"
else:
    use_debug_config=False
    ENV_CONFIG_VAR  = "Release"

def GetFileContent(path):
    with open(path) as f:
        content = f.readlines()
    return [x.strip() for x in content] 

print("Opencv dir : "+ os.path.expandvars("%OPENCV_DIR%"))

COMMENTARIES = ["//", "#", "\\\\"]

SRC_DLL = GetFileContent("./dll_{}.txt".format(ENV_CONFIG_VAR))
DEST = GetFileContent("./folder_{}.txt".format(ENV_CONFIG_VAR))

for s in SRC_DLL:
    if(s != "" and not s.startswith(tuple(COMMENTARIES))): 
        path_s = os.path.abspath(os.path.expandvars(s))
        filename = os.path.basename(path_s)
        for d in DEST:
            if(d != "" and not d.startswith(tuple(COMMENTARIES))): 
                path_d = os.path.abspath(os.path.join(d, filename))
                print(">"+path_s+"\n"+path_d)
                if(os.path.exists(path_d)):
                    os.remove(path_d)
                try:
                    os.symlink(path_s, path_d)
                except Exception as e:
                    print(e)
                    print("ERROR")
                    exit(1)
                print()

print("SYNC DONE")