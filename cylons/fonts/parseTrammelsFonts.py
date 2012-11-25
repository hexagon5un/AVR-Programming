


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

import string
uppercase = dict([[x, encodedBytes[x]] for x in string.letters[26:]])

up = open("uppercase.h", "write")
up.write("An uppercase alphabet in 5x8\n\n\n")
up.write("const uint8_t uppercase[26][5] = {\n")
for letter in uppercase:
    up.write("\t#  {}\n".format(letter))
    up.write('\t{')
    up.write("{x[0]}, {x[1]}, {x[2]}, {x[3]}, {x[4]}".format(x=uppercase[letter]))
    up.write("},\n")
up.write("};\n\n")
up.close()







