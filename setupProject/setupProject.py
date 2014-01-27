#! /usr/bin/env python
## Simple Python routine for creating a new AVR project
## Feel free to extend this to meet your needs

## In particular, main.h includes nearly every AVR library you'll ever need
##   which is no problem, b/c the linker will just ignore the unused ones.
##   But if you're not using them, it might cause confusion later.
##   Trim them down to fit?

## Also, the makefile uses a relative path to my AVR-Programming-Library
## If stuff is moved around, you'll need to edit the makefile accordingly.
## That is, make sure that EXTRA_SOURCE_DIR points to your library.

## Or, if you're feeling DIY, you can just copy the Makefile, main.c and main.h
## into a new directory yourself.  The other files are optional, but handy. 

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

## Create new project directory...

## ... in parent directory
## relativeDirectory = os.path.join(os.path.pardir, newProjectName)

## ... or in this directory, and you get to move it yourself.
relativeDirectory = newProjectName

os.mkdir(relativeDirectory)

## Replace generic "main.h" and "main.c" references in main.c and Makefile
##  and copy over into destination directory
codeFilename = newProjectName + ".c"
headerFilename = newProjectName + ".h"

## Function to search/replace string and copy file over in one go
def replaceAndCopy(templateFilename, newFilename, findString, replaceWith):
    template = open(templateFilename).read()
    template = template.replace(findString, replaceWith)
    newTemplate = open(os.path.join(relativeDirectory, newFilename), "w")
    newTemplate.write(template)

replaceAndCopy("main.c", codeFilename, "main.h", headerFilename)
replaceAndCopy("Makefile", "Makefile", "main.c", codeFilename)
shutil.copy("main.h", os.path.join(relativeDirectory, headerFilename))

## Other files can be copied directly over...
def copyToNewDirectory(whichFile, newDirectory):
    shutil.copy(whichFile, newDirectory)
## ... these ones. 
for filename in ["USART.h", "USART.c", "macros.h"]:
    copyToNewDirectory(filename, relativeDirectory)

print "Copied Makefile, main.c, and main.h into %s." % relativeDirectory
print "Time to start coding."

