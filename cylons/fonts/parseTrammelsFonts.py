
## Reads the font file from Trammell Hudson's CHDK work

FNAME = "font-small.in"
rawFont = open(FNAME)

letters = {}
letterBytes = None
currentLetter = None

def columnsToBytes(letter):
    # all padded with one extra space to left, two on top or bottom
    letter = letter[2:-3]
    letter = [row[1:6] for row in letter]
    # make 5 binary bytes columnwise
    letterBytes = []
    for column in range(0,5):
        columnByte = 0
        for bit in range(0,8):
            if letter[bit][column] == "#":
                columnByte += 2**bit
        letterBytes.append(columnByte)
    return(letterBytes)


for line in rawFont:
    if line.strip().endswith("="):
        # Save out whatever we've got so far
        if currentLetter:
            letters[currentLetter] = letterBytes

        # And on to the next one
        currentLetter = line[0] 
        letters[currentLetter] = []
        letterBytes = []
    else:
        letterBytes.append(line)

letters.pop("~")
encodedBytes = dict([[letter, columnsToBytes(letters[letter])] for letter in letters])

def writeFontHeader(fileName, whichLetters, encodedBytes):
    whichLetters.sort()
    font = open(fileName + ".h", "write")
    font.write("// A font alphabet in 5x8\n \n\n\n")
    font.write("const uint8_t ")
    font.write(fileName)
    font.write("[{}][{}] = ".format(len(whichLetters), len(encodedBytes[whichLetters[0]])))
    font.write("{\n")
    for letter in whichLetters:
        font.write("\t//  {}  -- ASCII {}\n".format(letter, ord(letter)))
        font.write('\t{')
        font.write("{x[0]}, {x[1]}, {x[2]}, {x[3]}, {x[4]}".format(x=encodedBytes[letter]))
        font.write("},\n")
    font.write("};\n\n")
    font.close()


writeFontHeader("thinFont", encodedBytes.keys(), encodedBytes)

import string
writeFontHeader("uppercase", list(string.letters[26:]), encodedBytes)
writeFontHeader("lowercase", list(string.letters[:26]), encodedBytes)
