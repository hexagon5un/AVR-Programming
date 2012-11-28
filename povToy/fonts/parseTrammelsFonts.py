
## Reads the font file from Trammell Hudson's CHDK work

FNAME = "font-small.in"
rawFont = open(FNAME)

def __stripLetter(letterText):
    ## This just takes the fonts out of the format I got them in
    # all padded with one extra space to left, two on top or bottom
    letterText = letterText[2:-3]
    # make 5 binary bytes columnwise
    letterText = [row[1:6] for row in letterText]
    return(letterText)

def rawLetterToBinary(letterText):
    '''Convert to column-wise binary'''
    letterBytes = ['0b'] * len(letterText[0])
    for row in letterText:
        for columnNum, cell in enumerate(row):
            if cell == "#":
                letterBytes[columnNum] += "1"
            else:
                letterBytes[columnNum] += "0"
    ## Flip upside-down for legibility in .h file
    letterBytes.reverse()
    return(letterBytes)
 
def writeFontHeader(fileName, whichLetters, encodedBytes):
    font = open(fileName + ".h", "write")
    font.write("// A font alphabet  \n\n\n")
    font.write("const uint8_t ")
    font.write(fileName)
    font.write("[{}][{}] = ".format(len(whichLetters), len(encodedBytes[whichLetters[0]])))
    font.write("{\n")
    for letter in whichLetters:
        font.write("\t//  {}  -- ASCII {}\n".format(letter, ord(letter)))
        font.write('{ ')
        for byte in encodedBytes[letter]:
            font.write("{},\n  ".format(byte))
        font.write("},\n")
    font.write("};\n\n")
    font.close()


if __name__ == "__main__":

    ## Read in file letter by letter
    rawLetters = {}
    for line in rawFont:
        if "=" in line:             # character definition
            currentLetter = line[0]
            rawLetters[currentLetter] = []
        else:
            rawLetters[currentLetter].append(line.strip("\n"))

    ## Strip out extra padding, empty characters
    for letter in rawLetters:
        rawLetters[letter] = __stripLetter(rawLetters[letter])
        
    ## Convert text format to binary representation
    encodedLetters = {}
    for letter in rawLetters:
        encodedLetters[letter] = rawLetterToBinary(rawLetters[letter])

    ## Sorting puts them in ASCII order
    writeFontHeader("thinFont", sorted(encodedLetters.keys()), encodedLetters)

    import string
    writeFontHeader("uppercase", sorted(list(string.letters[26:])), encodedLetters)
    writeFontHeader("lowercase", sorted(list(string.letters[:26])), encodedLetters)
