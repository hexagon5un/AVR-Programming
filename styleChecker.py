## Checks through all subdirectories: .c .h files
## Replaces makes code changes.

import os, re


def unique(myList):
    return(list(set(myList)))


def allFiles(rootDir = "."):
    filenames = []
    for root, subdir, fname in os.walk("."):
        filenames.extend([os.path.join(root, x) for x in fname if x.endswith(".c")])
        filenames.extend([os.path.join(root, x) for x in fname if x.endswith(".h")])
    return(filenames)

def replaceBV(text):
    reBV = re.compile(r'_BV\((.*?)\)')
    return(reBV.sub(r'(1 << \1)', text))


if __name__ == "__main__":

    for filename in sorted(allFiles()):
        inText = open(filename).read()
        outText = replaceBV(inText)

        open(filename, "w").write(outText)
        

