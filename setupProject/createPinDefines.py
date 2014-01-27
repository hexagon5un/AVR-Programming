#! /usr/bin/env python

## Given a list of names and pin defines, runs through a routine that
## sets up a standard set of macros which can then be cut/paste or
## otherwise included.

import sys
try:
    outputFilename = sys.argv[1]
except:
    print "No filename passed, printing results out to screen."

nicknames = []
pinouts = []
while(True):
    nickname = raw_input("\nNickname: ").strip().upper()
    if (nickname == ""):
        break
    nicknames.append(nickname)
    pinout = raw_input("Pinout: ").strip().upper()
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
