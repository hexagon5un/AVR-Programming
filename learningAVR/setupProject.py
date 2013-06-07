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


relativeDirectory = os.path.join(os.path.pardir, newProjectName)
os.mkdir(relativeDirectory)
shutil.copy("Makefile", relativeDirectory)
shutil.copy("main.c", relativeDirectory)

print "Copied Makefile and main.c to {}".format(newProjectName)
print "Look them over, and get to coding."

