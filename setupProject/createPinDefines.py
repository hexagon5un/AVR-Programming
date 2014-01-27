#! /usr/bin/env python

## Given a list of names and pin defines, runs through a routine that
## sets up a standard set of macros which can then be cut/paste or
## otherwise included.

import sys
try:
    outputFilename = sys.argv[1]
except:
    print
    print 'No filename passed, saving to "pinDefinitions.h" in this directory.'
    outputFilename = "pinDefinitions.h"

print
print
print "First we need to define macro names and the coresponding AVR pins."
print "None of the names are case-sensitive -- all are converted to uppercase."
print "When you're done, hit [enter] for the macro name."
print " "


nicknames = []
pinouts = []
while(True):
    nickname = raw_input("\nPin Macro Name: ").strip().upper()
    if (nickname == ""):
        break
    nicknames.append(nickname)
    pinout = raw_input("AVR Pinout: ").strip().upper()
    pinouts.append(pinout)
    
output =  "// ---------------\n//   Pin Defines  \n// ---------------\n\n"
for (nickname, pinout) in zip(nicknames, pinouts):
    output += "#define %s                     %s\n" % (nickname, pinout)
    output += "#define %s_PORT                PORT%s\n" % (nickname, pinout[1])
    output += "#define %s_PIN                 PIN%s\n" % (nickname, pinout[1])
    output += "#define %s_DDR                 DDR%s\n" % (nickname, pinout[1])
    output += "#define %s_SET_OUTPUT          %s_DDR |= _BV(%s)\n" %\
        (nickname, nickname, nickname)
    output += "#define %s_SET_INPUT           %s_DDR &= ~_BV(%s)\n" %\
        (nickname, nickname, nickname)
    output += "#define %s_SET_HIGH            %s_PORT |= _BV(%s)\n" % \
        (nickname, nickname, nickname)
    output += "#define %s_SET_LOW             %s_PORT &= ~_BV(%s)\n" % \
        (nickname, nickname, nickname)
    output += "#define %s_TOGGLE              %s_PORT ^= _BV(%s)\n" % \
        (nickname, nickname, nickname)    
    output += "#define %s_IS_HIGH             (%s_PIN & _BV(%s))\n" % \
        (nickname, nickname, nickname)
    output += "#define %s_IS_LOW              !(%s_PIN & _BV(%s))\n" % \
        (nickname, nickname, nickname)        
    output += "\n"

    
try:
    outfile = open(outputFilename, "w")
    outfile.write(output)
    outfile.close()
except NameError:
    print output
