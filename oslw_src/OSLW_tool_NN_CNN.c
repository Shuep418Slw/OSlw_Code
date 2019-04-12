/*(Ver.=0.96)
* OSLW_tool.c
*
*  Created on: 2019-01-22
*      Author: slw
*/

#include "OSLW_include.h"

#if !(OSLW_SIMPLE_LEVEL >= 2)


#if OSLW_TOOL_IMPORT_NN || OSLW_TOOL_IMPORT_ALL


#if OSLW_TOOL_IMPORT_NN_BPnn || OSLW_TOOL_IMPORT_ALL

OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerConvNew(
	ParaType *pin,
	ParaType *pout,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 kern_x, lw_u16 kern_y, lw_u16 kern_num,
	lw_u16 move_delt,
	OSlwToolMatrixConvMethodNUM move_method,
	lw_u16 max_mini_batch,
	OSlwMemoryBasicSTU *pmem,
	lw_u32 info[4]
)
{
	OSlwToolNNLayerConvSTU *node;
	lw_u32 in_row, out_row;
	lw_u32 out_x, out_y, out_z;

	LwMatColType kern_len1, kern_len2, kern_res_len;

	OSLW_assert(!(pmem));

	//计算参数
	in_row = in_x*in_y*in_z;
	out_z = kern_num;
	kern_len1 = kern_x*kern_y*in_z;
	kern_len2 = kern_x*kern_y*kern_num;

	//分配节点内存
	node = pmem->Calloc(pmem, sizeof(OSlwToolNNLayerConvSTU));

	node->databasic.basic.NN_Kind = OSlwToolNNSubLayerKind_Conv;

	//node->DataRes = (ParaType *)(((lw_u8 *)node) + sizeof(OSlwToolNNLayerConvSTU));

	node->ConvMethod = move_method;

	if (move_method == OSlwToolMatrixConvMethod_Valid)
	{
		out_x = (in_x - kern_x + 1) / (move_delt);//横向移动次数
		out_y = (in_y - kern_y + 1) / (move_delt);//纵向移动次数

	}
	else if (move_method == OSlwToolMatrixConvMethod_Full)
	{
		out_x = (in_x) / (move_delt)+1;//横向移动次数
		out_y = (in_y) / (move_delt)+1;//纵向移动次数

	}
	else
	{
		OSLW_assert(1);
	}

	out_row = out_x*out_y*out_z;

	//设置输入
	if (pin == NULL)
	{
		pin = pmem->Malloc(pmem, PARA_MEM_CAL(max_mini_batch * in_row));
	}

	OSlwToolMatrixInitial(&(node->databasic.basic.in), in_row, max_mini_batch, pin);

	//设置输出
	if (pout == NULL)
	{
		pout = pmem->Malloc(pmem, PARA_MEM_CAL(max_mini_batch * out_row));
	}


#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C
	OSlwToolMatrixInitial(&(node->databasic.basic.out), max_mini_batch, out_row, pout);
	OSlwToolMatrixInitial(&(node->databasic.basic.in), max_mini_batch, in_row, pin);

#elif OSLW_TOOL_NN_DATA_FRAME == OSLW_TOOL_NN_D_FRAME_F
	OSlwToolMatrixInitial(&(node->databasic.basic.out), out_row, max_mini_batch, pout);
	OSlwToolMatrixInitial(&(node->databasic.basic.in), in_row, max_mini_batch, pin);
#else

#error"NN data frame is unknown"
	OSLW_assert(1);

#endif // OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C

	//设置参数
	node->databasic.Bias.row = 1;
	node->databasic.Bias.col = out_z;
	node->databasic.Bias.length = out_z;

	node->databasic.DeltB.row = 1;
	node->databasic.DeltB.col = out_z;
	node->databasic.DeltB.length = out_z;

	node->databasic.Weight.row = kern_len1;//一个卷积核有三个维度
	node->databasic.Weight.col = kern_num;
	node->databasic.Weight.length = node->databasic.Weight.row*node->databasic.Weight.col;

	node->databasic.DeltW.row = kern_len1;;
	node->databasic.DeltW.col = kern_num;
	node->databasic.DeltW.length = node->databasic.Weight.row*node->databasic.Weight.col;

	kern_res_len = (kern_len1 > kern_len2) ? kern_len1 : kern_len2;
	node->databasic.basic.FlowData.uData = PARA_MEM_CAL(kern_res_len);
	node->move_delt = move_delt;


	node->in_x = in_x;
	node->in_y = in_y;
	node->out_x = out_x;
	node->out_y = out_y;

	node->conv_kernal_x = kern_x;
	node->conv_kernal_y = kern_y;
	node->conv_kernal_z = in_z;
	node->conv_kernal_num = kern_num;

	//计算要分配的内存大小
	node->databasic.basic.sizeofdata = PARA_MEM_CAL(node->databasic.Weight.length) + PARA_MEM_CAL(node->databasic.Bias.length);


	//成员函数(只有前向传递与反向传递采用独立函数)
	node->databasic.basic.Forward = OSlwToolBPnnLayerConvForward;
	node->databasic.basic.Backward = OSlwToolBPnnLayerConvBackward;

	node->databasic.basic.Update = OSlwToolBPnnLayerFullConUpdate;
	node->databasic.basic.NNmalloc = OSlwToolBPnnLayerFullConNNmalloc;
	node->databasic.basic.Clear = OSlwToolBPnnLayerFullConClear;
	node->databasic.basic.DataInit = OSlwToolBPnnLayerFullConDataInit;
	node->databasic.basic.Copy = OSlwToolBPnnLayerFullConCopy;
	node->databasic.basic.SoftReplace = OSlwToolBPnnLayerFullConSoftReplace;

	if (info)
	{
		info[0] = out_x;
		info[1] = out_y;
		info[2] = out_z;
		info[3] = out_x*out_y*out_z;
	}

	return (OSlwToolNNSubLayerBasicSTU *)node;

}

OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerConvSetIm2Col(
	OSlwToolNNSubLayerBasicSTU *pbase,
	lw_u32 udata_sizeof_pdata,
	void *pdata
)
{

	OSlwToolNNLayerConvSTU *pconv=pbase;

	OSLW_assert(!(pbase));

#if !(defined(OSLW_USING_CBLAS) && OSLW_USING_CBLAS)
	OSLW_assert(1);
#endif

	if (pdata==NULL && udata_sizeof_pdata ==0)
	{
		return;
	}
	else if (pdata!=NULL && udata_sizeof_pdata ==0)
	{

		pconv->im2col_flag = 1;
		pbase->FlowData.pData = pdata;
	}
	else if(pdata==NULL && udata_sizeof_pdata !=0)
	{
		pconv->im2col_flag = 1;
		pbase->FlowData.uData = udata_sizeof_pdata;
	}
	else
	{
		pconv->im2col_flag = 1;
		pbase->FlowData.uData = udata_sizeof_pdata;
		pbase->FlowData.pData = pdata;
	}


	return pbase;


}


#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_F


OSlwMat * _OSlwToolNNKernalConvFd
(
	OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *in_m, OSlwToolMatrixSTU *kern_m, OSlwToolMatrixSTU *bias,
	lw_u16 pic_x, lw_u16 pic_y, lw_u16 pic_z,
	lw_u16 kern_x, lw_u16 kern_y,
	lw_u16 out_x, lw_u16 out_y, lw_u16 out_z,
	lw_u16 move_delt,
	lw_u16 now_min_batch,
	lw_u8 move_method,
	ParaType *buf

)
{
	lw_u32 i, j, jmax, k, kmax;
	lw_u32 m, mmax, n, nmax, o, omax;
	lw_u32 _minb, _minb_mpy_col, _minb_mpy_picl, _minb_mpy_outl, _d;
	ParaType *p_pic_head, *p_out_head;
	ParaType *p_pic_pos, *p_pic_pos_row, *p_out_pos;
	ParaType *p_buf;
	ParaType *p_pic_move_pix, *p_out_move_pix;
	ParaType *p_pic_move_col;
	ParaType *p_pic_move_pic;
	ParaType *p_kern;
	ParaType *p_bias;
	ParaType _sum, _mpy_b;


	OSLW_assert(!(s));
	OSLW_assert(!(in_m));
	OSLW_assert(!(kern_m));
	OSLW_assert(!(bias));
	OSLW_assert(!(buf));

	_minb = in_m->col;
	_minb_mpy_col = _minb*pic_x;
	_minb_mpy_picl = _minb_mpy_col*pic_y;
	_d = move_delt*_minb_mpy_col;

#define _NEXT_PIX(P) (P+=_minb)
#define _NEXT_ROW(P) (P+=_minb_mpy_col)
#define _NEXT_ROW_M(P) (P+=_d)
#define _NEXT_PIC(P) (P+=_minb_mpy_picl)
#define _NEXT_O_PIC(P) (P+=_minb_mpy_outl)


	mmax = pic_z;
	nmax = kern_y;
	omax = kern_x;

	//计算不同模式，循环的次数
	jmax = out_y;
	kmax = out_x;
	move_delt *= _minb;

	//循环所有的min-batch
	for (i = 0; i < now_min_batch; ++i)
	{
		p_pic_head = (in_m->a) + i;
		p_out_head = (s->a) + i;

		_minb_mpy_outl = (jmax)*(kmax);

		p_pic_pos_row = p_pic_head;
		p_out_pos = p_out_head;

		//扫描图片
		for (j = 0; j < jmax; ++j)
		{
			p_pic_pos = p_pic_pos_row;

			for (k = 0; k < kmax; k++)
			{
				//抠图 得到小矩阵
				p_pic_move_pic = p_pic_pos;
				p_buf = buf;
				m = mmax;
				while (m--)
				{
					p_pic_move_col = p_pic_move_pic;
					n = nmax;
					while (n--)
					{
						o = omax;
						p_pic_move_pix = p_pic_move_col;
						while (o--)
						{
							*p_buf++ = *p_pic_move_pix;
							_NEXT_PIX(p_pic_move_pix);
						}//end o
						_NEXT_ROW(p_pic_move_col);
					}//end n
					_NEXT_PIC(p_pic_move_pic);
				}//end m


				 //与每一个卷积核运算 并且加上偏置
				p_kern = kern_m->a;
				p_out_move_pix = p_out_pos;
				//每一个卷积核
				for (m = 0; m < kern_m->row; ++m)
				{

					n = kern_m->col;
					_sum = _ParaFint(0);
					p_buf = buf;

					//相乘相加
					while (n--)
					{
						_mpy_b = _ParaMpy(*p_buf, *p_kern);
						_sum += _mpy_b;
						p_buf++;
						p_kern++;
					}//end while n


					*p_out_move_pix = _sum + bias->a[m];//加上偏置
					_NEXT_O_PIC(p_out_move_pix);
				}//end for m

				_NEXT_PIX(p_out_pos);//输出位移
				p_pic_pos += move_delt;//输入卷积核位移
			}//end k

			 //下一张小图像的行
			_NEXT_ROW_M(p_pic_pos_row);

		}//end j


	}//end i 
#undef _NEXT_PIX(P) 
#undef _NEXT_ROW(P) 
#undef _NEXT_ROW_M(P) 
#undef _NEXT_PIC(P)
#undef _NEXT_O_PIC(P)

	return s;
}


OSlwToolMatrixSTU* _OSlwToolNNKernalConvBK
(
	OSlwToolMatrixSTU *inerr, OSlwToolMatrixSTU *outerr, OSlwToolMatrixSTU *kern,
	lw_u16 pic_x, lw_u16 pic_y, lw_u16 pic_z,
	lw_u16 kern_x, lw_u16 kern_y,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 now_min_batch,
	lw_u16 move_delt,
	ParaType *buf
)
{
	lw_u32 i, j, jmax;
	lw_u32 _minb, kern_len, _minb_mpy_in_len, _minb_mpy_out_len;

	lw_32 ix, iy, ixmax, ixmin, iymax, iymin;
	lw_32 m, n, mmax, nmax;
	ParaType *p_inerr_head, *p_outerr_head;
	ParaType *p_outerr_pos, *p_outerr_move, *p_inerr_pos, *p_inerr_move;
	ParaType *p_buf, *p_buf_bott;
	ParaType *p_kern, *p_kern_pos;
	ParaType databuf, datasum;

#define _OUTERR_PIX(D,X,Y) (D+(Y*pic_x+X)*_minb)
#define _NEXT_PIX(P) (P+=_minb)
#define _IN_NEXT_PIC(P) (P+=_minb_mpy_in_len)
#define _OUT_NEXT_PIC(P) (P+=_minb_mpy_out_len)

	if (move_delt != 1)
	{
		//没看懂不是间隔1的情况下的求导方式 所以默认必须是移动1格
		OSLW_assert(1);
	}

	//清空输入矩阵
	LW_MAT_CLR(inerr);


	//变量初始化
	_minb = outerr->col;
	jmax = kern->row;
	p_buf_bott = buf + kern_x*kern_y - 1;
	kern_len = kern_x*kern_y;
	_minb_mpy_in_len = _minb*in_x*in_y;
	_minb_mpy_out_len = _minb*pic_x*pic_y;

	ixmin = 1 - (lw_32)kern_x;
	iymin = 1 - (lw_32)kern_y;

	ixmax = pic_x;
	iymax = pic_y;

	for (i = 0; i < now_min_batch; i++)
	{
		p_outerr_head = (outerr->a) + i;
		p_inerr_head = (inerr->a) + i;
		p_kern_pos = kern->a;

		p_outerr_pos = p_outerr_head;

		//先计算一张图像
		for (j = 0; j < jmax; ++j)
		{
			p_inerr_pos = p_inerr_head;
			//循环整张图像
			for (iy = iymin; iy < iymax; ++iy)
			{
				for (ix = ixmin; ix < ixmax; ++ix)
				{
					p_inerr_move = p_inerr_pos;

					//取出一小块
					nmax = kern_y + iy;
					p_buf = p_buf_bott;//指针倒过来取

					for (n = iy; n < nmax; ++n)
					{
#if 0

						for (m = ix; m < ix + kern_x; ++m)
						{
							if (n<0 || n >= pic_y || m<0 || m >= pic_x)
							{
								*p_buf-- = _ParaFint(0);
							}
							else
							{
								*p_buf-- = *(_OUTERR_PIX(p_outerr_pos, m, n));
							}
						}



#else		
						mmax = kern_x;

						if (n<0 || n >= pic_y)
						{
							m = mmax;
							/*
							超出范围 这一行数据全为0
							->  0  0
							0  X  X  X
							X  X  X
							*/
							while (m--)
							{
								*p_buf-- = _ParaFint(0);
							}
						}
						else
						{
							m = ix;

							if (m<0)
							{
								/*
								赋值为0
								0  0
								->	0  X  X  X
								X  X  X
								*/
								while (m)
								{
									*p_buf-- = _ParaFint(0);
									++m;
								}
								mmax += (ix);
							}

							p_outerr_move = _OUTERR_PIX(p_outerr_pos, m, n);

							if (ix + kern_x > pic_x)
							{
								/*
								X  X  X
								->  X  X  X  0
								0  0
								先赋值再补零
								*/
								m = (ix + kern_x - pic_x);//最右边像素点数量
								mmax -= m;// 如果这一行数据原本是 000 XXXX 000 那这时mmax应该是4
								while (mmax--)
								{
									*p_buf-- = *p_outerr_move;
									_NEXT_PIX(p_outerr_move);
								}
								while (m--)
								{
									*p_buf-- = _ParaFint(0);
								}
							}
							else
							{
								/*
								0  0
								->  X  X  X
								X  X  X
								直接赋值 不用补零
								*/
								while (mmax--)
								{
									*p_buf-- = *p_outerr_move;
									_NEXT_PIX(p_outerr_move);
								}
							}
						}// end if
#endif
					}//end n

					 //现在已经取出一小块
					p_kern = p_kern_pos;
					m = in_z;
					while (m--)
					{
						n = kern_len;
						p_buf = buf;
						datasum = _ParaFint(0);

						while (n--)
						{
							databuf = _ParaMpy(*p_kern, *p_buf);
							datasum += databuf;
							++p_kern;
							++p_buf;
						}
						*p_inerr_move += datasum;
						_IN_NEXT_PIC(p_inerr_move);

					}

					_NEXT_PIX(p_inerr_pos);

				}// end iX

			}// end iY

			p_kern_pos += kern->col;
			_OUT_NEXT_PIC(p_outerr_pos);

		}//end j

	}//end i

#undef _OUTERR_PIX(D,X,Y) 
#undef _NEXT_PIX(P) 
#undef _IN_NEXT_PIC(P) 
#undef _OUT_NEXT_PIC(P) 

	return inerr;
}


ParaType _OSlwToolNNKernalConvCalDeltOnce
(
	ParaType *delt, ParaType *inerr, ParaType *outerr1,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 kern_x, lw_u16 kern_y,
	lw_u16 out_x, lw_u16 out_y,
	lw_u16 min_batch,
	lw_u16 move_delt
)
{
	lw_u32 i;
	lw_u32 j, k;
	lw_u32 m, n;
	lw_u32 _minb, _minb_mpy_in_x, _minb_mpy_in_x_move_delt, _minb_mpy_move_delt, _minb_mpy_in_len;
	ParaType sum, databuf;

	ParaType *p_in_pic, *p_in_row, *p_in_pix;

	ParaType *p_in_move_row, *p_in_move_pix;

	ParaType *p_out_move;

	ParaType *p_delt;

#define _NEXT_PIX(P) (P+=_minb)
#define _NEXT_I_ROW(P) (P+=_minb_mpy_in_x)
#define _NEXT_I_ROW_D(P) (P+=_minb_mpy_in_x_move_delt)
#define _NEXT_I_PIC(P) (P+=_minb_mpy_in_len)
#define _NEXT_I_PIX_D(P) (P+=_minb_mpy_move_delt)
	//变量初始化
	_minb = min_batch;

	_minb_mpy_in_x = _minb*in_x;
	_minb_mpy_in_x_move_delt = _minb_mpy_in_x*move_delt;
	_minb_mpy_move_delt = min_batch*move_delt;
	_minb_mpy_in_len = min_batch*in_x*in_y;

	p_delt = delt;
	//循环每一张图片

	for (i = 0, p_in_pic = inerr; i < in_z; ++i)
	{
		for (j = 0, p_in_row = p_in_pic; j < kern_y; j++)
		{
			for (k = 0, p_in_pix = p_in_row; k < kern_x; k++)
			{

				//循环小图像并且相乘加
				m = out_y;
				p_in_move_row = p_in_pix;
				p_out_move = outerr1;
				sum = _ParaFint(0);

				while (m--)
				{
					n = out_x;
					p_in_move_pix = p_in_move_row;
					while (n--)
					{
						databuf = _ParaMpy(*p_in_move_pix, *p_out_move);
						sum += databuf;

						_NEXT_PIX(p_out_move);
						_NEXT_PIX(p_in_move_pix);

					}// end n

					_NEXT_I_ROW(p_in_move_row);
				}// end m

				 //得到一个数sum
				*p_delt++ = sum;

				_NEXT_I_PIX_D(p_in_pix);
			}//end k
			_NEXT_I_ROW_D(p_in_row);
		}//end j
		_NEXT_I_PIC(p_in_pic);
	}//end i



	sum = _ParaFint(0);
	i = out_x*out_y;
	while (i--)
	{
		sum += *outerr1++;
	}

#undef _NEXT_PIX(P)
#undef _NEXT_I_ROW(P)
#undef _NEXT_I_ROW_D(P)
#undef _NEXT_I_PIC(P) 
#undef _NEXT_I_PIX_D(P)

	return sum;
}

#endif // OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_F


lw_ptr OSlwToolBPnnLayerConvForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	OSlwToolNNLayerFullConSTU *pfc;
	OSlwToolNNLayerConvSTU *pcv;
	lw_ptr min_b = mini_b_num;
	OSlwMat in, out, we;
	OSLW_assert(!(pNNSLB));
	pfc = (OSlwToolNNLayerFullConSTU *)pNNSLB;
	pcv = (void *)pfc;

#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C

	OSlwMatInit(&in, pcv->in_x, pcv->in_y, pfc->basic.in.a);
	OSlwMatInit(&out, pcv->out_x, pcv->out_y, pfc->basic.out.a);
	OSlwMatInit(&we, pcv->conv_kernal_x, pcv->conv_kernal_y, pfc->Weight.a);
	we.length *= pcv->conv_kernal_z;
	out.length *= pcv->conv_kernal_z;


	if (pcv->im2col_flag)
	{
		while (min_b--)
		{

			OSlwToolMatrixConvFastMultChIm2ColFd(
				&out, &we, &in, &(pfc->Bias),
				pcv->conv_kernal_z, pcv->conv_kernal_num,
				pcv->move_delt, pcv->move_delt,
				pcv->ConvMethod,
				pfc->basic.FlowData.uData / sizeof(ParaType),
				pfc->basic.FlowData.pData
			);

			in.a += pfc->basic.in.col;
			out.a += pfc->basic.out.col;

		}
	}
	else
	{
		while (min_b--)
		{
			pOSlwToolMatrixConvFastMultCh(
				&out, &we, &in, &(pfc->Bias),
				pcv->conv_kernal_z, pcv->conv_kernal_num,
				pcv->move_delt, pcv->move_delt,
				pcv->ConvMethod,
				1,
				pfc->basic.FlowData.pData
			);

			in.a += pfc->basic.in.col;
			out.a += pfc->basic.out.col;

		}
	}






#elif OSLW_TOOL_NN_DATA_FRAME == OSLW_TOOL_NN_D_FRAME_F

	_OSlwToolNNKernalConvFd(
		&(pfc->basic.out),
		&(pfc->basic.in),
		&(pfc->Weight),
		&(pfc->Bias),
		pcv->in_x,
		pcv->in_y,
		pcv->conv_kernal_z,
		pcv->conv_kernal_x,
		pcv->conv_kernal_y,
		pcv->out_x,
		pcv->out_y,
		pcv->conv_kernal_num,
		1,//强制默认为1
		mini_b_num,
		's',//强制默认为非填充
		pcv->databasic.basic.FlowData.pData
	);

#else

#error"NN data frame is unknown"
	OSLW_assert(1);

#endif // OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C

	return mini_b_num;

}


lw_ptr OSlwToolBPnnLayerConvBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	OSlwToolNNLayerFullConSTU *pfc;
	OSlwToolNNLayerConvSTU *pcv;
	lw_u16 i = 0, j, k;
	ParaType *db, *dw, *dw2, delt_bias, *dbm;
	lw_u32 move_d;
	OSlwMat m_dw, m_in, m_out;
	OSLW_assert(!(pNNSLB));
	pfc = (OSlwToolNNLayerFullConSTU *)pNNSLB;
	pcv = (void *)pfc;
	move_d = pfc->basic.in.col*pcv->out_x*pcv->out_y;

#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C
	i = mini_b_num;
	db = pfc->DeltB.a;

	OSlwMatInit(&m_dw, pcv->conv_kernal_x, pcv->conv_kernal_y, pfc->DeltW.a);
	OSlwMatInit(&m_in, pcv->in_x, pcv->in_y, pfc->basic.in.a);
	OSlwMatInit(&m_out, pcv->out_x, pcv->out_y, pfc->basic.out.a);

	////如果是第一次反向传播 需要覆盖原先的梯度 update函数自动清空梯度
	//if (pNNSLB->pNN->Train._batch_stream_count <= 1)
	//{
	//	//对于每一个卷积核
	//	for ( j = 0, m_out.a= pfc->basic.out.a; j < pcv->conv_kernal_num; j++,m_out.a+=m_out.length)
	//	{
	//		//计算db
	//		for ( k = 0, delt_bias=_ParaFint(0), dbm=m_out.a; k < m_out.length; k++, dbm++)
	//		{
	//			delt_bias += *dbm;
	//		}
	//		*db++ = delt_bias;
	//		//对于每一个维度
	//		for ( k = 0, m_in.a= pfc->basic.in.a; k < pcv->conv_kernal_z; k++,m_in.a+=m_in.length)
	//		{
	//			pOSlwToolMatrixConv2(
	//				&m_dw, &m_out, &m_in,
	//				pcv->move_delt, pcv->move_delt,
	//				1,
	//				's',
	//				0,
	//				NULL
	//			);
	//			m_dw.a += m_dw.length;
	//		}
	//	}
	//	i--;
	//}

	if (pNNSLB->pNN->Train.Flag.NeedTrain== OSlwToolNNNeedTrain_Need)
	{
		while (i--)
		{
			m_dw.a = pfc->DeltW.a;
			db = pfc->DeltB.a;

			for (j = 0, m_out.a = pfc->basic.out.a; j < pcv->conv_kernal_num; j++, m_out.a += m_out.length, db++)
			{

				//计算db
				for (k = 0, delt_bias = _ParaFint(0), dbm = m_out.a; k < m_out.length; k++, dbm++)
				{
					delt_bias += *dbm;
				}
				*db += delt_bias;

				for (k = 0, m_in.a = pfc->basic.in.a; k < pcv->conv_kernal_z; k++, m_in.a += m_in.length)
				{
					pOSlwToolMatrixConv2(
						&m_dw, &m_out, &m_in,
						pcv->move_delt, pcv->move_delt,
						0,
						OSlwToolMatrixConvMethod_Valid,
						0,
						NULL
					);
					m_dw.a += m_dw.length;
				}
			}
		}
	}



	//求输入误差
	i = mini_b_num;
	m_dw.a = pfc->Weight.a;
	m_dw.length *= pcv->conv_kernal_z;
	m_out.a = pfc->basic.out.a;
	m_in.a = pfc->basic.in.a;
	m_in.length *= pcv->conv_kernal_z;
	while (i--)
	{
		pOSlwToolMatrixConvFastMultCh(
			&m_in, &m_dw, &m_out, NULL,
			pcv->conv_kernal_num, pcv->conv_kernal_z,
			pcv->move_delt, pcv->move_delt,
			pcv->ConvMethod,
			0,
			pfc->basic.FlowData.pData
		);

		m_in.a += m_in.length;
		m_out.a += m_out.length;
	}


#elif OSLW_TOOL_NN_DATA_FRAME == OSLW_TOOL_NN_D_FRAME_F
	//如果是第一次反向传播 需要覆盖原先的梯度
	if (pNNSLB->pNN->Train._batch_stream_count <= 1)
	{
		j = pcv->conv_kernal_num;
		db = pfc->DeltB.a;
		dw = pfc->DeltW.a;
		while (j--)
		{
			dw2 = pcv->databasic.basic.FlowData.pData;

			//先计算delt
			delt_bias = _OSlwToolNNKernalConvCalDeltOnce(
				dw2,
				pfc->basic.in.a,
				pfc->basic.out.a,
				pcv->in_x,
				pcv->in_y,
				pcv->conv_kernal_z,
				pcv->conv_kernal_x,
				pcv->conv_kernal_y,
				pcv->out_x, pcv->out_y,
				pfc->basic.in.col,
				1
			);

			//由于是第一次反向传播，要进行覆盖
			k = move_d;
			while (k--)
			{
				*dw++ = *dw2++;
			}
			*db++ = delt_bias;

		}

		i++;
	}

	for (; i < mini_b_num; i++)
	{
		j = pcv->conv_kernal_num;
		db = pfc->DeltB.a + i;
		dw = pfc->DeltW.a + i;
		//循环每一个卷积核
		while (j--)
		{
			dw2 = pcv->databasic.basic.FlowData.pData;

			//先计算delt
			delt_bias = _OSlwToolNNKernalConvCalDeltOnce(
				dw2,
				pfc->basic.in.a,
				pfc->basic.out.a,
				pcv->in_x,
				pcv->in_y,
				pcv->conv_kernal_z,
				pcv->conv_kernal_x,
				pcv->conv_kernal_y,
				pcv->out_x, pcv->out_y,
				pfc->basic.in.col,
				1
			);

			//不能覆盖 相加
			k = move_d;
			while (k--)
			{
				*dw += *dw2++;
				dw++;
			}
			*db += delt_bias;
			db++;
		}
	}

	//求输入误差
	_OSlwToolNNKernalConvBK(
		&(pfc->basic.in),
		&(pfc->basic.out),
		&(pfc->Weight),
		pcv->out_x,
		pcv->out_y,
		pcv->conv_kernal_num,
		pcv->conv_kernal_x,
		pcv->conv_kernal_y,
		pcv->in_x,
		pcv->in_y,
		pcv->conv_kernal_z,
		mini_b_num,
		1,
		pcv->databasic.basic.FlowData.pData
	);
#else

#error"NN data frame is unknown"
	OSLW_assert(1);

#endif // OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C





	return mini_b_num;
}



OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerPoolNew(
	ParaType *pin,
	ParaType *pout,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 pool_x, lw_u16 pool_y,
	OSlwToolNNPoolingMethodNUM pool_method,
	lw_u16 max_mini_batch,
	OSlwMemoryBasicSTU *pmem,
	lw_u32 info[4]
)
{
	OSlwToolNNLayerPoolSTU *node;
	lw_u32 max_data_len;
	lw_u32 out_x, out_y;
	LwMatRowType in_row, out_row;

	out_x = in_x / pool_x;
	out_y = in_y / pool_y;

	in_row = in_x*in_y*in_z;;
	out_row = out_x*out_y*in_z;

	if (pool_method == 'M')
	{
		node = pmem->Malloc(pmem, sizeof(OSlwToolNNLayerPoolSTU) + sizeof(lw_ptr)*out_row);
		node->MaxPool = (void *)(((lw_u8 *)(node)) + sizeof(OSlwToolNNLayerPoolSTU));
	}
	else if (pool_method == 'A')
	{
		node = pmem->Malloc(pmem, sizeof(OSlwToolNNLayerPoolSTU));
	}
	else
	{
		OSLW_assert(1);
	}

	node->basic.NN_Kind = OSlwToolNNSubLayerKind_Pool;

	node->in_x = in_x;
	node->in_y = in_y;
	node->move_x = pool_x;
	node->move_y = pool_y;
	node->out_x = out_x;
	node->out_y = out_y;
	node->pic_z = in_z;
	node->PoolMethod = pool_method;

	//设置输入
	if (pin == NULL)
	{
		pin = pmem->Malloc(pmem, PARA_MEM_CAL(max_mini_batch * in_row));
	}

	OSlwToolMatrixInitial(&(node->basic.in), in_row, max_mini_batch, pin);

	//设置输出
	if (pout == NULL)
	{
		pout = pmem->Malloc(pmem, PARA_MEM_CAL(max_mini_batch * out_row));
	}

	OSlwToolMatrixInitial(&(node->basic.out), out_row, max_mini_batch, pout);


	//成员函数 只用前向传递采用采用函数其他采用默认
	node->basic.Forward = OSlwToolBPnnLayerPoolForward;
	node->basic.Backward = OSlwToolBPnnLayerPoolBackward;

	node->basic.Update = OSlwToolBPnnLayerUpdateDefault;
	node->basic.NNmalloc = OSlwToolBPnnLayerNNmallocDefault;
	node->basic.Clear = OSlwToolBPnnLayerClearDefault;
	node->basic.DataInit = OSlwToolBPnnLayerDataInitDefault;
	node->basic.Copy = OSlwToolBPnnLayerCopyDefault;
	node->basic.SoftReplace = OSlwToolBPnnLayerSoftReplaceDefault;

	if (info)
	{
		info[0] = out_x;
		info[1] = out_y;
		info[2] = in_z;
		info[3] = out_row;
	}

	return (OSlwToolNNSubLayerBasicSTU *)node;
}





#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C

void* _OSlwToolNNMaxPoolingFD
(
	OSlwToolMatrixSTU *in, OSlwToolMatrixSTU *out,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 out_x, lw_u16 out_y,
	lw_u16 now_min_batch,
	lw_u16 move_x, lw_u16 move_y,
	ParaType **data_mem
)
{
	ParaType *in_a = in->a, *out_a = out->a, *in_p;
	ParaType *in_c, *in_r, *in_p_c, *in_p_r;
	lw_u32 in_pic_2d_move, in_c_move, in_r_move, in_p_c_move;
	ParaType now_max, *pMax = NULL;
	lw_32  jx, jy, k, l, m;


	in_pic_2d_move = in_x*in_y;
	in_p_c_move = in_x;
	in_r_move = move_x;
	in_c_move = in_x*move_y;

	now_min_batch *= in_z;
	while (now_min_batch--)
	{

		//大图像循环
		for (jy = 0, in_c = in_a; jy < out_y; jy++, in_c += in_c_move)
		{
			for (jx = 0, in_r = in_c; jx < out_x; jx++, in_r += in_r_move)
			{

				//小图像循环
				for (k = 0, in_p_c = in_r, pMax = in_r, now_max = *in_r; k < move_y; k++, in_p_c += in_p_c_move)
				{
					for (l = 0, in_p_r = in_p_c; l < move_x; l++, in_p_r++)
					{
						if (*in_p_r > now_max)
						{
							pMax = in_p_r;
							now_max = *in_p_r;
						}
					}
				}
				*data_mem++ = pMax;
				*out_a++ = now_max;
			}
		}


		in_a += in_pic_2d_move;
	}
	return (void *)data_mem;
}

OSlwToolMatrixSTU* _OSlwToolNNAvgPoolingFD
(
	OSlwToolMatrixSTU *in, OSlwToolMatrixSTU *out,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 out_x, lw_u16 out_y,
	lw_u16 now_min_batch,
	lw_u16 move_x, lw_u16 move_y
)
{
	ParaType *in_a = in->a, *out_a = out->a, *in_p;
	ParaType *in_c, *in_r, *in_p_c, *in_p_r;
	lw_u32 in_pic_2d_move, in_c_move, in_r_move, in_p_c_move;
	ParaType now_sum, data_div = _ParaDiv(_ParaFint(1), _ParaFint(move_x* move_y));
	lw_32 jx, jy, k, l, m;


	in_pic_2d_move = in_x*in_y;
	in_p_c_move = in_x;
	in_r_move = move_x;
	in_c_move = in_x*move_y;

	now_min_batch *= in_z;
	while (now_min_batch--)
	{

		//大图像循环
		for (jy = 0, in_c = in_a; jy < out_y; jy++, in_c += in_c_move)
		{
			for (jx = 0, in_r = in_c; jx < out_x; jx++, in_r += in_r_move)
			{

				//小图像循环
				for (k = 0, in_p_c = in_r, now_sum = _ParaFint(0); k < move_y; k++, in_p_c += in_p_c_move)
				{
					for (l = 0, in_p_r = in_p_c; l < move_x; l++, in_p_r++)
					{
						now_sum += *in_p_r;
					}
				}

				*out_a++ = _ParaMpy(data_div, now_sum);
			}
		}


		in_a += in_pic_2d_move;
	}
	return (void *)out;
}



OSlwToolMatrixSTU* _OSlwToolNNMaxPoolingBK
(
	OSlwToolMatrixSTU *inerr, OSlwToolMatrixSTU *outerr,
	lw_u16 inerr_x, lw_u16 inerr_y, lw_u16 inerr_z,
	lw_u16 outerr_x, lw_u16 outerr_y,
	lw_u16 now_min_batch,
	lw_u16 move_x, lw_u16 move_y,
	ParaType **data_mem
)
{

	lw_u32 i = now_min_batch*outerr_x*outerr_y*inerr_z;
	ParaType *out_a = outerr->a;

	LW_MAT_CLR(inerr);
	while (i--)
	{
		**data_mem = *out_a++;
		data_mem++;
	}

	return inerr;
}


OSlwToolMatrixSTU* _OSlwToolNNAvgPoolingBK
(
	OSlwToolMatrixSTU *inerr, OSlwToolMatrixSTU *outerr,
	lw_u16 inerr_x, lw_u16 inerr_y, lw_u16 inerr_z,
	lw_u16 outerr_x, lw_u16 outerr_y,
	lw_u16 now_min_batch,
	lw_u16 move_x, lw_u16 move_y
)
{
	ParaType *in_a = inerr->a, *out_a = outerr->a, *in_p;
	ParaType *in_c, *in_r, *in_p_c, *in_p_r;
	lw_u32 in_pic_2d_move, in_c_move, in_r_move, in_p_c_move;
	ParaType now_sum, data_div = _ParaDiv(_ParaFint(1), _ParaFint(move_x* move_y));
	lw_32 jx, jy, k, l, m;


	in_pic_2d_move = inerr_x*inerr_y;
	in_p_c_move = inerr_x;
	in_r_move = move_x;
	in_c_move = inerr_x*move_y;

	now_min_batch *= inerr_z;
	while (now_min_batch--)
	{

		//大图像循环
		for (jy = 0, in_c = in_a; jy < outerr_y; jy++, in_c += in_c_move)
		{
			for (jx = 0, in_r = in_c; jx < outerr_x; jx++, in_r += in_r_move, out_a++)
			{

				//小图像循环
				for (k = 0, in_p_c = in_r, now_sum = _ParaMpy(*out_a, data_div); k < move_y; k++, in_p_c += in_p_c_move)
				{
					for (l = 0, in_p_r = in_p_c; l < move_x; l++, in_p_r++)
					{
						*in_p_r = now_sum;
					}
				}
			}
		}


		in_a += in_pic_2d_move;
	}



	return inerr;
}

#elif OSLW_TOOL_NN_DATA_FRAME == OSLW_TOOL_NN_D_FRAME_F
void* _OSlwToolNNMaxPoolingFD
(
	OSlwToolMatrixSTU *in, OSlwToolMatrixSTU *out,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 out_x, lw_u16 out_y,
	lw_u16 now_min_batch,
	lw_u16 move_x, lw_u16 move_y,
	ParaType **data_mem
)
{
	LwMatColType _minb;
	lw_u16 _minb_mpy_in_x, _minb_mpy_in_len;
	lw_u32 m_x, m_y;
	lw_u32 i, j, m, n;

	ParaType *p_in_head, *p_in_pic, *p_in_row, *p_in_pix;
	ParaType *p_in_move_row, *p_in_move_pix;
	ParaType *p_out_head, *p_out_pix;

	ParaType maxdata;
	ParaType *pMaxIndex;

#define _NEXT_PIX(P) (P+=_minb)
#define _I_NEXT_ROW(P) (P+=_minb_mpy_in_x)
#define _I_NEXT_PIC(P) (P+=_minb_mpy_in_len)
#define _I_MOVE_COL(P) (P+=m_x)
#define _I_MOVE_ROW(P) (P+=m_y)

	_minb = in->col;
	_minb_mpy_in_x = _minb * in_x;
	_minb_mpy_in_len = _minb_mpy_in_x * in_y;
	m_x = _minb*move_x;
	m_y = _minb_mpy_in_x*move_y;

	p_in_head = in->a;
	p_out_head = out->a;
	//循环每一个minbatch
	while (now_min_batch--)
	{
		p_in_pic = p_in_head;
		p_out_pix = p_out_head;
		//循环每一张图像
		while (in_z--)
		{
			i = out_y;
			p_in_row = p_in_pic;
			while (i--)
			{

				j = out_x;
				p_in_pix = p_in_row;
				while (j--)
				{

					//循环小图像
					p_in_move_row = p_in_pix;
					m = move_y;
					maxdata = *p_in_move_row;
					pMaxIndex = p_in_move_row;
					while (m--)
					{
						p_in_move_pix = p_in_move_row;
						n = move_x;
						while (n--)
						{
							if (maxdata < *p_in_move_pix)
							{
								maxdata = *p_in_move_pix;
								pMaxIndex = p_in_move_pix;
							}

							_NEXT_PIX(p_in_move_pix);
						}
						_I_NEXT_ROW(p_in_move_row);
					}

					//更新输出
					*data_mem++ = pMaxIndex;
					*p_out_pix = maxdata;


					_NEXT_PIX(p_out_pix);
					_I_MOVE_COL(p_in_pix);
				}

				_I_MOVE_ROW(p_in_row);
			}
			_I_NEXT_PIC(p_in_pic);
		}

		p_in_head++;
		p_out_head++;
	}


#undef _NEXT_PIX(P)
#undef _I_NEXT_ROW(P)
#undef _I_NEXT_PIC(P)
#undef _I_MOVE_COL(P)
#undef _I_MOVE_ROW(P)

	return (void *)data_mem;
}

OSlwToolMatrixSTU* _OSlwToolNNAvgPoolingFD
(
	OSlwToolMatrixSTU *in, OSlwToolMatrixSTU *out,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 out_x, lw_u16 out_y,
	lw_u16 now_min_batch,
	lw_u16 move_x, lw_u16 move_y
)
{
	LwMatColType _minb;
	lw_u16 _minb_mpy_in_x, _minb_mpy_in_len;

	lw_u32 m_x, m_y;
	lw_u32 i, j, m, n;

	ParaType *p_in_head, *p_in_pic, *p_in_row, *p_in_pix;
	ParaType *p_in_move_row, *p_in_move_pix;
	ParaType *p_out_head, *p_out_pix;

	ParaType sumdata, NumData;


#define _NEXT_PIX(P) (P+=_minb)
#define _I_NEXT_ROW(P) (P+=_minb_mpy_in_x)
#define _I_NEXT_PIC(P) (P+=_minb_mpy_in_len)
#define _I_MOVE_COL(P) (P+=m_x)
#define _I_MOVE_ROW(P) (P+=m_y)

	_minb = in->col;
	_minb_mpy_in_x = _minb * in_x;
	_minb_mpy_in_len = _minb_mpy_in_x * in_y;
	m_x = _minb*move_x;
	m_y = _minb_mpy_in_x*move_y;

	NumData = _ParaDiv(_ParaFint(1), _ParaFint(move_x*move_y));

	p_in_head = in->a;
	p_out_head = out->a;
	//循环每一个minbatch
	while (now_min_batch--)
	{
		p_in_pic = p_in_head;
		p_out_pix = p_out_head;
		//循环每一张图像
		while (in_z--)
		{
			i = out_y;
			p_in_row = p_in_pic;
			while (i--)
			{

				j = out_x;
				p_in_pix = p_in_row;
				while (j--)
				{

					//循环小图像
					p_in_move_row = p_in_pix;
					m = move_y;
					sumdata = _ParaFint(0);
					while (m--)
					{
						p_in_move_pix = p_in_move_row;
						n = move_x;
						while (n--)
						{

							sumdata += *p_in_move_pix;

							_NEXT_PIX(p_in_move_pix);
						}
						_I_NEXT_ROW(p_in_move_row);
					}

					//更新输出

					*p_out_pix = _ParaMpy(sumdata, NumData);

					_NEXT_PIX(p_out_pix);
					_I_MOVE_COL(p_in_pix);
				}

				_I_MOVE_ROW(p_in_row);
			}
			_I_NEXT_PIC(p_in_pic);
		}

		p_in_head++;
		p_out_head++;
	}


#undef _NEXT_PIX(P)
#undef _I_NEXT_ROW(P)
#undef _I_NEXT_PIC(P)
#undef _I_MOVE_COL(P)
#undef _I_MOVE_ROW(P)

	return out;
}



OSlwToolMatrixSTU* _OSlwToolNNMaxPoolingBK
(
	OSlwToolMatrixSTU *inerr, OSlwToolMatrixSTU *outerr,
	lw_u16 inerr_x, lw_u16 inerr_y, lw_u16 inerr_z,
	lw_u16 outerr_x, lw_u16 outerr_y,
	lw_u16 now_min_batch,
	lw_u16 move_x, lw_u16 move_y,
	ParaType **data_mem
)
{

	LwMatColType _minb;
	ParaType *p_out_head, *p_out_pix;
	LwMatRowType i;

#define _NEXT_PIX(P) (P+=_minb)

	_minb = inerr->col;

	LW_MAT_CLR(inerr);

	p_out_head = outerr->a;
	while (now_min_batch--)
	{
		p_out_pix = p_out_head;
		i = outerr->row;

		while (i--)
		{
			**data_mem = *p_out_pix;
			++data_mem;
			_NEXT_PIX(p_out_pix);
		}

		p_out_head++;
	}


#undef _NEXT_PIX(P)

	return inerr;
}


OSlwToolMatrixSTU* _OSlwToolNNAvgPoolingBK
(
	OSlwToolMatrixSTU *inerr, OSlwToolMatrixSTU *outerr,
	lw_u16 inerr_x, lw_u16 inerr_y, lw_u16 inerr_z,
	lw_u16 outerr_x, lw_u16 outerr_y,
	lw_u16 now_min_batch,
	lw_u16 move_x, lw_u16 move_y
)
{
	LwMatColType _minb;
	lw_u16 _minb_mpy_in_x, _minb_mpy_in_len;
	lw_u32 m_x, m_y;
	lw_u32 i, j, m, n;

	ParaType *p_in_head, *p_in_pic, *p_in_row, *p_in_pix;
	ParaType *p_in_move_row, *p_in_move_pix;
	ParaType *p_out_head, *p_out_pix;

	ParaType data, NumData;


#define _NEXT_PIX(P) (P+=_minb)
#define _I_NEXT_ROW(P) (P+=_minb_mpy_in_x)
#define _I_NEXT_PIC(P) (P+=_minb_mpy_in_len)
#define _I_MOVE_COL(P) (P+=m_x)
#define _I_MOVE_ROW(P) (P+=m_y)

	_minb = inerr->col;;
	_minb_mpy_in_x = _minb * inerr_x;
	_minb_mpy_in_len = _minb_mpy_in_x * inerr_y;
	m_x = _minb*move_x;
	m_y = _minb_mpy_in_x*move_y;

	NumData = _ParaDiv(_ParaFint(1), _ParaFint(move_x*move_y));

	p_in_head = inerr->a;
	p_out_head = outerr->a;
	//循环每一个minbatch
	while (now_min_batch--)
	{
		p_in_pic = p_in_head;
		p_out_pix = p_out_head;
		//循环每一张图像
		while (inerr_z--)
		{
			i = outerr_y;
			p_in_row = p_in_pic;
			while (i--)
			{

				j = outerr_x;
				p_in_pix = p_in_row;
				while (j--)
				{

					//循环小图像
					p_in_move_row = p_in_pix;
					m = move_y;
					data = _ParaMpy(*p_out_pix, NumData);//得到平均值
					while (m--)
					{
						p_in_move_pix = p_in_move_row;
						n = move_x;
						while (n--)
						{
							*p_in_move_pix = data;//赋值
							_NEXT_PIX(p_in_move_pix);
						}
						_I_NEXT_ROW(p_in_move_row);
					}
					_NEXT_PIX(p_out_pix);
					_I_MOVE_COL(p_in_pix);
				}
				_I_MOVE_ROW(p_in_row);
			}
			_I_NEXT_PIC(p_in_pic);
		}

		p_in_head++;
		p_out_head++;
	}


#undef _NEXT_PIX(P)
#undef _I_NEXT_ROW(P)
#undef _I_NEXT_PIC(P)
#undef _I_MOVE_COL(P)
#undef _I_MOVE_ROW(P)

	return inerr;
}
#else
#error"NN data frame is unknown"
#endif // OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C


lw_ptr OSlwToolBPnnLayerPoolForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	OSlwToolNNLayerPoolSTU *pPL;
	OSLW_assert(!(pNNSLB));
	pPL = (OSlwToolNNLayerPoolSTU *)pNNSLB;


	if (pPL->PoolMethod == OSlwToolNNPoolingMethod_Max)
	{
		_OSlwToolNNMaxPoolingFD(
			&(pNNSLB->in),
			&(pNNSLB->out),
			pPL->in_x, pPL->in_y, pPL->pic_z,
			pPL->out_x, pPL->out_y,
			mini_b_num,
			pPL->move_x, pPL->move_y,
			pPL->MaxPool
		);
	}
	else if (pPL->PoolMethod == OSlwToolNNPoolingMethod_Avg)
	{
		_OSlwToolNNAvgPoolingFD(
			&(pNNSLB->in),
			&(pNNSLB->out),
			pPL->in_x, pPL->in_y, pPL->pic_z,
			pPL->out_x, pPL->out_y,
			mini_b_num,
			pPL->move_x, pPL->move_y
		);
	}
	else
	{
		OSLW_assert(1);
	}


	return mini_b_num;
}

lw_ptr OSlwToolBPnnLayerPoolBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	OSlwToolNNLayerPoolSTU *pPL;
	OSLW_assert(!(pNNSLB));
	pPL = (OSlwToolNNLayerPoolSTU *)pNNSLB;


	if (pPL->PoolMethod == OSlwToolNNPoolingMethod_Max)
	{
		_OSlwToolNNMaxPoolingBK(
			&(pNNSLB->in),
			&(pNNSLB->out),
			pPL->in_x, pPL->in_y, pPL->pic_z,
			pPL->out_x, pPL->out_y,
			mini_b_num,
			pPL->move_x, pPL->move_y,
			pPL->MaxPool
		);
	}
	else if (pPL->PoolMethod == OSlwToolNNPoolingMethod_Avg)
	{
		_OSlwToolNNAvgPoolingBK(
			&(pNNSLB->in),
			&(pNNSLB->out),
			pPL->in_x, pPL->in_y, pPL->pic_z,
			pPL->out_x, pPL->out_y,
			mini_b_num,
			pPL->move_x, pPL->move_y
		);
	}
	else
	{
		OSLW_assert(1);
	}


	return mini_b_num;
}


void* OSlwToolBPnnConvAppend
(
	OSlwToolBPnnSTU *pBPnn,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 kern_x, lw_u16 kern_y, lw_u16 kern_num,
	OSlwToolMatrixConvMethodNUM conv_method,
	ParaType *pin, ParaType *pout,
	ParaType *pWe, ParaType *pBi,
	OSlwNNinitFunType pfun,
	OSlwToolRandomBasicSTU *pr,
	ParaType d1, ParaType d2,
	OSlwToolNNLayerActFunSTU *pTemplet,
	OSlwMemoryBasicSTU *pmem,
	lw_u32 info[4]
)
{

	OSlwToolNNSubLayerBasicSTU *pnode1, *pnode2;
	OSlwToolNNLayerConvSTU *pcv;
	OSlwToolNNLayerFullConSTU *pfc;
	OSlwToolDListNodeSTU *pln1, *pln2;
	ParaType *pWreal, *pBreal;
	OSlwToolNNSubLayerBasicSTU **ppLIST1, **ppLIST2, **pptail;
	LwMatRowType in_col, mat_out_col;
	lw_u32 out_x, out_y;
	OSLW_assert(!(pBPnn));
	OSLW_assert(!(pmem));

	in_col = in_x*in_y*in_z;
	out_x = (in_x - kern_x) + 1;//横向移动次数
	out_y = (in_y - kern_y) + 1;//纵向移动次数
	mat_out_col = out_x*out_y*kern_num;

	if (pTemplet)
	{

		_NN_GET_IN(pBPnn, pin);

		pnode1 = OSlwToolNNLayerConvNew(
			pin,
			NULL,
			in_x, in_y, in_z,
			kern_x, kern_y, kern_num,
			1,
			conv_method,
			pBPnn->Train.mini_batch_max,
			pmem,
			info
		);

		pnode2 = OSlwToolNNLayerActFunNew(
			pnode1->out.a, pout,
			mat_out_col, pBPnn->Train.mini_batch_max,
			pmem, pTemplet, 0
		);

		pln1 = pmem->Calloc(pmem, sizeof(OSlwToolDListNodeSTU));
		ppLIST1 = pmem->Malloc(pmem, sizeof(OSlwToolNNSubLayerBasicSTU *) * 2);


		ppLIST1[0] = pnode1;
		ppLIST1[1] = pnode2;

		OSlwToolBPnnLayerAppend(pBPnn, pln1, 2, ppLIST1);

		//每次都要复制输出层
		memcpy(&(pBPnn->y), &(pnode2->out), sizeof(OSlwMat));

		pcv = (void *)pnode1;
		pfc = (void *)pnode1;
	}
	else//采用不带激活函数的方式
	{

		_NN_GET_IN(pBPnn, pin);

		pnode1 = OSlwToolNNLayerConvNew(
			pin,
			pout,
			in_x, in_y, in_z,
			kern_x, kern_y, kern_num,
			1,
			conv_method,
			pBPnn->Train.mini_batch_max,
			pmem,
			info
		);

		pln1 = pmem->Calloc(pmem, sizeof(OSlwToolDListNodeSTU));


		ppLIST1 = pmem->Malloc(pmem, sizeof(OSlwToolNNSubLayerBasicSTU *) * 1);
		ppLIST1[0] = pnode1;

		OSlwToolBPnnLayerAppend(pBPnn, pln1, 1, ppLIST1);

		//每次都要复制输出层
		memcpy(&(pBPnn->y), &(pnode1->out), sizeof(OSlwMat));

		pcv = (void *)pnode1;
		pfc = (void *)pnode1;
	}


	pfc->pfun = pfun;
	pfc->pr = pr;
	pfc->initd1 = d1;
	pfc->initd2 = d2;

	_NN_FULL_CON_CHIP_ALLAC_1(pBPnn, pfc);

	//pBPnn->ParaGroupNum++;

	return ppLIST1;
}


void* OSlwToolBPnnPoolAppend
(
	OSlwToolBPnnSTU *pBPnn,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 pool_x, lw_u16 pool_y,
	ParaType *pin, ParaType *pout,
	OSlwToolNNPoolingMethodNUM pool_method,
	OSlwMemoryBasicSTU *pmem,
	lw_u32 info[4]
)
{

	OSlwToolNNSubLayerBasicSTU *pnode1;
	OSlwToolNNLayerPoolSTU *pPL;
	OSlwToolDListNodeSTU *pln1;
	OSlwToolNNSubLayerBasicSTU **ppLIST1, **pptail;
	LwMatColType in_col;


	OSLW_assert(!(pBPnn));
	OSLW_assert(!(pmem));

	in_col = in_x*in_y*in_z;

	_NN_GET_IN(pBPnn, pin);

	pnode1 = OSlwToolNNLayerPoolNew(
		pin,
		pout,
		in_x, in_y, in_z,
		pool_x, pool_x,
		pool_method,
		pBPnn->Train.mini_batch_max,
		pmem,
		info
	);

	pln1 = pmem->Calloc(pmem, sizeof(OSlwToolDListNodeSTU));
	ppLIST1 = pmem->Malloc(pmem, sizeof(OSlwToolNNSubLayerBasicSTU *) * 1);

	ppLIST1[0] = pnode1;

	OSlwToolBPnnLayerAppend(pBPnn, pln1, 1, ppLIST1);

	//每次都要复制输出层
	memcpy(&(pBPnn->y), &(pnode1->out), sizeof(OSlwMat));

	return ppLIST1;
}


#endif // OSLW_TOOL_IMPORT_NN_BPnn || OSLW_TOOL_IMPORT_ALL

#endif // OSLW_TOOL_IMPORT_NN || OSLW_TOOL_IMPORT_ALL

#endif // !(OSLW_SIMPLE_LEVEL >= 2)

