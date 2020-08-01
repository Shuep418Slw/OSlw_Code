

/*
记录性文件（开始于0.9）：
0.9版本 2018/02/27
屏蔽删去大部分不需要的内容
但不改变文件结构

添加精简模式等级（1-4）以及其相关功能
添加精简模式调度方式
（可以在MSP430G2452上完成移植）
具体见《OSlw精简模式说明.doc》

完善快速内存分配机制


0.91版本 2018/06、01
修改了内存分配相关bug
random部分修改了RMIN与RMAX位置 现在使用前小后大

0.92版本 2018/09/01
增加strdbg功能
增加CNN

0.93版本 2019/01/26
修改了层APPEND函数的return
修改了神经网络输入输出数据模式
增加了RNN GRURNN
修改了IQ类型运算的bug

0.94版本 2019/02/16
修改了RNNCELL的模型 可以支持更加丰富的RNNcell
增加了LayerNorm,并且可以与RNN（包括GRU）相结合，也可单独用在其他层
增加了shift层（不包括追加函数），可以用于替换训练好的BatchNorm
修改了部分全连接层追加函数代码，降低代码耦合性

增加stepfun模式，使OS可以嵌入其他运行
修改rest为nap
修改OSlwTic与Toc的测试方式


0.95版本 2019/04/01
修改矩阵部分部分代码 在树莓派zerow上提高一倍效率

增加split层，用以分割数据（可反向传递，没有append函数）
增加mix层，用以混合(相加)数据（可反向传递，没有append函数）
增加pad层，已经实现constant模式（可反向传递，有append函数）
增加extend层，已经实现nearest模式（可反向传递，有append函数）

修改shift层的new函数，shift层的权重与输入维度独立

修改CNN的append函数，可以设定VALID之外的模式（VALID之外模式尚未实现）

增加梯度（修改量）限幅功能

修改sigmoid与tanh函数实现方法 采用三次样条插值替代 提高速度

增加快速exp函数实现方法(包含256点低精度与1024点中等精度)


0.96版本 2019/04/12
修改了部分内置卷积代码 提高了20%的效率（树莓派zerow-BCM2835测得）

支持CBLAS库，可以在支持CBLAS的平台上提高性能（主要针对树莓派等）

增加了CNN的im2col模式，仅限于前向传递，可以使用im2col+矩阵运算库大幅度提高CNN前向传递性能

im2col模式下支持局部展开，可以在嵌入式设备上减小内存消耗

修改了部分bug

0.97版本 2020/07/13
提升了内核调度性能，修正了任务不能超过16的bug
修改了数学底层代码 增加vect系列宏定义函数 作为数学函数底层与cblas兼容
矩阵数学中，增加了SVD分解（仅仅float和double支持）,高斯消元法解方程，追赶法解方程，矩阵求逆 通过浮点测试
修改了control包中的代码 去除了原始的参数机制，引入了全新的代码包括2~4阶IIR系统，PID,sogi，pr等
全面提升signal代码，加入了FIR滤波器 RT形态学滤波 3次样条插值，插值迭代器，FFT(最高支持65536点 可通过宏定义修改最大支持点数 以减小内存使用)
DWT(完成单次离散小波)，EMD（原始版）VMD(原始版本)
提供python转化代码，可以将所有文件压缩成2个文件增加易用性 代码在最下面

*/

#if 0
import os
import re

'''
file must be:
-this_code.py
-oslw_inc
|-OSLW_include.h
|-OSLW_define.h
|-...
-oslw_ruc
|-OSLW_memory.c
|-OSLW_gift.h
|-...
'''

s = os.getcwd()
print(s)


h_dir = s + "\oslw_inc\\"
inc_list = os.listdir(h_dir)
print(inc_list)


myinc_list = ['OSLW_include.h', 'OSLW_define.h', 'OSLW_tool_define.h', 'OSLW_memory.h', 'OSLW_tool_basic.h',
              'OSLW_tool_vector_math.h', 'OSLW_tool_basic_math.h', 'OSLW_tool_string.h', 'OSLW_tool_random.h',
              'OSLW_tool_NN.h', 'OSLW_tool_RL.h', 'OSLW_gift.h', 'OSLW_task.h', 'OSLW_timer.h']


for i in inc_list:
    try:
        myinc_list.index(i)
    except:
        if (i == 'OSLW_core.h'):
            print(i)
        else:
            myinc_list.append(i)
            print(i)

myinc_list.append('OSLW_core.h')
print(myinc_list)

s_dir = s + "\oslw_src\\"
src_list = os.listdir(s_dir)
print(src_list)


def getLineNum(f):
    t = f.read()
    count = t.count('\n')
    return count


def Delete_note1(s):
    code_content_temp = re.sub(r'((?<=\n)|^)[ \t]*\/\*.*?\*\/\n?|\/\*.*?\*\/|((?<=\n)|^)[ \t]*\/\/[^\n]*\n|\/\/[^\n]*',
                               '', s)
    code_content_temp = re.sub(r'(?<!/)/\*([^*/]|\*(?!/)|/(?<!\*))*((?=\*/))(\*/)',
                               '', code_content_temp)
    code_content_temp = re.sub(
        r'((?<=\n)|^)[ \t]*\/\*.*?\*\/\n?|\/\*.*?\*\/|((?<=\n)|^)[ \t]*#include \"[^\n]*\n|#include \"[^\n]*',
        '', code_content_temp)
    return code_content_temp


Count = 0
IncStr = ""

for n in myinc_list[0:]:
    temp1 = h_dir + n
    print(temp1)
    try:
        f = open(temp1, 'r', encoding='UTF-8')
        temp1 = f.read()
        temp1 = Delete_note1(temp1)
        IncStr += "\n" + temp1
        Count += len(f.readlines())
        f.close()
    except:
        f.close()
        f = open(temp1, 'r', encoding='GBK')
        temp1 = f.read()
        temp1 = Delete_note1(temp1)
        IncStr += "\n" + temp1
        Count += len(f.readlines())
        f.close()


SrcStr = ""
for n in src_list[0:]:
    temp1 = s_dir + n
    print(temp1)
    try:
        f = open(temp1, 'r', encoding='UTF-8')
        temp1 = f.read()
        temp1 = Delete_note1(temp1)
        SrcStr += "\n" + temp1
        Count += len(f.readlines())
        f.close()
    except:
        f.close()
        f = open(temp1, 'r', encoding='GBK')
        temp1 = f.read()
        temp1 = Delete_note1(temp1)
        SrcStr += "\n" + temp1
        Count += len(f.readlines())
        f.close()


SrcStr = "#include \"OSLW_inc_min.h\"\n\n" + SrcStr
SrcStr = "#ifdef __cplusplus\nextern \"C\" {\n#endif\n" + SrcStr + "\n#ifdef __cplusplus\n}\n#endif\n"
SrcStr = "#ifndef OSLW_SRC_MIN_C_\n#define OSLW_SRC_MIN_C_\n\n" + SrcStr + "\n#endif\n\n"


IncStr = "#ifdef __cplusplus\nextern \"C\" {\n#endif\n" + IncStr + "\n#ifdef __cplusplus\n}\n#endif\n"
IncStr = "#ifndef OSLW_INC_MIN_H_\n#define OSLW_INC_MIN_H_\n\n" + IncStr + "\n#endif\n\n"


f = open("OSLW_inc_min.h", "w+", encoding='ascii')
IncStr = IncStr.encode('ascii', 'ignore').decode("ascii")
f.write(IncStr)
f.close()

f = open("OSLW_inc_min.h", "r", encoding='ascii')
print(getLineNum(f))
f.close()

f = open("OSLW_src_min.c", "w+", encoding='ascii')
SrcStr = SrcStr.encode('ascii', 'ignore').decode("ascii")
f.write(SrcStr)
f.close()

f = open("OSLW_src_min.c", "r", encoding='ascii')
print(getLineNum(f))
f.close()

#endif