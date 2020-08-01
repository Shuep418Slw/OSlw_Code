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

//pfirΪNUL�ͻᱻ����
void *OSlwToolSignalFirNew(OSlwToolSignalFirSTU *pfir, lw_u32 order, ParaType *num, OSlwMemoryBasicSTU *pmem);
#define LwNewFir(fir,order,num,pmem) OSlwToolSignalFirNew(fir,order,num,pmem)



typedef struct OSLW_TOOL_SIGNAL_OC_FILTER1D_STRUCT {
	lw_u32 Order;
	ParaType *graph;//Order����
	ParaType *reg1;//2*Order����
	ParaType *reg2;//2*Order����
	ParaType *reg3;//2*Order����
	ParaType *reg4;//2*Order����
	ParaType *data_buf;//Order����
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
	LwMatLenType Len;//һ���������� �����3���ϵ� ����4
	ParaType(*Interp)(struct OSLW_TOOL_SIGNAL_INTERP_BASIC_STRUCT *p, ParaType xnow);
	ParaType(*_interp_with_zoom)(struct OSLW_TOOL_SIGNAL_INTERP_BASIC_STRUCT *p, ParaType xnow,LwMatLenType now_zoom);
	LwMatLenType(*_interp_find)(struct OSLW_TOOL_SIGNAL_INTERP_BASIC_STRUCT *p, ParaType xnow);
}OSlwToolSignalInterpBasicSTU;

LwMatLenType OSlwToolSignalInterpFind(OSlwToolSignalInterpBasicSTU *p,ParaType xnow);
ParaType OSlwToolSignalInterpRun(OSlwToolSignalInterpBasicSTU *p, ParaType xnow);
#define LwInterpGet(INTERPI,XNOW) OSlwToolSignalInterpRun((OSlwToolSignalInterpIterSTU *)(INTERPI),(XNOW))

//----------------------------
//��ֵ������ ʹ��LwInterpIterDefault��ʼ�� ����LwInterpIterGet�õ���ֵ��ֵ
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
//<������>OSlwToolSignalSplineInitStatic</������>
//<����˵��>������ֵ��̬��ʼ��</����˵��>
//<����˵��>p:������ֵ�ṹ��
//xin:�ϵ�
//yout:����ֵ
//x_y_len:�����źų���
//dy:����΢�� ����ΪNULL Ϊ�մ�����Ȼ�߽�
//spline_coff �������� ����4*(x_y_len+!)
// Da crout�������Խ��� ����x_y_len+1
// La crout�����¶Խ��� ����x_y_len
// Ua crout�����϶Խ��� ����x_y_len
// Ma crout����X ���� x_y_len+1
// Fa crout����Y ���� x_y_len+1
//</����˵��>
//<���˵��>0:�ɹ� 1:ʧ��</���˵��>
//<��ע>xin���뵥������</��ע>
lw_16 OSlwToolSignalSplineInitStatic(
	OSlwToolSignalSplineSTU *p,
	ParaType *xin, ParaType *yout, LwMatLenType x_y_len,
	ParaType *dy,
	ParaType *spline_coff,
	ParaType *Da,//crout�������Խ���
	ParaType *La,//crout�����¶Խ���
	ParaType *Ua,//crout�����϶Խ���
	ParaType *Ma,//crout����X
	ParaType *Fa//crout����Y
);


//------------------------------------------
//<������>OSlwToolSignalSplineNew</������>
//<����˵��>������ֵ��̬����</����˵��>
//<����˵��>p:������ֵ�ṹ�� ����ΪNULL ΪNULL�����һ�� ��������x��y���ڴ�ռ� ������xin��yout
//xin:�ϵ�
//yout:����ֵ
//x_y_len:�����źų���
//dy:����΢�� ����ΪNULL Ϊ�մ�����Ȼ�߽�
//spline_coff:������ֵ���� ����ΪNULL
//mem_ptr:��̬�ڴ����ָ��
//</����˵��>
//<���˵��>0:�ɹ� 1:ʧ��</���˵��>
//<��ע>xin���뵥������</��ע>
OSlwToolSignalSplineSTU *OSlwToolSignalSplineNew(
	OSlwToolSignalSplineSTU *p,
	ParaType *xin, ParaType *yout, LwMatLenType x_y_len,
	ParaType *dy, ParaType *spline_coff, lw_mem mem_ptr
);

//------------------------------------------
//<������>OSlwToolSignalFFT</������>
//<����˵��>FFT</����˵��>
//<����˵��>x:FFT���飬��ŷ�ʽΪʵ��,�鲿,ʵ��,�鲿
//len:FFT���� 16,32��... 65536 ���Ը���OSLW_TOOL_FFT_MAX_LEN����FFT������
//BitRev_flag:�Ƿ��� Ϊ1����
//iFFT_flag:�Ƿ����IFFT Ϊ1����
//temp:���ýӿ� �������ṹ�����
//</����˵��>
//<���˵��>x�ĵ�ַ</���˵��>
//<��ע>��ʹ��pOSlwToolSignalFFT���ýӿں��� Ҳ�ɲ���LwFFT��LwIFFT���׵���</��ע>
//------------------------------------------
typedef void *(*OSlwToolSignalFFT_Type)(ParaType *x, lw_u32 len, lw_u8 BitRev_flag, lw_u8 iFFT_flag, void *temp);
extern OSlwToolSignalFFT_Type pOSlwToolSignalFFT;
void * OSlwToolSignalFFT(ParaType *x, lw_u32 len, lw_u8 BitRev_flag, lw_u8 iFFT_flag, void *temp);

#define LwFFT(X,LEN) pOSlwToolSignalFFT(X,LEN,1,0,NULL)
#define LwIFFT(X,LEN) pOSlwToolSignalFFT(X,LEN,1,1,NULL)

//------------------------------------------
//<������>OSlwToolSignalFFTCompelex</������>
//<����˵��>��չFFT������</����˵��>
//<����˵��>x_complex:�����ź� ��������FFT_len*2
//x:ʵ�� ��������FFT_len
//FFT_len:FFT�ĳ���
//</����˵��>
//<���˵��>x_complex�ĵ�ַ</���˵��>
//<��ע>��ʹ��x_complex��x������ͬ��ַ</��ע>
//------------------------------------------
void * OSlwToolSignalFFTCompelex(ParaType *x_complex, ParaType *x, lw_u32 FFT_len);
#define LwFFTcomplex OSlwToolSignalFFTCompelex

//------------------------------------------
//<������>OSlwToolSignalFFTReal</������>
//<����˵��>ȡFFT��ʵ��</����˵��>
//<����˵��>x:ʵ�� ��������FFT_len
//x_complex:�����ź� ��������FFT_len * 2
//FFT_len:FFT�ĳ���
//</����˵��>
//<���˵��>x�ĵ�ַ</���˵��>
//<��ע>��ʹ��x_complex��x������ͬ��ַ</��ע>
//------------------------------------------
void * OSlwToolSignalFFTReal(ParaType *x, ParaType *x_complex, lw_u32 FFT_len);
#define LwFFTreal OSlwToolSignalFFTReal

//------------------------------------------
//<������>OSlwToolSignalFFTShift</������>
//<����˵��>fftshift</����˵��>
//<����˵��>x1:���� ��������FFT_len*2
//x2:���� ��������FFT_len*2
//FFT_len:FFT�ĳ���
//</����˵��>
//<���˵��>x1�ĵ�ַ</���˵��>
//<��ע>��ʹ��x1��x2������ͬ��ַ</��ע>
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
//<������>OSlwToolSignalDwtKern</������>
//<����˵��>������ɢС���任</����˵��>
//<����˵��>���������źų���Ϊxin_Len
//cA:��Ƶ�ź����� ��������(xin_Len+kern_Len-1)/2
//cD:��Ƶ�ź����� ��������(xin_Len+kern_Len-1)/2
//xin:�����ź�
//Lod:��ͨ�˲��� ����kern_Len ע:û�в��õ��þ���ķ��� matlab�Ĳ�����Ҫ���� ��ͬ
//Hid:��ͨ�˲��� ����kern_Len
//xin_Len:ԭʼ�źų���
//kern_Len:�ں˳���
//temp_work:����2*kern_Len+xin_len���ڴ�ռ�
//</����˵��>
//<���˵��>cA</���˵��>
//<��ע>�������ĶԳƲ���ķ�ʽ,����²���</��ע>
//------------------------------------------
void *OSlwToolSignalDwtKern(ParaType *cA, ParaType *cD, ParaType *xin, ParaType *LoD, ParaType *HiD, lw_u32 xin_Len, lw_u32 kern_Len, ParaType *temp_work);

//------------------------------------------
//<������>OSlwToolSignalIDwtKern</������>
//<����˵��>������ɢ��С���任</����˵��>
//<����˵��>��������ca cd�źų���Ϊc_Len
//xout:������ ����ԭʼ�źų�������c_Len*2-(kern_Len-2)
//cA:��Ƶ�ź����� ����c_Len
//cD:��Ƶ�ź����� ����c_Len
//LoR:��ͨ�ع��˲��� ����kern_Len ע:û�в��õ��þ���ķ��� matlab�Ĳ�����Ҫ���� ��ͬ
//HiR:��ͨ�ع��˲��� ����kern_Len
//c_Len:�ع��źų��� ����ԭʼ�źų���
//kern_Len:�ں˳���
//temp_work:����c_Len*2-(kern_Len-2)+kern_Len*2���ڴ�ռ�
//</����˵��>
//<���˵��>xout</���˵��>
//<��ע>�������ĶԳƲ���ķ�ʽ ���㷽ʽ���ϲ���</��ע>
void *OSlwToolSignalIDwtKern(ParaType *xout, ParaType *cA, ParaType *cD, ParaType *LoR, ParaType *HiR, lw_u32 c_Len, lw_u32 kern_Len, ParaType *temp_work);



//------------------------------------------
//<������>OSlwToolSignalEmdSift</������>
//<����˵��>EMD�õ�����</����˵��>
//<����˵��>res_sig:����Ĳв��ź�
//mean_val:��ֵ�ź�
//signal_len:�źų���
//peak_len:�������� �ϼ���+�¼���
//sig_rms:��ֵ�źž���ֵ �в��źž���ֵ 
//pmem:�ڴ����ṹ��
//</����˵��>
//<���˵��>0:�ɹ� 1:����ȱʧ 2:����������ͬ</���˵��>
//<��ע>pmem��������Simple�ķ��䷽��</��ע>
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
//<������>OSlwToolSignalEmdKern</������>
//<����˵��>EMD�õ�һ��IMF����</����˵��>
//<����˵��>
//imf:��ģ����
//res_sig:����Ĳв��ź� ����������ȥIMF
//signal_len:�źų���
//Max_iter:����������
//rtol:��ֵRMS/�в�RMS����ֵ��������ֵС��rtolʱ��������
//pmem:�ڴ����ṹ��
//</����˵��>
//<���˵��>��ǰ��rtol ��С��0���� �������</���˵��>
//<��ע>pmem��������Simple�ķ��䷽��</��ע>
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
//<������>OSlwToolSignalEmdSimple</������>
//<����˵��>EMD�ֽ�</����˵��>
//<����˵��>
//res ��ģ�������� �д�����Ҫ�ֽ�������� �д����źų���
//signal:�����ź� ����������ǲв�
//Imf_Max_iter:IMF ����������
//imf_rtol:��ֵRMS/�в�RMS����ֵ��������ֵС��rtolʱ��������
//pmem:�ڴ����ṹ��
//</����˵��>
//<��ע>pmem��������Simple�ķ��䷽��</��ע>
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
//<������>OSlwToolSignalVmdSimple</������>
//<����˵��>VMD�ֽ�</����˵��>
//<����˵��>
//res_mat �ֽ���� �д�����Ҫ�ֽ������ �д����źų��� aΪ�վͻ����
//signal:�����ź�
//alpha:�������� ����ֵ2000
//tau:�����Ŷ���ֵ ����Ѱ����һ�����Ŷ�
//rtol:����������ֵ ��ǰ��Ƶ�����ľ�����С�ڸ���ֵ��ʱ�����
//Max_loopN:���ѭ����������
//omega_mat:����Ƶ�ʱ仯���� ���ڹ۲��������������Ƶ�ʵı仯 ����ΪNULL ����ΪNULL��ʱ��Ҫ�� �е��ڷֽ������ ��������������Ƶ�ʵĴ��� aΪ�վͻ����
//pmem:�ڴ����ṹ��
//</����˵��>
//<���˵��>����ǰ��Ƶ�����ľ�����</���˵��>
//<��ע>pmem��������Simple�ķ��䷽��</��ע>
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
