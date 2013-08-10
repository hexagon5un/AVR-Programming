
import os
import re


commentMatcherC   = re.compile('/\*(.*?)\*/')
commentMatcherCpp = re.compile('//(.*?)$')


def hasComment(string):
    if "/*" in string and "*/" in string:
	return(True)
    if "//" in string:
	return(True)
    return(False)
    
def commentLength(string):
    pass

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

def rightJustifyComments(string):
    code, comment = splitComment(string)
    extraSpaces = MAXWIDTH - sum(findLengths(string)) - 6 # delimiter chars
    if comment:
	formattedComment = "/* " + comment + " */"
	if extraSpaces > 0:
	    return(code + " "*(extraSpaces) + formattedComment)
	else: # longer than maxwidth, newline
	    return(formattedComment + "\n" + code)
    else:
	return(code)

def trimTrailingSpace(string):
    while string.endswith(" "):
	string = string[:-1]
    return(string)

def alignComment(code, comment, commentStart):
    if (len(comment) + commentStart + 6) > MAXWIDTH:
	if len(code) == 0:
	    return(" "*(MAXWIDTH-len(comment)-6) + \
		    "/* " + comment + " */")
	else:
	    raise Exception("Not enough room for comment")
    internalSpaces = MAXWIDTH - commentStart - len(comment) - 6
    formattedComment = "/* " + comment + " "*internalSpaces + " */"

    if len(code) + len(formattedComment) >= MAXWIDTH:
	return(" "*commentStart + formattedComment + "\n" + code)
    else:
	extraSpaces = commentStart - len(code)
	return(code + " "*extraSpaces + formattedComment)

def alignBlock(block, commentStart):
    outBlock = []
    for line in block.split("\n"):
	code, comment = splitComment(line)
	if comment:
	    outBlock.append(alignComment(code, comment, commentStart))
	else:
	    outBlock.append(code)
    return(outBlock)    

if __name__ == "__main__":
    testfile = "opticalTheremin.c"
    MAXWIDTH = 73
    blocks = breakIntoBlocks(open(testfile).read())
    block = blocks[7]
    print alignBlock(block, 34)

    line = block.split("\n")[-2]
    code, comment = splitComment(line)
    alignComment(code, comment, 40)


	
