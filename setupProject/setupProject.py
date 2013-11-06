## Simple Python routine for creating a new AVR project
## Feel free to extend this to meet your needs

## In particular, main.h includes nearly every AVR library you'll ever need
##   which is no problem, b/c the linker will just ignore the unused ones.
##   But it doesn't look good.  Trim it down to fit?

## Also, the makefile uses a relative path to my AVR-Programming-Library
## If stuff is moved around, you'll need to edit the makefile accordingly.
## That is, make sure that EXTRA_SOURCE_DIR points to your library.

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
shutil.copy("main.c", relativeDirectory)
shutil.copy("main.h", relativeDirectory)
## Or break out into two files
## shutil.copy("main.h", os.path.join(relativeDirectory, newProjectName + ".h"))
## shutil.copy("main.h", os.path.join(relativeDirectory, newProjectName + ".h"))

## Read in and customize Makefile 
makefile = open("Makefile").read()
## makefile = makefile.replace("YOUR_MAIN_PROGRAM", newProjectName)
makefile = makefile.replace("YOUR_MAIN_PROGRAM.c", "main.c")
newMakefile = open(os.path.join(relativeDirectory, "Makefile"), "w")
newMakefile.write(makefile)

print "Copied Makefile, main.c, and main.h into %s." % relativeDirectory
print "Time to start coding."

