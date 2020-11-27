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
        status_list = []
        rax_list = []
        rbx_list = []
        rcx_list = []
        rdx_list = []
        rbp_list = []
        rsp_list = []
        rsi_list = []
        rdi_list = []
        r8_list = []
        r9_list = []
        r10_list = []
        with open(file_name[1]) as f:
            for line in f.readlines():
                if(line[:6] == "Status"):
                    status_list.append(line[8:].strip())
                if(line[:4] == "%rax"):
                    rax_list.append([int(line[6:24].strip(), 16), int(line[24:].strip(), 16)])
                if(line[:4] == "%rbx"):
                    rbx_list.append([int(line[6:24].strip(), 16), int(line[24:].strip(), 16)])
                if(line[:4] == "%rcx"):
                    rcx_list.append([int(line[6:24].strip(), 16), int(line[24:].strip(), 16)])
                if(line[:4] == "%rbp"):
                    rbp_list.append([int(line[6:24].strip(), 16), int(line[24:].strip(), 16)])
                if(line[:4] == "%rsp"):
                    rsp_list.append([int(line[6:24].strip(), 16), int(line[24:].strip(), 16)])
                if(line[:4] == "%rsi"):
                    rsi_list.append([int(line[6:24].strip(), 16), int(line[24:].strip(), 16)])
                if(line[:4] == "%rdi"):
                    rdi_list.append([int(line[6:24].strip(), 16), int(line[24:].strip(), 16)])
                if(line[:3] == "%r8"):
                    r8_list.append([int(line[5:23].strip(), 16), int(line[23:].strip(), 16)])
                if(line[:3] == "%r9"):
                    r9_list.append([int(line[5:23].strip(), 16), int(line[23:].strip(), 16)])
                if(line[:4] == "%r10"):
                    r10_list.append([int(line[6:24].strip(), 16), int(line[24:].strip(), 16)])
        overall_list.append([status_list, rax_list, rbx_list, rcx_list, rdx_list, rbp_list, rsp_list, rsi_list, rdi_list, r8_list, r9_list, r10_list])
    return overall_list
                

if __name__ == "__main__":
    file_list = getFileName(path)
    overall = processText(file_list)
    print(len(overall))