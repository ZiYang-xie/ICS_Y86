# 用以处理ans文本
import os

path1 = "../test/y86_code/test_yo"
path2 = "../test/y86_code/test_ans"

''' 获取指定目录下的所有.yo后缀的文件名 '''
def getFileName(path):
    file_list = []
    f_list = os.listdir(path)
    for i in f_list:
        if os.path.splitext(i)[1] == '.yo':
            file_list.append([i, path + "/" + i])
    return file_list


''' 
接口说明: 
    输入 file_list, 指定需要flash_code的文件名
    返回指定yo文件的 flash_code 
'''
def getFlashCode(file_list, obj_file_name):
    for file_name in file_list:
        if(file_name[0] == obj_file_name):
            res = ""
            with open(file_name[1]) as f:
                for line in f.readlines():
                    left = line.split(sep='|')[0].split(":")
                    if len(left) == 2:
                        res += "0"*(2*int(left[0].strip(),16)-len(res))
                        res += left[1].strip()
            return res

''' 
接口说明: 
    输入 ans_file_list 进行解析生成格式化的所有答案列表 format_ans_list
'''
def processText(file_list):
    format_ans_list =[]
    for file_name in file_list:
        with open(file_name[1]) as f:
            code_list = []
            tmp_list = [file_name[0]]
            key_list = ["Status", "%rax", "%rbx", "%rcx", "%rdx", "%rbp", "%rsp", "%rsi", "%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14"]
            dict_ = {"Status":"State", "%rax":"RAX", "%rbx":"RBX", "%rcx":"RCX", "%rdx":"RDX", "%rbp":"RBP", "%rsp":"RSP", "%rsi":"RSI", "%rdi":"RDI", "%r8":"R8", "%r9":"R9", "%r10":"R10", "%r11":"R11", "%r12":"R12", "%r13":"R13", "%r14":"R14"}
            for line in f.readlines():
                for key in key_list:
                    key_len = len(key)
                    if(line[:key_len].strip() == key):
                        if(key == "Status"):
                            code_list.append(["State", line[8:].strip()])
                        else:
                            code_list.append([dict_[key], line[key_len+1:key_len+20].strip(), line[key_len+20:].strip()])
            tmp_list.append(code_list)
            format_ans_list.append(tmp_list)
    return format_ans_list

''' 
接口说明: 
    输入 format_ans_list 和 code_file_list (用以获取flashcode) 以及生成文件路径
    将格式化的 unitest 写入目标文件
'''
def writePy(overall_list, code_file_list, write_path):
    fileObject = open(write_path, 'w')
    for file in overall_list:
        debugState = "False"
        func_name = "test_" + file[0][:-3].replace('-','_')
        fileObject.write("TEST_F(RealCodeTest, " + func_name + ")\n{\n")
        fileObject.write("\tc.FlashCode('" + getFlashCode(code_file_list, file[0]) + "');\n")
        fileObject.write("\tc.RUN();\n")
        assertHead = "ASSERT_EQ("
        list_ = ["State", "RAX", "RBX", "RCX", "RDX", "RBP", "RSP", "RSI", "RDI", "R8", "R9", "R10", "R11", "R12", "R13", "R14"]
        for code_res in file[1]:
            for item in list_:
                if(code_res[0] == item):
                    if(item == "State"):
                        fileObject.write("\t" + assertHead + "c.d.Stat, c.d.S" + code_res[1] + ");\n")
                    else:
                        fileObject.write("\t" + assertHead + "c.d.Reg[" + item + "], " + code_res[2] + ");\n")
        fileObject.write("}\n\n")
            

if __name__ == "__main__":

    write_path = "./generated"
    ans_file_list = getFileName(path2)
    format_ans_list = processText(ans_file_list)

    code_file_list = getFileName(path1)
    writePy(format_ans_list, code_file_list, write_path)