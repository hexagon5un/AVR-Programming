#! /usr/bin/env python
import os
import re
import shutil

commentMatcherC   = re.compile('/\*(.*?)\*/')
commentMatcherCpp = re.compile('//(.*?)$')


def hasComment(string):
    if "/*" in string and "*/" in string:
	return(True)
    if "//" in string:
	return(True)
    return(False)
    
def breakIntoBlocks(string):
    blocks = string.split("\n\n")
    return(blocks)

def splitComment(string):
    try:
	splitPoint = commentMatcherC.search(string).start()
	code = string[:splitPoint]
	comment = commentMatcherC.findall(string)[0].strip()
    except AttributeError: 
	code = string
	comment = None
    code = trimTrailingSpace(code)
    return(code, comment)

def findLengths(string):
    code, comment = splitComment(string)
    try:
	return([len(code), len(comment)])
    except TypeError:  # returned None, has no length
	return([len(code),0])

def makeComment(string):
    return("/* " + string + " */")

def rightJustifyComments(string):
    code, comment = splitComment(string)
    extraSpaces = MAXWIDTH - sum(findLengths(string)) - 6 # delimiter chars
    if comment:
	formattedComment = makeComment(comment)
	if extraSpaces > 0:
	    return(code + " "*(extraSpaces) + formattedComment)
	else: # longer than maxwidth, newline
	    return(formattedComment + "\n" + code)
    else:
	return(code)

def rightJustify(string):
    extraSpaces = MAXWIDTH -len(string)
    return(" "*(extraSpaces) + string)

def trimTrailingSpace(string):
    while string.endswith(" "):
	string = string[:-1]
    return(string)

def alignComment(code, comment, commentStart):
    internalSpaces = MAXWIDTH - commentStart - len(comment) - 6
    formattedComment = makeComment(comment + " "*internalSpaces)

    if len(code) + len(formattedComment) >= MAXWIDTH:
	return(" "*commentStart + formattedComment + "\n" + code)
    else:
	extraSpaces = commentStart - len(code)
	return(code + " "*extraSpaces + formattedComment)

def alignBlock(block, commentStart):
    outBlock = []
    for line in block.split("\n"):
	code, comment = splitComment(line)
	if code:
	    if comment:
		outBlock.append(alignComment(code, comment, commentStart))
	    else:
		outBlock.append(code)
	else: # just comment, right justify
	    outBlock.append(rightJustify(makeComment(comment)))
    return(outBlock)    

def autoAlignBlock(block):
    '''Implement the simplest max/max rule?'''
    codeLengths = []
    commentLengths = []
    for line in block.split("\n"):
	codeL, commentL = findLengths(line)
	if codeL > 0 and commentL > 0:  
	    # only worry about lines with both code and comments
	    codeLengths.append(codeL)
	    commentLengths.append(commentL)
    # drop "long" comments until they fit -- these will get wrapped 
    while(max(codeLengths) + max(commentLengths) + 6 > MAXWIDTH):
	i = commentLengths.index(max(commentLengths))
	commentLengths.pop(i)
    commentStart = MAXWIDTH - max(commentLengths) - 6
    return(alignBlock(block,commentStart))

def rightJustifyFile(filename, backup=".bak"):
    shutil.copy(filename, filename+backup)
    outString = ""
    outfile = open(filename, "w")
    infile = open(filename+backup)
    for line in infile.readlines():
	line = line.strip("\n")
	outfile.write(rightJustifyComments(line) + "\n")
    return(outString)


if __name__ == "__main__":
    testfile = "opticalTheremin.c"
    MAXWIDTH = 73

    print rightJustifyFile(testfile)


    if False:
	blocks = breakIntoBlocks(open(testfile).read())
	for block in blocks:
	    try:
		block = autoAlignBlock(block)
		print "\n".join(block)
	    except:
		print block
	    print "\n".join(block)
	    print

	
