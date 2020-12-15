import os
import sys
import filecmp
# 这不是一个严格意义上的单元测试，但如果测试通过表示我们的汇编器工作正常
# 运行方法：需要在本目录下运行，夹带了很多不优雅的shell程序


def cmp_str(s1, s2):
    for i in range(len(s1)):
        if s1[i] != s2[i]:
            print(f'{i//2:#x}: {s1[i]} - {s2[i]}', end='\t')


flag = True
fname_list = []
for _, _, filenames in os.walk(r'./test_yo/'):
    fname_list = filenames
fname_list = [s.split('.')[0] for s in fname_list if '.' in s]
# 下面利用我们的汇编器生成一个my_test_yo文件夹
try:
    os.system("mkdir my_test_yo")
    for f in fname_list:
        os.system(f"python assembler.py test_ys/{f}.ys > my_test_yo/{f}.yo")
    for f in fname_list:
        os.system(f"python striper.py test_yo/{f}.yo > temp1")
        os.system(f"python striper.py my_test_yo/{f}.yo > temp2")
        if not filecmp.cmp("temp1", "temp2"):
            flag = False
            print(f"In file {f}")
            f1 = open("temp1")
            f2 = open("temp2")
            cmp_str(f1.read(), f2.read())
            f1.close()
            f2.close()
            print()
finally:
    os.system("rm -r my_test_yo")
    os.system("rm temp1 temp2")
if flag:
    print("PASS")
