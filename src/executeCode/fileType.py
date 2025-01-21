import sys
fileName = sys.argv[1]
isPeriod = 0
fileType = ""
i = 0

for c in fileName:
    if(isPeriod == 1):
        fileType += c
    if(c == '.' and i != 0 and fileName[i+1] != '/'):
        isPeriod = 1
    i += 1

print(fileType)
