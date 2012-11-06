## Checks through all subdirectories: .c .h files
## Replaces makes code changes.

import os, re


def unique(myList):
    return(list(set(myList)))


def allFiles(rootDir = ".", extensions=[".c", ".h"]):
    filenames = []
    for root, subdir, fnames in os.walk("."):
        fullnames = [os.path.join(root, x) for x in fnames]
        fullnames = [x for x in fullnames if x[-2:] in extensions]
        filenames.extend(fullnames)
    return(filenames)

def replaceBV(text):
    ## I'm nearly sure this could be a one-liner in sed...
    reBV = re.compile(r'_BV\((.*?)\)')
    return(reBV.sub(r'(1 << \1)', text))


if __name__ == "__main__":

    for filename in sorted(allFiles()):
        inText = open(filename).read()
        outText = replaceBV(inText)

        open(filename, "w").write(outText)
        

