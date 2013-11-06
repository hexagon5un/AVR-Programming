#! /usr/bin/env python
import os
import re
import sys
import shutil

commentMatcherC   = re.compile('/\*(.*?)\*/')

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

def rightJustifyFile(filename, backup=".bak"):
    shutil.copy(filename, filename+backup)
    outfile = open(filename, "w")
    infile = open(filename+backup)
    for line in infile.readlines():
	line = line.strip("\n")
	justified = rightJustifyComments(line) 
	outfile.write(justified + "\n")
	print(justified)
	if not "\n" in justified and len(justified) > 73:
	    raise Exception("line too long:\n"+justified)

if __name__ == "__main__":
    
    MAXWIDTH = 73
    try:
	filename = sys.argv[1]
    except:
	raise Exception("No filename passed, foolio!")
    rightJustifyFile(filename)


	
