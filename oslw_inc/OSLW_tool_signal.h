#ifndef OSLW_TOOL_SIGNAL_H_
#define OSLW_TOOL_SIGNAL_H_
#include "OSLW_define.h"
#include "OSLW_tool_basic.h"
#include "OSLW_tool_basic_math.h"
#include "OSLW_tool_random.h"
#include "OSLW_memory.h"

#if !(OSLW_SIMPLE_LEVEL >= 3)

typedef struct OSLW_TOOL_SIGNAL_FIR_STRUCT {
	lw_u32 Order;
	ParaType *P_Num;
	ParaType *M_x;
}OSlwToolSignalFirSTU;

typedef OSlwToolSignalFirSTU OSlwSigFir;
typedef OSlwToolSignalFirSTU LwFir;

#define LwFir_Default(ORDER,NUM,X) {(ORDER),(NUM),(X)}

ParaType OSlwToolSignalFirRun(OSlwToolSignalFirSTU *pfir, ParaType xin);
#define LwRunFir(fir,xin) OSlwToolSignalFirRun(fir,xin)

//pfir为NUL就会被分配
void *OSlwToolSignalFirNew(OSlwToolSignalFirSTU *pfir, lw_u32 order, ParaType *num, OSlwMemoryBasicSTU *pmem);
#define LwNewFir(fir,order,num,pmem) OSlwToolSignalFirNew(fir,order,num,pmem)



typedef struct OSLW_TOOL_SIGNAL_OC_FILTER1D_STRUCT {
	lw_u32 Order;
	ParaType *graph;//Order长度
	ParaType *reg1;//2*Order长度
	ParaType *reg2;//2*Order长度
	ParaType *reg3;//2*Order长度
	ParaType *reg4;//2*Order长度
	ParaType *data_buf;//Order长度
}OSlwToolSignalOCFilterSTU;

typedef OSlwToolSignalOCFilterSTU OSlwSigOCF;
typedef OSlwToolSignalOCFilterSTU LwOCF;

ParaType OSlwToolSignalOCFilter1dRun(OSlwToolSignalOCFilterSTU *pfir, ParaType xin);
#define LwRunOCF(filter,xin) OSlwToolSignalOCFilter1dRun(filter,xin)

void *OSlwToolSignalOCFilterNew(OSlwToolSignalOCFilterSTU *f, lw_u32 order, ParaType *graph, OSlwMemoryBasicSTU *pmem);
#define LwNewOCF(pfir,order1,num1,pmem) OSlwToolSignalOCFilterNew(pfir,order1,num1,pmem)


typedef struct OSLW_TOOL_SIGNAL_INTERP_BASIC_STRUCT {
	ParaType *coff;
	ParaType *x_point;
	LwMatLenType Len;//一共多少区间 如果是3个断点 就是4
	ParaType(*Interp)(struct OSLW_TOOL_SIGNAL_INTERP_BASIC_STRUCT *p, ParaType xnow);
	ParaType(*_interp_with_zoom)(struct OSLW_TOOL_SIGNAL_INTERP_BASIC_STRUCT *p, ParaType xnow,LwMatLenType now_zoom);
	LwMatLenType(*_interp_find)(struct OSLW_TOOL_SIGNAL_INTERP_BASIC_STRUCT *p, ParaType xnow);
}OSlwToolSignalInterpBasicSTU;

LwMatLenType OSlwToolSignalInterpFind(OSlwToolSignalInterpBasicSTU *p,ParaType xnow);
ParaType OSlwToolSignalInterpRun(OSlwToolSignalInterpBasicSTU *p, ParaType xnow);
#define LwInterpGet(INTERPI,XNOW) OSlwToolSignalInterpRun((OSlwToolSignalInterpIterSTU *)(INTERPI),(XNOW))

//----------------------------
//插值迭代器 使用LwInterpIterDefault初始化 利用LwInterpIterGet得到插值数值
//----------------------------
typedef struct OSLW_TOOL_SIGNAL_INTERP_ITERATION_STRUCT {
	OSlwToolSignalInterpBasicSTU *pInterp;
	LwMatLenType move_now;
}OSlwToolSignalInterpIterSTU;

typedef OSlwToolSignalInterpIterSTU LwInterpIter;

#define LwInterpIterDefault(INTERP,NOW_ZOOM) {(OSlwToolSignalInterpBasicSTU *)(INTERP),(LwMatLenType)(NOW_ZOOM)}
ParaType OSlwToolSignalInterpIterGet(OSlwToolSignalInterpIterSTU *p, ParaType xnow);
#define LwInterpIterGet(INTERPI,XNOW) OSlwToolSignalInterpIterGet((OSlwToolSignalInterpIterSTU *)(INTERPI),(XNOW))


typedef struct OSLW_TOOL_SIGNAL_INTERP_SPLINE_STRUCT {
	OSlwToolSignalInterpBasicSTU interp1_basic;
	ParaType dy0, dyn;
}OSlwToolSignalSplineSTU;

ParaType OSlwToolSignalSplineInterpZoom(OSlwToolSignalInterpBasicSTU *p, ParaType xnow, LwMatLenType now_zoom);


//------------------------------------------
//<函数名>OSlwToolSignalSplineInitStatic</函数名>
//<功能说明>样条插值静态初始化</功能说明>
//<输入说明>p:样条插值结构体
//xin:断点
//yout:函数值
//x_y_len:输入信号长度
//dy:两端微分 可以为NULL 为空代表自然边界
//spline_coff 保存因子 长度4*(x_y_len+!)
// Da crout法的主对角线 长度x_y_len+1
// La crout法的下对角线 长度x_y_len
// Ua crout法的上对角线 长度x_y_len
// Ma crout法的X 长度 x_y_len+1
// Fa crout法的Y 长度 x_y_len+1
//</输入说明>
//<输出说明>0:成功 1:失败</输出说明>
//<备注>xin必须单调递增</备注>
lw_16 OSlwToolSignalSplineInitStatic(
	OSlwToolSignalSplineSTU *p,
	ParaType *xin, ParaType *yout, LwMatLenType x_y_len,
	ParaType *dy,
	ParaType *spline_coff,
	ParaType *Da,//crout法的主对角线
	ParaType *La,//crout法的下对角线
	ParaType *Ua,//crout法的上对角线
	ParaType *Ma,//crout法的X
	ParaType *Fa//crout法的Y
);


//------------------------------------------
//<函数名>OSlwToolSignalSplineNew</函数名>
//<功能说明>样条插值动态分配</功能说明>
//<输入说明>p:样条插值结构体 可以为NULL 为NULL会分配一个 并且申请x和y的内存空间 并复制xin与yout
//xin:断点
//yout:函数值
//x_y_len:输入信号长度
//dy:两端微分 可以为NULL 为空代表自然边界
//spline_coff:样条插值参数 可以为NULL
//mem_ptr:动态内存分配指针
//</输入说明>
//<输出说明>0:成功 1:失败</输出说明>
//<备注>xin必须单调递增</备注>
OSlwToolSignalSplineSTU *OSlwToolSignalSplineNew(
	OSlwToolSignalSplineSTU *p,
	ParaType *xin, ParaType *yout, LwMatLenType x_y_len,
	ParaType *dy, ParaType *spline_coff, lw_mem mem_ptr
);

//------------------------------------------
//<函数名>OSlwToolSignalFFT</函数名>
//<功能说明>FFT</功能说明>
//<输入说明>x:FFT数组，存放方式为实部,虚部,实部,虚部
//len:FFT长度 16,32，... 65536 可以根据OSLW_TOOL_FFT_MAX_LEN调节FFT最大点数
//BitRev_flag:是否倒叙 为1倒叙
//iFFT_flag:是否进行IFFT 为1进行
//temp:备用接口 与其他结构体兼容
//</输入说明>
//<输出说明>x的地址</输出说明>
//<备注>可使用pOSlwToolSignalFFT调用接口函数 也可采用LwFFT与LwIFFT简易调用</备注>
//------------------------------------------
typedef void *(*OSlwToolSignalFFT_Type)(ParaType *x, lw_u32 len, lw_u8 BitRev_flag, lw_u8 iFFT_flag, void *temp);
extern OSlwToolSignalFFT_Type pOSlwToolSignalFFT;
void * OSlwToolSignalFFT(ParaType *x, lw_u32 len, lw_u8 BitRev_flag, lw_u8 iFFT_flag, void *temp);

#define LwFFT(X,LEN) pOSlwToolSignalFFT(X,LEN,1,0,NULL)
#define LwIFFT(X,LEN) pOSlwToolSignalFFT(X,LEN,1,1,NULL)

//------------------------------------------
//<函数名>OSlwToolSignalFFTCompelex</函数名>
//<功能说明>扩展FFT到复数</功能说明>
//<输入说明>x_complex:复数信号 长度至少FFT_len*2
//x:实数 长度至少FFT_len
//FFT_len:FFT的长度
//</输入说明>
//<输出说明>x_complex的地址</输出说明>
//<备注>可使用x_complex与x可以相同地址</备注>
//------------------------------------------
void * OSlwToolSignalFFTCompelex(ParaType *x_complex, ParaType *x, lw_u32 FFT_len);
#define LwFFTcomplex OSlwToolSignalFFTCompelex

//------------------------------------------
//<函数名>OSlwToolSignalFFTReal</函数名>
//<功能说明>取FFT到实数</功能说明>
//<输入说明>x:实数 长度至少FFT_len
//x_complex:复数信号 长度至少FFT_len * 2
//FFT_len:FFT的长度
//</输入说明>
//<输出说明>x的地址</输出说明>
//<备注>可使用x_complex与x可以相同地址</备注>
//------------------------------------------
void * OSlwToolSignalFFTReal(ParaType *x, ParaType *x_complex, lw_u32 FFT_len);
#define LwFFTreal OSlwToolSignalFFTReal

//------------------------------------------
//<函数名>OSlwToolSignalFFTShift</函数名>
//<功能说明>fftshift</功能说明>
//<输入说明>x1:复数 长度至少FFT_len*2
//x2:复数 长度至少FFT_len*2
//FFT_len:FFT的长度
//</输入说明>
//<输出说明>x1的地址</输出说明>
//<备注>可使用x1与x2可以相同地址</备注>
//------------------------------------------
void * OSlwToolSignalFFTShift(ParaType *x1, ParaType *x2, lw_u32 FFT_len);
#define LwFFTshift OSlwToolSignalFFTShift


extern const lw_u16 OSlwToolSignalFFT_BitRevArr_16[];
extern const lw_u16 OSlwToolSignalFFT_BitRevArr_32[];
extern const lw_u16 OSlwToolSignalFFT_BitRevArr_64[];
extern const lw_u16 OSlwToolSignalFFT_BitRevArr_128[];
extern const lw_u16 OSlwToolSignalFFT_BitRevArr_256[];
extern const lw_u16 OSlwToolSignalFFT_BitRevArr_512[];
extern const lw_u16 OSlwToolSignalFFT_BitRevArr_1024[];
extern const lw_u16 OSlwToolSignalFFT_BitRevArr_2048[];
extern const lw_u16 OSlwToolSignalFFT_BitRevArr_4096[];
extern const lw_u16 OSlwToolSignalFFT_BitRevArr_8192[];
extern const lw_u16 OSlwToolSignalFFT_BitRevArr_16384[];
extern const lw_u16 OSlwToolSignalFFT_BitRevArr_32768[];
extern const lw_u16 OSlwToolSignalFFT_BitRevArr_65536[];


#if OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_16
#define OSlwToolSignalFFT_BitRevTab OSlwToolSignalFFT_BitRevArr_16
#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_32
#define OSlwToolSignalFFT_BitRevTab OSlwToolSignalFFT_BitRevArr_32
#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_64
#define OSlwToolSignalFFT_BitRevTab OSlwToolSignalFFT_BitRevArr_64
#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_128
#define OSlwToolSignalFFT_BitRevTab OSlwToolSignalFFT_BitRevArr_128
#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_256
#define OSlwToolSignalFFT_BitRevTab OSlwToolSignalFFT_BitRevArr_256
#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_512
#define OSlwToolSignalFFT_BitRevTab OSlwToolSignalFFT_BitRevArr_512
#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_1024
#define OSlwToolSignalFFT_BitRevTab OSlwToolSignalFFT_BitRevArr_1024
#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_2048
#define OSlwToolSignalFFT_BitRevTab OSlwToolSignalFFT_BitRevArr_2048
#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_4096
#define OSlwToolSignalFFT_BitRevTab OSlwToolSignalFFT_BitRevArr_4096
#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_8192
#define OSlwToolSignalFFT_BitRevTab OSlwToolSignalFFT_BitRevArr_8192
#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_16384
#define OSlwToolSignalFFT_BitRevTab OSlwToolSignalFFT_BitRevArr_16384
#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_32768
#define OSlwToolSignalFFT_BitRevTab OSlwToolSignalFFT_BitRevArr_32768
#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_65536
#define OSlwToolSignalFFT_BitRevTab OSlwToolSignalFFT_BitRevArr_65536
#endif


extern const ParaType OSlwToolSignalFFT_Tw16[];
extern const ParaType OSlwToolSignalFFT_Tw32[];
extern const ParaType OSlwToolSignalFFT_Tw64[];
extern const ParaType OSlwToolSignalFFT_Tw128[];
extern const ParaType OSlwToolSignalFFT_Tw256[];
extern const ParaType OSlwToolSignalFFT_Tw512[];
extern const ParaType OSlwToolSignalFFT_Tw1024[];
extern const ParaType OSlwToolSignalFFT_Tw2048[];
extern const ParaType OSlwToolSignalFFT_Tw4096[];
extern const ParaType OSlwToolSignalFFT_Tw8192[];
extern const ParaType OSlwToolSignalFFT_Tw16384[];
extern const ParaType OSlwToolSignalFFT_Tw32768[];
extern const ParaType OSlwToolSignalFFT_Tw65536[];


#if OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_16
#define OSlwToolSignalFFT_Coef OSlwToolSignalFFT_Tw16
#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_32
#define OSlwToolSignalFFT_Coef OSlwToolSignalFFT_Tw32
#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_64
#define OSlwToolSignalFFT_Coef OSlwToolSignalFFT_Tw64
#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_128
#define OSlwToolSignalFFT_Coef OSlwToolSignalFFT_Tw128
#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_256
#define OSlwToolSignalFFT_Coef OSlwToolSignalFFT_Tw256
#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_512
#define OSlwToolSignalFFT_Coef OSlwToolSignalFFT_Tw512
#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_1024
#define OSlwToolSignalFFT_Coef OSlwToolSignalFFT_Tw1024
#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_2048
#define OSlwToolSignalFFT_Coef OSlwToolSignalFFT_Tw2048
#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_4096
#define OSlwToolSignalFFT_Coef OSlwToolSignalFFT_Tw4096
#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_8192
#define OSlwToolSignalFFT_Coef OSlwToolSignalFFT_Tw8192
#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_16384
#define OSlwToolSignalFFT_Coef OSlwToolSignalFFT_Tw16384
#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_32768
#define OSlwToolSignalFFT_Coef OSlwToolSignalFFT_Tw32768
#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_65536
#define OSlwToolSignalFFT_Coef OSlwToolSignalFFT_Tw65536
#endif


//------------------------------------------
//<函数名>OSlwToolSignalDwtKern</函数名>
//<功能说明>单次离散小波变换</功能说明>
//<输入说明>假设输入信号长度为xin_Len
//cA:低频信号数组 长度至少(xin_Len+kern_Len-1)/2
//cD:高频信号数组 长度至少(xin_Len+kern_Len-1)/2
//xin:输入信号
//Lod:低通滤波器 长度kern_Len 注:没有采用倒置卷积的方法 matlab的参数需要倒置 下同
//Hid:高通滤波器 长度kern_Len
//xin_Len:原始信号长度
//kern_Len:内核长度
//temp_work:至少2*kern_Len+xin_len的内存空间
//</输入说明>
//<输出说明>cA</输出说明>
//<备注>采用中心对称补齐的方式,间隔下采样</备注>
//------------------------------------------
void *OSlwToolSignalDwtKern(ParaType *cA, ParaType *cD, ParaType *xin, ParaType *LoD, ParaType *HiD, lw_u32 xin_Len, lw_u32 kern_Len, ParaType *temp_work);

//------------------------------------------
//<函数名>OSlwToolSignalIDwtKern</函数名>
//<功能说明>单次离散逆小波变换</功能说明>
//<输入说明>假设输入ca cd信号长度为c_Len
//xout:输出结果 代表原始信号长度至少c_Len*2-(kern_Len-2)
//cA:低频信号数组 长度c_Len
//cD:高频信号数组 长度c_Len
//LoR:低通重构滤波器 长度kern_Len 注:没有采用倒置卷积的方法 matlab的参数需要倒置 下同
//HiR:高通重构滤波器 长度kern_Len
//c_Len:重构信号长度 不是原始信号长度
//kern_Len:内核长度
//temp_work:至少c_Len*2-(kern_Len-2)+kern_Len*2的内存空间
//</输入说明>
//<输出说明>xout</输出说明>
//<备注>采用中心对称补齐的方式 补零方式的上采样</备注>
void *OSlwToolSignalIDwtKern(ParaType *xout, ParaType *cA, ParaType *cD, ParaType *LoR, ParaType *HiR, lw_u32 c_Len, lw_u32 kern_Len, ParaType *temp_work);



//------------------------------------------
//<函数名>OSlwToolSignalEmdSift</函数名>
//<功能说明>EMD得到单次</功能说明>
//<输入说明>res_sig:输入的残差信号
//mean_val:均值信号
//signal_len:信号长度
//peak_len:极点数量 上极点+下极点
//sig_rms:均值信号均方值 残差信号均方值 
//pmem:内存分配结构体
//</输入说明>
//<输出说明>0:成功 1:极点缺失 2:极点数量不同</输出说明>
//<备注>pmem不可以是Simple的分配方法</备注>
//------------------------------------------
lw_32 OSlwToolSignalEmdSift(
	ParaType *res_sig,
	ParaType *mean_val,
	lw_32 signal_len,
	lw_32 peak_len[2],
	ParaType sig_rms[2],
	lw_mem pmem
);

//------------------------------------------
//<函数名>OSlwToolSignalEmdKern</函数名>
//<功能说明>EMD得到一个IMF函数</功能说明>
//<输入说明>
//imf:内模函数
//res_sig:输入的残差信号 运算结束会减去IMF
//signal_len:信号长度
//Max_iter:最大迭代次数
//rtol:均值RMS/残差RMS的阈值，当该数值小于rtol时结束迭代
//pmem:内存分配结构体
//</输入说明>
//<输出说明>当前的rtol 若小于0代表 极点错误</输出说明>
//<备注>pmem不可以是Simple的分配方法</备注>
//------------------------------------------
ParaType OSlwToolSignalEmdKern(
	ParaType *imf,
	ParaType *res_sig,
	lw_32 signal_len,
	lw_u32 Max_iter,
	ParaType rtol,
	lw_mem pmem
);

//------------------------------------------
//<函数名>OSlwToolSignalEmdSimple</函数名>
//<功能说明>EMD分解</功能说明>
//<输入说明>
//res 内模函数矩阵 行代表需要分解的最大次数 列代表信号长度
//signal:输入信号 运算结束就是残差
//Imf_Max_iter:IMF 最大迭代次数
//imf_rtol:均值RMS/残差RMS的阈值，当该数值小于rtol时结束迭代
//pmem:内存分配结构体
//</输入说明>
//<备注>pmem不可以是Simple的分配方法</备注>
//------------------------------------------
void OSlwToolSignalEmdSimple(
	LwMat *res,
	ParaType *signal,
	lw_u32 Imf_Max_iter,
	ParaType imf_rtol,
	lw_mem pmem
);

#define LwEmd OSlwToolSignalEmdSimple


//------------------------------------------
//<函数名>OSlwToolSignalVmdSimple</函数名>
//<功能说明>VMD分解</功能说明>
//<输入说明>
//res_mat 分解矩阵 行代表需要分解的数量 列代表信号长度 a为空就会分配
//signal:输入信号
//alpha:带宽限制 典型值2000
//tau:噪声扰动数值 让搜寻具有一定的扰动
//rtol:迭代收敛阈值 当前后频谱相差的均方根小于该数值的时候结束
//Max_loopN:最大循环迭代次数
//omega_mat:中心频率变化矩阵 用于观察迭代过程中中心频率的变化 可以为NULL 当不为NULL的时候要求 列等于分解的数量 行数代表保存中心频率的次数 a为空就会分配
//pmem:内存分配结构体
//</输入说明>
//<输出说明>最终前后频谱相差的均方根</输出说明>
//<备注>pmem不可以是Simple的分配方法</备注>
//------------------------------------------
ParaType OSlwToolSignalVmdSimple(
	LwMat *res_mat,
	ParaType *signal,
	ParaType alpha,
	ParaType tau,
	ParaType rtol,
	lw_u32 Max_loopN,
	LwMat *omega_mat,
	lw_mem pmem
);

#define LwVmd(R,S,A,T,TOL,L,M) OSlwToolSignalVmdSimple((R),(S),(A),(T),(TOL),(L),(NULL),(M))

#endif // !(OSLW_SIMPLE_LEVEL >= 3)


#endif // !OSLW_TOOL_SIGNAL_H_
