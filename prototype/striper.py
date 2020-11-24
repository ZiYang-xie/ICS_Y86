import sys
#TODO: 这里是一个不完整的striper，无法根据地址来补0
if len(sys.argv)!=2:
    raise ValueError("You can only input 1 file at a time")

file_name = sys.argv[1]

with open(file_name) as f:
    for line in f.readlines():
        left = line.split(sep='|')[0].split(":")
        if len(left)==2:
            print(left[1].strip(),end='')

