import sys
if len(sys.argv) != 2:
    raise ValueError("You can only input 1 file at a time")

file_name = sys.argv[1]
if file_name[-3:] != '.yo':
    print("Usage: python3 striper.py file.yo")
    exit()
res = ""
with open(file_name) as f:
    for line in f.readlines():
        left = line.split(sep='|')[0].split(":")
        if len(left) == 2:
            res += "0"*(2*int(left[0].strip(), 16)-len(res))
            res += left[1].strip()
print(res)
