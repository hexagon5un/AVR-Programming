## Simple Python routine for creating a new AVR project
## Feel free to extend this to meet your needs

import os
import shutil
import sys


## Get command-line input
class UsageError(Exception):
    pass
try:
    newProjectName = sys.argv[1]
except IndexError:
    raise(UsageError("Please specify a project name on the command-line.\n"))

## Create new directory
relativeDirectory = os.path.join(os.path.pardir, newProjectName)
os.mkdir(relativeDirectory)

## Copy over blank .c file
shutil.copy("main.c", os.path.join(relativeDirectory, newProjectName + ".c"))

## Read in and customize Makefile 
makefile = open("Makefile").read()
makefile = makefile.replace("YOUR_MAIN_PROGRAM", newProjectName)

newMakefile = open(os.path.join(relativeDirectory, "Makefile"), "w")
newMakefile.write(makefile)

print "Copied Makefile and {}.c to {}".format(newProjectName, newProjectName)
print "Look them over, and get to coding."

