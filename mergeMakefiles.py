
## Merge each project's makefile (presumable with good per-project headers
##  with a sample version, with updated mechanics

separator = "Makefile Magic!"
mainFile = "setupProject/Makefile"

import os
import shutil

makefiles = []
for root, dirs, files in os.walk("."):
    if "Makefile" in files:
        makefiles.append(root + "/Makefile")

def getTop(filename):
    string = ""
    for l in open(filename).readlines():
        if "PROGRAMMER_TYPE = usbasp" in l:
            l = "PROGRAMMER_TYPE = usbtiny\n"
        if separator in l:
            break
        else:
            string += l
    return string

def getBottom(filename):
    string = ""
    seenSeparator = False
    for l in open(filename).readlines():
        if separator in l:
            seenSeparator = True
        if seenSeparator:
            string += l
    return string

bottom = getBottom(mainFile)

for f in makefiles:
    top = getTop(f)
    os.rename(f, f+"_old")
    open(f, "w").write(top + bottom)


