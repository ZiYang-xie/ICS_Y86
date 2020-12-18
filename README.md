# ICS_2020_PJ

## 环境要求

0. MacOSX 10.15

1. Python3.7

2. Cmake3.1.7或更高

3. Apple clang version 11.0.3 

## 如何运行

### 可执行文件

#### 主程序
主程序为`./build/ICS_Y86`，运行方法为
```shell script
cd build
python3 striper.py $path_to_file_name/$file_name | ./ICS_Y86
# Example: python3 striper.py test_yo/ret-hazard.yo | ./ICS_Y86 
```
#### 测试程序(建议使用)
```shell script
cd ./test/build
./Google_Tests_run 
```

### 自行编译

#### 使用测试模块

为了方便项目的开发，我们已经将所有的y86-code（非honor部分）的测试写入了main_test.cpp，按如下步骤即可编译运行
```shell script
cd ./test/build
cmake ..
make 
./Google_Tests_run 
```
同时测试中还有我们写的针对每一条指令的测试，如果助教不嫌弃明年也可以直接给下一届的同学用(在Stage2中还会扩充)

#### 手动测试

如果希望进行手动测试，需要先要先把$filename.yo转成无注释的版本，再以stdin的形式输入
先进行编译
```shell script
cmake .
make
cd build
python3 striper.py $path_to_file_name/$file_name | ./ICS_Y86
# Example: python3 striper.py test_yo/ret-hazard.yo | ./ICS_Y86 
```





