import sys
import os
path = "../test/y86-code/test_yo"

def getFileName(path):
    file_list = []
    ''' 获取指定目录下的所有.yo后缀的文件名 '''
    f_list = os.listdir(path)
    for i in f_list:
        if os.path.splitext(i)[1] == '.yo':
            file_list.append([i, path + "/" + i])
    return file_list

def getFlashCode(file_list):
    res_list = []
    for file_name in file_list:
        res = ""
        with open(file_name[1]) as f:
            for line in f.readlines():
                left = line.split(sep='|')[0].split(":")
                if len(left) == 2:
                    res += "0"*(2*int(left[0].strip(),16)-len(res))
                    res += left[1].strip()
        res_list.append([file_name[0], res])
    return res_list

def writeText(res_list):
    fileObject = open(path + '/raw_code', 'w')
    for code in res_list:
        fileObject.write(code[0])
        fileObject.write(': ')
        fileObject.write(code[1])
        fileObject.write('\n')


if __name__ == "__main__":
    file_list = getFileName(path)
    res_list = getFlashCode(file_list)
    writeText(res_list)
    