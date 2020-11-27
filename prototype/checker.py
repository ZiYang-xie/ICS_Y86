# 用以处理ans文本
import os

path = "../test/y86_code/test_ans"

def getFileName(path):
    file_list = []
    ''' 获取指定目录下的所有.yo后缀的文件名 '''
    f_list = os.listdir(path)
    for i in f_list:
        if os.path.splitext(i)[1] == '.yo':
            file_list.append([i, path + "/" + i])
    return file_list

def processText(file_list):

    overall_list =[]
    for file_name in file_list:
        with open(file_name[1]) as f:
            tmp_list = [file_name[0]]
            for line in f.readlines():
                if(line[:6] == "Status"):
                    tmp_list.append(line[8:].strip())
                if(line[:4] == "%rax"):
                    tmp_list.append(["%rax", int(line[6:24].strip(), 16), int(line[24:].strip(), 16)])
                if(line[:4] == "%rbx"):
                    tmp_list.append(["%rbx", int(line[6:24].strip(), 16), int(line[24:].strip(), 16)])
                if(line[:4] == "%rcx"):
                    tmp_list.append(["%rcx", int(line[6:24].strip(), 16), int(line[24:].strip(), 16)])
                if(line[:4] == "%rbp"):
                    tmp_list.append(["%rbp", int(line[6:24].strip(), 16), int(line[24:].strip(), 16)])
                if(line[:4] == "%rsp"):
                    tmp_list.append(["%rsp", int(line[6:24].strip(), 16), int(line[24:].strip(), 16)])
                if(line[:4] == "%rsi"):
                    tmp_list.append(["%rsi", int(line[6:24].strip(), 16), int(line[24:].strip(), 16)])
                if(line[:4] == "%rdi"):
                    tmp_list.append(["%rdi", int(line[6:24].strip(), 16), int(line[24:].strip(), 16)])
                if(line[:3] == "%r8"):
                    tmp_list.append(["%r8", int(line[5:23].strip(), 16), int(line[23:].strip(), 16)])
                if(line[:3] == "%r9"):
                    tmp_list.append(["%r9", int(line[5:23].strip(), 16), int(line[23:].strip(), 16)])
                if(line[:4] == "%r10"):
                    tmp_list.append(["%r10", int(line[6:24].strip(), 16), int(line[24:].strip(), 16)])
            overall_list.append(tmp_list)
    return overall_list


if __name__ == "__main__":
    file_list = getFileName(path)
    overall = processText(file_list)
    for n in range(0, 20):
        print(overall[n])