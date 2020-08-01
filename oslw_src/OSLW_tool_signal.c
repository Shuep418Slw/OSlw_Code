/*(Ver.=0.97)
* OSLW_tool.c
*
*  Created on: 2018-02-22
*      Author: slw
*/

#include "OSLW_include.h"
#if !(OSLW_SIMPLE_LEVEL >= 3)


ParaType OSlwToolSignalFirRun(OSlwToolSignalFirSTU *pfir, ParaType xin)
{
	lw_u32 i, t_div, t_mod, d;

	ParaType *p1, *p2, _sum;
	OSLW_assert(!(pfir));

	p1 = pfir->M_x;
	p2 = pfir->P_Num;
	d = pfir->Order;

	for (i = d - 1; i > 0; i--)
	{
		p1[i] = p1[i - 1];
	}
	p1[0] = xin;

	t_div = d / CACHE_READ_NUM, t_mod = d%CACHE_READ_NUM;

	LwVectDot(_sum, p1, p2, t_div, t_mod);

	return _sum;
}


void *OSlwToolSignalFirNew(OSlwToolSignalFirSTU *pfir, lw_u32 order, ParaType *num, OSlwMemoryBasicSTU *pmem)
{
	OSLW_assert(!(pmem));
	if (pfir == NULL)
	{
		pfir = pmem->Malloc(pmem, sizeof(OSlwToolSignalFirSTU));
	}

	pfir->Order = order;
	pfir->M_x = pmem->Calloc(pmem, PARA_MEM_CAL(order));
	pfir->P_Num = pmem->Calloc(pmem, PARA_MEM_CAL(order));

	if (num)
	{
		memcpy(pfir->P_Num, num, PARA_MEM_CAL(order));
	}
	return pfir;
}

ParaType _OSlwToolSignalOCFilter1d_Corrode(ParaType *din, ParaType *graph, ParaType x, lw_u32 n, ParaType *temp)
{
	lw_u32 i;
	ParaType m;
	ParaType *d = din + n;

	//求差值
	for (i = 0; i < n; i++)
	{
		temp[i] = _ParaSub(d[i], graph[i]);
	}

	//求最值
	m = temp[0];
	for (i = 0; i < n; i++)
	{
		if (m >= temp[i])
			m = temp[i];
	}
	//滑窗
	for (i = 1; i < 2 * n; i++)
	{
		din[i - 1] = din[i];
	}
	din[2 * n - 1] = x;

	return m;

}

ParaType _OSlwToolSignalOCFilter1d_Expand(ParaType *din, ParaType *graph, ParaType x, lw_u32 n, ParaType *temp)
{
	lw_u32 i;
	ParaType m;
	ParaType *d = din;
	//求差值
	for (i = 0; i < n; i++)
	{
		temp[i] = _ParaAdd(d[i], graph[i]);//按照公式应该是d[t-i]+g[i] 就是数组应该倒叙，但是因为g一般对称所以简化
	}

	//求最值
	m = temp[0];
	for (i = 0; i < n; i++)
	{
		if (m <= temp[i])
			m = temp[i];
	}
	//滑窗
	for (i = 1; i < 2 * n; i++)
	{
		din[i - 1] = din[i];
	}
	din[2 * n - 1] = x;
	return m;
}


ParaType OSlwToolSignalOCFilter1dRun(OSlwToolSignalOCFilterSTU *f, ParaType xin)
{
	ParaType temp;

	temp = _OSlwToolSignalOCFilter1d_Corrode(f->reg1, f->graph, xin, f->Order, f->data_buf);
	temp = _OSlwToolSignalOCFilter1d_Expand(f->reg2, f->graph, temp, f->Order, f->data_buf);
	temp = _OSlwToolSignalOCFilter1d_Expand(f->reg3, f->graph, temp, f->Order, f->data_buf);
	temp = _OSlwToolSignalOCFilter1d_Corrode(f->reg4, f->graph, temp, f->Order, f->data_buf);

	return temp;
}


void *OSlwToolSignalOCFilterNew(OSlwToolSignalOCFilterSTU *f, lw_u32 order, ParaType *graph, OSlwMemoryBasicSTU *pmem)
{
	OSLW_assert(!(pmem));
	if (f == NULL)
	{
		f = pmem->Malloc(pmem, sizeof(OSlwToolSignalOCFilterSTU));
	}

	f->Order = order;
	f->reg1 = pmem->Calloc(pmem, PARA_MEM_CAL(2 * order));
	f->reg2 = pmem->Calloc(pmem, PARA_MEM_CAL(2 * order));
	f->reg3 = pmem->Calloc(pmem, PARA_MEM_CAL(2 * order));
	f->reg4 = pmem->Calloc(pmem, PARA_MEM_CAL(2 * order));

	f->data_buf = pmem->Calloc(pmem, PARA_MEM_CAL(order));

	f->graph = pmem->Calloc(pmem, PARA_MEM_CAL(order));

	if (graph)
	{
		memcpy(f->graph, graph, PARA_MEM_CAL(order));
	}

	return f;
}

//
//LwMatLenType _SignalInterpFind(ParaType *x_point,LwMatLenType len, ParaType xnow)
//{
//	//二分法搜索
//	LwMatLenType  Len, count = 0;
//	LwMatLenType _s, _m, _m2, _e;
//	
//	Len = len - 1;
//
//	if (xnow<=x_point[0]){
//		return 0;
//	}
//	else if (xnow>=x_point[Len-1]){
//		return Len;
//	}
//	else;
//
//	while (Len)
//	{
//		switch (Len)
//		{
//		case 1:
//			return count + 1;
//
//		case 2:
//		//只要比较一次
//		// c c+1 c+2 三个数 比c+1就行了
//			if (xnow >= x_point[count + 1])
//				return count + 2;
//			else
//				return count + 1;
//		default:
//			//奇数
//			if (Len % 2)
//			{
//				_s = count;
//				_m = count + (Len >> 1);
//				_e = count + Len - 1;
//				if (xnow >= x_point[_s] && xnow<x_point[_m]) {
//					count = _s;
//				}
//				else {
//					count = _m;
//				}
//			}
//			else{
//				_s = count;
//				_m = count + (Len >> 1) - 1;
//				_m2 = count + (Len >> 1);
//				_e = count + Len - 1;
//
//				if (xnow >= x_point[_s] && xnow<x_point[_m]) {
//					count = _s;
//				}
//				else if (xnow >= x_point[_m] && xnow<x_point[_m2]) {//直接定位
//					return _m2;
//				}
//				else {
//					count = _m2;
//				}
//			}
//			break;
//		}
//		Len >>= 1;
//	}
//
//	return count + 1;
//}



LwMatLenType _SignalInterpFind(ParaType *x_point, LwMatLenType len, ParaType xnow)
{
	//二分法搜索
	LwMatLenType  Len, count = 0, lent;
	LwMatLenType _s, _m, _m2;

	Len = len - 1;

	if (xnow <= x_point[0]) {
		return 0;
	}
	else if (xnow >= x_point[Len - 1]) {
		return Len;
	}
	else;

	while (Len)
	{
		switch (Len)
		{
		case 1:
			return count + 1;

		case 2:
			//只要比较一次
			// c c+1 c+2 三个数 比c+1就行了
			if (xnow >= x_point[count + 1])
				return count + 2;
			else
				return count + 1;
		default:
			//奇数
			if (Len % 2)
			{
				lent = Len >> 1;
				_s = count;
				_m = count + lent;
				if (xnow < x_point[_m]) {
					count = _s;
				}
				else {
					count = _m;
				}
			}
			else {
				lent = Len >> 1;
				_s = count;
				_m = count + lent - 1;
				_m2 = count + lent;

				if (xnow < x_point[_m]) {
					count = _s;
				}
				else if (xnow >= x_point[_m] && xnow < x_point[_m2]) {//直接定位
					return _m2;
				}
				else {
					count = _m2;
				}
			}
			break;
		}
		Len = lent + 1;
	}

	return count + 1;
}

LwMatLenType OSlwToolSignalInterpFind(OSlwToolSignalInterpBasicSTU *p, ParaType xnow)
{
	OSLW_assert(!(p));
	return _SignalInterpFind(p->x_point, p->Len, xnow);
}


ParaType OSlwToolSignalInterpRun(OSlwToolSignalInterpBasicSTU *p, ParaType xnow)
{
	LwMatLenType i;
	OSLW_assert(!(p));
	i = p->_interp_find(p, xnow);
	return p->_interp_with_zoom(p, xnow, i);
}


ParaType OSlwToolSignalInterpIterGet(OSlwToolSignalInterpIterSTU *p, ParaType xnow)
{

	LwMatLenType Len;
	OSlwToolSignalInterpBasicSTU *pinter;

	Len = p->pInterp->Len;
	pinter = p->pInterp;

	if (p->move_now == 0)
	{
		//判断是否满足条件
		if (xnow < pinter->x_point[0]) {
			return pinter->_interp_with_zoom(pinter, xnow, 0);
		}
		else if (xnow >= pinter->x_point[0] && xnow < pinter->x_point[1])//因为至少2个点 3个区间 0后面一定不是最后区间
		{
			p->move_now++;
			return pinter->_interp_with_zoom(pinter, xnow, 1);
		}
	}
	else if (p->move_now == Len - 1)
	{
		//判断是否满足条件
		//记住 断点比区间少一个
		if (xnow > pinter->x_point[Len - 2]) {
			return pinter->_interp_with_zoom(pinter, xnow, p->move_now);
		}
	}
	else
	{
		//判断是否满足条件
		if (xnow >= pinter->x_point[p->move_now - 1] && xnow < pinter->x_point[p->move_now]) {
			return pinter->_interp_with_zoom(pinter, xnow, p->move_now);
		}
		else
		{
			//下一个是最后区间
			if (p->move_now == Len - 2) {
				//比下一个点大就可以了
				if (xnow >= pinter->x_point[Len - 1]) {
					p->move_now++;
					return pinter->_interp_with_zoom(pinter, xnow, p->move_now);
				}

			}
			else
			{
				//在下一个区间吗?
				if (xnow >= pinter->x_point[p->move_now] && xnow < pinter->x_point[p->move_now + 1]) {
					p->move_now++;
					return pinter->_interp_with_zoom(pinter, xnow, p->move_now);
				}
			}
		}
	}

	//都不是就重新搜索
	p->move_now = pinter->_interp_find(pinter, xnow);

	return pinter->_interp_with_zoom(pinter, xnow, p->move_now);
}


ParaType OSlwToolSignalSplineInterpZoom(OSlwToolSignalInterpBasicSTU *p, ParaType xnow, LwMatLenType now_zoom)
{
	ParaType xtemp;
	ParaType y;
	ParaType a3, a2, a1, a0;
	OSLW_assert(!(p));

	//先保存高次项
	a3 = *(p->coff + now_zoom * 4 + 0);
	a2 = *(p->coff + now_zoom * 4 + 1);
	a1 = *(p->coff + now_zoom * 4 + 2);
	a0 = *(p->coff + now_zoom * 4 + 3);

	if (now_zoom == 0) {
		xtemp = _ParaSub(xnow, p->x_point[0]);
	}
	else if (now_zoom == p->Len - 1) {
		xtemp = _ParaSub(xnow, p->x_point[now_zoom - 2]);

	}
	else {
		xtemp = _ParaSub(xnow, p->x_point[now_zoom - 1]);
	}

	y = _ParaAdd(_ParaMpy(a3, xtemp), a2);
	y = _ParaAdd(_ParaMpy(y, xtemp), a1);
	y = _ParaAdd(_ParaMpy(y, xtemp), a0);


	return y;
}


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
)
{
	lw_u32 Len = x_y_len + 1;
	lw_u32 i;
	ParaType a3, a2, a1, a0;

	LwMat D;
	LwMat L;
	LwMat U;
	LwMat M;
	LwMat F;

	ParaType h1, h2;
	ParaType dy1, dy2;
	ParaType dm;
	ParaType *coff0;
	ParaType onedivh;

	OSlwMatSolveStatusNUM s_s;
	OSLW_assert(!(p));
	OSLW_assert(!(xin));
	OSLW_assert(!(yout));
	OSLW_assert(!(x_y_len));
	OSLW_assert(!(spline_coff));
	OSLW_assert(!(Da));
	OSLW_assert(!(La));
	OSLW_assert(!(Ua));
	OSLW_assert(!(Ma));
	OSLW_assert(!(Fa));

	//测试是不是单调的
	for (i = 1; i < x_y_len; i++)
	{
		if (xin[i] < xin[i - 1]) {
			return 1;
		}
	}

	LwMatInit(&D, 1, x_y_len, Da);
	LwMatInit(&L, 1, x_y_len - 1, La);
	LwMatInit(&U, 1, x_y_len - 1, Ua);
	LwMatInit(&M, 1, x_y_len, Ma);
	LwMatInit(&F, 1, x_y_len, Fa);

	if (dy)
	{
		p->dy0 = dy[0];
		p->dyn = dy[1];
	}
	else
	{
		p->dy0 = 0;
		p->dyn = 0;
	}

	p->interp1_basic.coff = spline_coff;
	p->interp1_basic.Len = Len;
	p->interp1_basic._interp_find = OSlwToolSignalInterpFind;
	p->interp1_basic.Interp = OSlwToolSignalInterpRun;
	p->interp1_basic.x_point = xin;
	p->interp1_basic._interp_with_zoom = OSlwToolSignalSplineInterpZoom;

	for (i = 0; i < x_y_len - 2; i++)
	{
		h1 = xin[i + 1] - xin[i];
		h2 = xin[i + 2] - xin[i + 1];

		dy1 = yout[i + 1] - yout[i];
		dy2 = yout[i + 2] - yout[i + 1];

		Da[i + 1] = _ParaMpy((h1 + h2), _ParaFrom(2));
		La[i] = h1;
		Ua[i + 1] = h2;
		Fa[i + 1] = _ParaMpy(_ParaDiv(dy2, h2) - _ParaDiv(dy1, h1), _ParaFrom(6));
	}

	//指定导数
	if (dy)
	{
		h1 = xin[1] - xin[0];
		h2 = xin[x_y_len - 1] - xin[x_y_len - 2];

		dy1 = yout[1] - yout[0];
		dy2 = yout[x_y_len - 1] - yout[x_y_len - 2];


		Da[0] = _ParaMpy(_ParaFrom(2), h1);
		Da[x_y_len - 1] = _ParaMpy(_ParaFrom(2), h2);

		Ua[0] = h1;
		La[x_y_len - 2] = h2;

		Fa[0] = _ParaMpy((_ParaDiv(dy1, h1) - dy[0]), _ParaFrom(6));
		Fa[x_y_len - 1] = _ParaMpy((dy[1] - _ParaDiv(dy2, h1)), _ParaFrom(6));
	}
	else
	{
		//自然边界
		Da[0] = _ParaFrom(1);
		Da[x_y_len - 1] = _ParaFrom(1);

		Ua[0] = _ParaFint(0);
		La[x_y_len - 2] = _ParaFint(0);

		Fa[0] = _ParaFrom(0);
		Fa[x_y_len - 1] = _ParaFrom(0);
	}


	s_s = pOSlwToolMatrixSolveCrout(&M, &L, &D, &U, &F);
	if (s_s != OSlwMatSolveSucces) {
		return 2;
	}

	coff0 = spline_coff;
	spline_coff += 4;

	for (i = 1; i < x_y_len; i++)
	{
		h1 = xin[i] - xin[i - 1];
		dy1 = yout[i] - yout[i - 1];
		dm = (Ma[i] - Ma[i - 1]);
		onedivh = _ParaDiv(_ParaFrom(1), h1);

		a3 = _ParaMpy(_ParaMpy(dm, onedivh), _ParaFrom(1.0 / 6.0));
		a2 = _ParaMpy(Ma[i - 1], _ParaFrom(0.5));
		a1 = _ParaMpy(dy1, onedivh) - _ParaMpy(_ParaFrom(1.0 / 6.0), _ParaMpy(_ParaFrom(2), _ParaMpy(h1, Ma[i - 1])) + _ParaMpy(h1, Ma[i]));
		a0 = yout[i - 1];

		*spline_coff++ = a3;
		*spline_coff++ = a2;
		*spline_coff++ = a1;
		*spline_coff++ = a0;
	}


	coff0[0] = coff0[4];coff0[1] = coff0[5];coff0[2] = coff0[6];coff0[3] = coff0[7];

	spline_coff[0] = spline_coff[-4];
	spline_coff[1] = spline_coff[-3];
	spline_coff[2] = spline_coff[-2];
	spline_coff[3] = spline_coff[-1];

	return 0;

}

OSlwToolSignalSplineSTU *OSlwToolSignalSplineNew(
	OSlwToolSignalSplineSTU *p,
	ParaType *xin, ParaType *yout, LwMatLenType x_y_len,
	ParaType *dy, ParaType *spline_coff, lw_mem mem_ptr
)
{
	ParaType *x, *y;
	ParaType *Da;//crout法的主对角线
	ParaType *La;//crout法的下对角线
	ParaType *Ua;//crout法的上对角线
	ParaType *Ma;//crout法的X
	ParaType *Fa;//crout法的Y
	lw_u32 size_of_len = PARA_MEM_CAL(x_y_len);
	lw_16 re;
	OSLW_assert(!(mem_ptr));
	OSLW_assert(!(xin));
	OSLW_assert(!(yout));
	OSLW_assert(!(x_y_len));

	//如果p是空 就会分配内存空间并且复制x与y
	if (p == NULL) {
		p = mem_ptr->Malloc(mem_ptr, sizeof(OSlwToolSignalSplineSTU));
		x = mem_ptr->Malloc(mem_ptr, size_of_len);
		y = mem_ptr->Malloc(mem_ptr, size_of_len);
		memcpy(x, xin, size_of_len);
		memcpy(y, yout, size_of_len);
	}
	else {
		x = xin;y = yout;
	}

	//分配内存空间
	if (spline_coff == NULL) {
		spline_coff = mem_ptr->Malloc(mem_ptr, PARA_MEM_CAL(x_y_len * 4));
	}

	Da = mem_ptr->Malloc(mem_ptr, size_of_len);
	Ma = mem_ptr->Malloc(mem_ptr, size_of_len);
	Fa = mem_ptr->Malloc(mem_ptr, size_of_len);

	La = mem_ptr->Malloc(mem_ptr, PARA_MEM_CAL(x_y_len - 1));
	Ua = mem_ptr->Malloc(mem_ptr, PARA_MEM_CAL(x_y_len - 1));

	re = OSlwToolSignalSplineInitStatic(
		p,
		x, y, x_y_len, dy,
		spline_coff, Da, La, Ua, Ma, Fa
	);

	mem_ptr->Free(mem_ptr, Da);
	mem_ptr->Free(mem_ptr, Ma);
	mem_ptr->Free(mem_ptr, Fa);
	mem_ptr->Free(mem_ptr, La);
	mem_ptr->Free(mem_ptr, Ua);

	//初始化失败
	if (re) {
		OSLW_assert(1);
		mem_ptr->Free(mem_ptr, x);
		mem_ptr->Free(mem_ptr, y);
		mem_ptr->Free(mem_ptr, spline_coff);
		mem_ptr->Free(mem_ptr, p);
		return NULL;
	}
	else
	{
		return p;
	}
}


inline lw_u32 _CentDataIndex(lw_32 getId, lw_u32 len, lw_u32 period)
{
	if (getId < 0)
		getId = -getId;
	getId %= period;
	if ((lw_u32)getId < (lw_u32)len)
		return getId;
	else
		return period - getId;
}



void _OSlwToolSignal_FFT_BitRev(
	ParaType * pSrc,
	uint16_t FFT_len,
	uint16_t BitMove,
	uint16_t *BitRevTable
)
{
	lw_u32 LenDiv2, LenDiv2Add1;
	lw_u32 i, j;
	ParaType temp;
	j = 0;
	LenDiv2 = FFT_len >> 1;
	LenDiv2Add1 = (FFT_len >> 1) + 1;

	for (i = 0; i <= (LenDiv2 - 2); i += 2)
	{
		if (i < j)
		{
			temp = pSrc[2 * i];
			pSrc[2 * i] = pSrc[2 * j];
			pSrc[2 * j] = temp;

			temp = pSrc[(2 * i) + 1];
			pSrc[(2 * i) + 1] = pSrc[(2 * j) + 1];
			pSrc[(2 * j) + 1] = temp;

			temp = pSrc[2 * (i + LenDiv2Add1)];
			pSrc[2 * (i + LenDiv2Add1)] = pSrc[2 * (j + LenDiv2Add1)];
			pSrc[2 * (j + LenDiv2Add1)] = temp;

			temp = pSrc[(2 * (i + LenDiv2Add1)) + 1];
			pSrc[(2 * (i + LenDiv2Add1)) + 1] =
				pSrc[(2 * (j + LenDiv2Add1)) + 1];
			pSrc[(2 * (j + LenDiv2Add1)) + 1] = temp;
		}

		temp = pSrc[2 * (i + 1)];
		pSrc[2 * (i + 1)] = pSrc[2 * (j + LenDiv2)];
		pSrc[2 * (j + LenDiv2)] = temp;

		temp = pSrc[(2 * (i + 1)) + 1];
		pSrc[(2 * (i + 1)) + 1] = pSrc[(2 * (j + LenDiv2)) + 1];
		pSrc[(2 * (j + LenDiv2)) + 1] = temp;
		j = *BitRevTable;
		BitRevTable += BitMove;
	}
}


void _OSlwToolSignal_BF_R4(
	ParaType * pSrc,
	lw_u16 FFT_len,
	ParaType * Tw,
	lw_u16 TwMove)
{
	ParaType co1, co2, co3, si1, si2, si3;
	lw_u32 ia1, ia2, ia3;
	lw_u32 i0, i1, i2, i3;
	lw_u32 n1, n2, j, k;

	ParaType xaIn, yaIn, xbIn, ybIn, xcIn, ycIn, xdIn, ydIn;
	ParaType Xa_add_c, Xb_add_d, Ya_add_c, Yb_add_d, Xa_mpy_c, Xb_mpy_d, Ya_mpy_c,
		Yb_mpy_d;
	ParaType Xb12Co, Yb12Co, Xc12Co, Yc12Co, Xd12Co, Yd12Co;
	ParaType Xb12o, Yb12o, Xc12o, Yc12o, Xd12o, Yd12o;
	ParaType *ptr1;
	ParaType p0, p1, p2, p3, p4, p5;
	ParaType a0, a1, a2, a3, a4, a5, a6, a7;
	n2 = FFT_len;
	n1 = n2;
	n2 >>= 2u;
	i0 = 0u;
	ia1 = 0u;
	j = n2;
	do
	{
		i1 = i0 + n2;
		i2 = i1 + n2;
		i3 = i2 + n2;
		xaIn = pSrc[(2u * i0)];
		yaIn = pSrc[(2u * i0) + 1u];
		xbIn = pSrc[(2u * i1)];
		ybIn = pSrc[(2u * i1) + 1u];
		xcIn = pSrc[(2u * i2)];
		ycIn = pSrc[(2u * i2) + 1u];
		xdIn = pSrc[(2u * i3)];
		ydIn = pSrc[(2u * i3) + 1u];
		Xa_add_c = xaIn + xcIn;
		Xb_add_d = xbIn + xdIn;
		Ya_add_c = yaIn + ycIn;
		Yb_add_d = ybIn + ydIn;
		ia2 = ia1 + ia1;
		co2 = Tw[ia2 * 2u];
		si2 = Tw[(ia2 * 2u) + 1u];
		Xa_mpy_c = xaIn - xcIn;
		Xb_mpy_d = xbIn - xdIn;
		Ya_mpy_c = yaIn - ycIn;
		Yb_mpy_d = ybIn - ydIn;
		pSrc[(2u * i0)] = Xa_add_c + Xb_add_d;
		pSrc[(2u * i0) + 1u] = Ya_add_c + Yb_add_d;
		Xb12Co = (Xa_mpy_c + Yb_mpy_d);
		Yb12Co = (Ya_mpy_c - Xb_mpy_d);
		Xc12Co = (Xa_add_c - Xb_add_d);
		Yc12Co = (Ya_add_c - Yb_add_d);
		Xd12Co = (Xa_mpy_c - Yb_mpy_d);
		Yd12Co = (Xb_mpy_d + Ya_mpy_c);
		co1 = Tw[ia1 * 2u];
		si1 = Tw[(ia1 * 2u) + 1u];
		ia3 = ia2 + ia1;
		co3 = Tw[ia3 * 2u];
		si3 = Tw[(ia3 * 2u) + 1u];
		Xb12o = _ParaMpy(Xb12Co, co1);
		Yb12o = _ParaMpy(Yb12Co, co1);
		Xc12o = _ParaMpy(Xc12Co, co2);
		Yc12o = _ParaMpy(Yc12Co, co2);
		Xd12o = _ParaMpy(Xd12Co, co3);
		Yd12o = _ParaMpy(Yd12Co, co3);
		p0 = _ParaMpy(Yb12Co, si1);
		p1 = _ParaMpy(Xb12Co, si1);
		p2 = _ParaMpy(Yc12Co, si2);
		p3 = _ParaMpy(Xc12Co, si2);
		p4 = _ParaMpy(Yd12Co, si3);
		p5 = _ParaMpy(Xd12Co, si3);
		Xb12o += p0;
		Yb12o -= p1;
		Xc12o += p2;
		Yc12o -= p3;
		Xd12o += p4;
		Yd12o -= p5;
		pSrc[2u * i1] = Xc12o;
		pSrc[(2u * i1) + 1u] = Yc12o;
		pSrc[2u * i2] = Xb12o;
		pSrc[(2u * i2) + 1u] = Yb12o;
		pSrc[2u * i3] = Xd12o;
		pSrc[(2u * i3) + 1u] = Yd12o;
		ia1 += TwMove;
		i0++;
	} while (--j);
	TwMove <<= 2u;
	for (k = FFT_len >> 2u; k > 4u; k >>= 2u)
	{
		n1 = n2;
		n2 >>= 2u;
		ia1 = 0u;
		j = 0;
		do
		{
			ia2 = ia1 + ia1;
			ia3 = ia2 + ia1;
			co1 = Tw[ia1 * 2u];
			si1 = Tw[(ia1 * 2u) + 1u];
			co2 = Tw[ia2 * 2u];
			si2 = Tw[(ia2 * 2u) + 1u];
			co3 = Tw[ia3 * 2u];
			si3 = Tw[(ia3 * 2u) + 1u];
			ia1 += TwMove;
			i0 = j;
			do
			{
				i1 = i0 + n2;
				i2 = i1 + n2;
				i3 = i2 + n2;
				xaIn = pSrc[(2u * i0)];
				yaIn = pSrc[(2u * i0) + 1u];
				xbIn = pSrc[(2u * i1)];
				ybIn = pSrc[(2u * i1) + 1u];
				xcIn = pSrc[(2u * i2)];
				ycIn = pSrc[(2u * i2) + 1u];
				xdIn = pSrc[(2u * i3)];
				ydIn = pSrc[(2u * i3) + 1u];
				Xa_mpy_c = xaIn - xcIn;
				Xb_mpy_d = xbIn - xdIn;
				Ya_mpy_c = yaIn - ycIn;
				Yb_mpy_d = ybIn - ydIn;
				Xa_add_c = xaIn + xcIn;
				Xb_add_d = xbIn + xdIn;
				Ya_add_c = yaIn + ycIn;
				Yb_add_d = ybIn + ydIn;
				Xb12Co = (Xa_mpy_c + Yb_mpy_d);
				Yb12Co = (Ya_mpy_c - Xb_mpy_d);
				Xc12Co = (Xa_add_c - Xb_add_d);
				Yc12Co = (Ya_add_c - Yb_add_d);
				Xd12Co = (Xa_mpy_c - Yb_mpy_d);
				Yd12Co = (Xb_mpy_d + Ya_mpy_c);
				pSrc[(2u * i0)] = Xa_add_c + Xb_add_d;
				pSrc[(2u * i0) + 1u] = Ya_add_c + Yb_add_d;
				Xb12o = _ParaMpy(Xb12Co, co1);
				Yb12o = _ParaMpy(Yb12Co, co1);
				Xc12o = _ParaMpy(Xc12Co, co2);
				Yc12o = _ParaMpy(Yc12Co, co2);
				Xd12o = _ParaMpy(Xd12Co, co3);
				Yd12o = _ParaMpy(Yd12Co, co3);
				p0 = _ParaMpy(Yb12Co, si1);
				p1 = _ParaMpy(Xb12Co, si1);
				p2 = _ParaMpy(Yc12Co, si2);
				p3 = _ParaMpy(Xc12Co, si2);
				p4 = _ParaMpy(Yd12Co, si3);
				p5 = _ParaMpy(Xd12Co, si3);
				Xb12o += p0;
				Yb12o -= p1;
				Xc12o += p2;
				Yc12o -= p3;
				Xd12o += p4;
				Yd12o -= p5;
				pSrc[2u * i1] = Xc12o;
				pSrc[(2u * i1) + 1u] = Yc12o;
				pSrc[2u * i2] = Xb12o;
				pSrc[(2u * i2) + 1u] = Yb12o;
				pSrc[2u * i3] = Xd12o;
				pSrc[(2u * i3) + 1u] = Yd12o;
				i0 += n1;
			} while (i0 < FFT_len);
			j++;
		} while (j <= (n2 - 1u));
		TwMove <<= 2u;
	}
	j = FFT_len >> 2;
	ptr1 = &pSrc[0];
	do
	{
		xaIn = ptr1[0];
		yaIn = ptr1[1];
		xbIn = ptr1[2];
		ybIn = ptr1[3];
		xcIn = ptr1[4];
		ycIn = ptr1[5];
		xdIn = ptr1[6];
		ydIn = ptr1[7];
		Xa_add_c = xaIn + xcIn;
		Xa_mpy_c = xaIn - xcIn;
		Ya_add_c = yaIn + ycIn;
		Ya_mpy_c = yaIn - ycIn;
		Xb_add_d = xbIn + xdIn;
		Yb_add_d = ybIn + ydIn;
		Xb_mpy_d = xbIn - xdIn;
		Yb_mpy_d = ybIn - ydIn;
		a0 = (Xa_add_c + Xb_add_d);
		a1 = (Ya_add_c + Yb_add_d);
		a2 = (Xa_add_c - Xb_add_d);
		a3 = (Ya_add_c - Yb_add_d);
		a4 = (Xa_mpy_c + Yb_mpy_d);
		a5 = (Ya_mpy_c - Xb_mpy_d);
		a6 = (Xa_mpy_c - Yb_mpy_d);
		a7 = (Xb_mpy_d + Ya_mpy_c);
		ptr1[0] = a0;
		ptr1[1] = a1;
		ptr1[2] = a2;
		ptr1[3] = a3;
		ptr1[4] = a4;
		ptr1[5] = a5;
		ptr1[6] = a6;
		ptr1[7] = a7;
		ptr1 += 8u;
	} while (--j);
}


void _OSlwToolSignal_IBF_R4(
	ParaType * pSrc,
	lw_u16 FFT_len,
	ParaType * Tw,
	lw_u16 TwMove,
	ParaType oneDivLen)
{
	ParaType co1, co2, co3, si1, si2, si3;
	lw_u32 ia1, ia2, ia3;
	lw_u32 i0, i1, i2, i3;
	lw_u32 n1, n2, j, k;
	ParaType xa_t, ya_t, xb_t, yb_t, xc_t, yc_t, xd_t, yd_t;
	ParaType Xa_add_c, Xb_add_d, Ya_add_c, Yb_add_d, Xa_mpy_c, Xb_mpy_d, Ya_mpy_c,
		Yb_mpy_d;
	ParaType Xb12Co, Yb12Co, Xc12Co, Yc12Co, Xd12Co, Yd12Co;
	ParaType Xb12o, Yb12o, Xc12o, Yc12o, Xd12o, Yd12o;
	ParaType *ptr1;
	ParaType p0, p1, p2, p3, p4, p5, p6, p7;
	ParaType a0, a1, a2, a3, a4, a5, a6, a7;
	n2 = FFT_len;
	n1 = n2;
	n2 >>= 2u;
	i0 = 0u;
	ia1 = 0u;
	j = n2;
	do
	{
		i1 = i0 + n2;
		i2 = i1 + n2;
		i3 = i2 + n2;
		xa_t = pSrc[(2u * i0)];
		ya_t = pSrc[(2u * i0) + 1u];
		xc_t = pSrc[(2u * i2)];
		yc_t = pSrc[(2u * i2) + 1u];
		xb_t = pSrc[(2u * i1)];
		yb_t = pSrc[(2u * i1) + 1u];
		xd_t = pSrc[(2u * i3)];
		yd_t = pSrc[(2u * i3) + 1u];
		Xa_add_c = xa_t + xc_t;
		Xb_add_d = xb_t + xd_t;
		Ya_add_c = ya_t + yc_t;
		Yb_add_d = yb_t + yd_t;
		ia2 = ia1 + ia1;
		co2 = Tw[ia2 * 2u];
		si2 = Tw[(ia2 * 2u) + 1u];
		Xa_mpy_c = xa_t - xc_t;
		Xb_mpy_d = xb_t - xd_t;
		Ya_mpy_c = ya_t - yc_t;
		Yb_mpy_d = yb_t - yd_t;
		pSrc[(2u * i0)] = Xa_add_c + Xb_add_d;
		pSrc[(2u * i0) + 1u] = Ya_add_c + Yb_add_d;
		Xb12Co = (Xa_mpy_c - Yb_mpy_d);
		Yb12Co = (Ya_mpy_c + Xb_mpy_d);
		Xc12Co = (Xa_add_c - Xb_add_d);
		Yc12Co = (Ya_add_c - Yb_add_d);
		Xd12Co = (Xa_mpy_c + Yb_mpy_d);
		Yd12Co = (Ya_mpy_c - Xb_mpy_d);
		co1 = Tw[ia1 * 2u];
		si1 = Tw[(ia1 * 2u) + 1u];
		ia3 = ia2 + ia1;
		co3 = Tw[ia3 * 2u];
		si3 = Tw[(ia3 * 2u) + 1u];
		Xb12o = _ParaMpy(Xb12Co, co1);
		Yb12o = _ParaMpy(Yb12Co, co1);
		Xc12o = _ParaMpy(Xc12Co, co2);
		Yc12o = _ParaMpy(Yc12Co, co2);
		Xd12o = _ParaMpy(Xd12Co, co3);
		Yd12o = _ParaMpy(Yd12Co, co3);
		p0 = _ParaMpy(Yb12Co, si1);
		p1 = _ParaMpy(Xb12Co, si1);
		p2 = _ParaMpy(Yc12Co, si2);
		p3 = _ParaMpy(Xc12Co, si2);
		p4 = _ParaMpy(Yd12Co, si3);
		p5 = _ParaMpy(Xd12Co, si3);
		Xb12o -= p0;
		Yb12o += p1;
		Xc12o -= p2;
		Yc12o += p3;
		Xd12o -= p4;
		Yd12o += p5;
		pSrc[2u * i1] = Xc12o;
		pSrc[(2u * i1) + 1u] = Yc12o;
		pSrc[2u * i2] = Xb12o;
		pSrc[(2u * i2) + 1u] = Yb12o;
		pSrc[2u * i3] = Xd12o;
		pSrc[(2u * i3) + 1u] = Yd12o;
		ia1 = ia1 + TwMove;
		i0 = i0 + 1u;
	} while (--j);
	TwMove <<= 2u;
	for (k = FFT_len >> 2u; k > 4u; k >>= 2u)
	{
		n1 = n2;
		n2 >>= 2u;
		ia1 = 0u;
		j = 0;
		do
		{
			ia2 = ia1 + ia1;
			ia3 = ia2 + ia1;
			co1 = Tw[ia1 * 2u];
			si1 = Tw[(ia1 * 2u) + 1u];
			co2 = Tw[ia2 * 2u];
			si2 = Tw[(ia2 * 2u) + 1u];
			co3 = Tw[ia3 * 2u];
			si3 = Tw[(ia3 * 2u) + 1u];
			ia1 = ia1 + TwMove;
			i0 = j;
			do
			{
				i1 = i0 + n2;
				i2 = i1 + n2;
				i3 = i2 + n2;
				xa_t = pSrc[(2u * i0)];
				ya_t = pSrc[(2u * i0) + 1u];
				xb_t = pSrc[(2u * i1)];
				yb_t = pSrc[(2u * i1) + 1u];
				xc_t = pSrc[(2u * i2)];
				yc_t = pSrc[(2u * i2) + 1u];
				xd_t = pSrc[(2u * i3)];
				yd_t = pSrc[(2u * i3) + 1u];
				Xa_mpy_c = xa_t - xc_t;
				Xb_mpy_d = xb_t - xd_t;
				Ya_mpy_c = ya_t - yc_t;
				Yb_mpy_d = yb_t - yd_t;
				Xa_add_c = xa_t + xc_t;
				Xb_add_d = xb_t + xd_t;
				Ya_add_c = ya_t + yc_t;
				Yb_add_d = yb_t + yd_t;
				Xb12Co = (Xa_mpy_c - Yb_mpy_d);
				Yb12Co = (Ya_mpy_c + Xb_mpy_d);
				Xc12Co = (Xa_add_c - Xb_add_d);
				Yc12Co = (Ya_add_c - Yb_add_d);
				Xd12Co = (Xa_mpy_c + Yb_mpy_d);
				Yd12Co = (Ya_mpy_c - Xb_mpy_d);
				pSrc[(2u * i0)] = Xa_add_c + Xb_add_d;
				pSrc[(2u * i0) + 1u] = Ya_add_c + Yb_add_d;
				Xb12o = _ParaMpy(Xb12Co, co1);
				Yb12o = _ParaMpy(Yb12Co, co1);
				Xc12o = _ParaMpy(Xc12Co, co2);
				Yc12o = _ParaMpy(Yc12Co, co2);
				Xd12o = _ParaMpy(Xd12Co, co3);
				Yd12o = _ParaMpy(Yd12Co, co3);
				p0 = _ParaMpy(Yb12Co, si1);
				p1 = _ParaMpy(Xb12Co, si1);
				p2 = _ParaMpy(Yc12Co, si2);
				p3 = _ParaMpy(Xc12Co, si2);
				p4 = _ParaMpy(Yd12Co, si3);
				p5 = _ParaMpy(Xd12Co, si3);
				Xb12o -= p0;
				Yb12o += p1;
				Xc12o -= p2;
				Yc12o += p3;
				Xd12o -= p4;
				Yd12o += p5;
				pSrc[2u * i1] = Xc12o;
				pSrc[(2u * i1) + 1u] = Yc12o;
				pSrc[2u * i2] = Xb12o;
				pSrc[(2u * i2) + 1u] = Yb12o;
				pSrc[2u * i3] = Xd12o;
				pSrc[(2u * i3) + 1u] = Yd12o;
				i0 += n1;
			} while (i0 < FFT_len);
			j++;
		} while (j <= (n2 - 1u));
		TwMove <<= 2u;
	}
	j = FFT_len >> 2;
	ptr1 = &pSrc[0];
	do
	{
		xa_t = ptr1[0];
		ya_t = ptr1[1];
		xb_t = ptr1[2];
		yb_t = ptr1[3];
		xc_t = ptr1[4];
		yc_t = ptr1[5];
		xd_t = ptr1[6];
		yd_t = ptr1[7];
		Xa_add_c = xa_t + xc_t;
		Xa_mpy_c = xa_t - xc_t;
		Ya_add_c = ya_t + yc_t;
		Ya_mpy_c = ya_t - yc_t;
		Xb_add_d = xb_t + xd_t;
		Yb_add_d = yb_t + yd_t;
		Xb_mpy_d = xb_t - xd_t;
		Yb_mpy_d = yb_t - yd_t;
		a0 = (Xa_add_c + Xb_add_d);
		a1 = (Ya_add_c + Yb_add_d);
		a2 = (Xa_add_c - Xb_add_d);
		a3 = (Ya_add_c - Yb_add_d);
		a4 = (Xa_mpy_c - Yb_mpy_d);
		a5 = (Ya_mpy_c + Xb_mpy_d);
		a6 = (Xa_mpy_c + Yb_mpy_d);
		a7 = (Ya_mpy_c - Xb_mpy_d);
		p0 = _ParaMpy(a0, oneDivLen);
		p1 = _ParaMpy(a1, oneDivLen);
		p2 = _ParaMpy(a2, oneDivLen);
		p3 = _ParaMpy(a3, oneDivLen);
		p4 = _ParaMpy(a4, oneDivLen);
		p5 = _ParaMpy(a5, oneDivLen);
		p6 = _ParaMpy(a6, oneDivLen);
		p7 = _ParaMpy(a7, oneDivLen);
		ptr1[0] = p0;
		ptr1[1] = p1;
		ptr1[2] = p2;
		ptr1[3] = p3;
		ptr1[4] = p4;
		ptr1[5] = p5;
		ptr1[6] = p6;
		ptr1[7] = p7;
		ptr1 = ptr1 + 8u;
	} while (--j);
}



void _OSlwToolSignal_BF_R2(
	ParaType * pSrc,
	lw_u32 FFT_len,
	ParaType * Tw,
	uint16_t TwMove)
{
	lw_u32 i, j, k, l;
	lw_u32 n1, n2, ia;
	ParaType xt, yt, cosVal, sinVal;
	ParaType p0, p1, p2, p3;
	ParaType a0, a1;

	n2 = FFT_len >> 1;
	ia = 0;
	i = 0;
	for (k = n2; k > 0; k--)
	{
		cosVal = Tw[ia * 2];
		sinVal = Tw[(ia * 2) + 1];

		ia += TwMove;

		l = i + n2;

		a0 = pSrc[2 * i] + pSrc[2 * l];
		xt = pSrc[2 * i] - pSrc[2 * l];
		yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];
		a1 = pSrc[2 * l + 1] + pSrc[2 * i + 1];
		p0 = _ParaMpy(xt, cosVal);
		p1 = _ParaMpy(yt, sinVal);
		p2 = _ParaMpy(yt, cosVal);
		p3 = _ParaMpy(xt, sinVal);
		pSrc[2 * i] = a0;
		pSrc[2 * i + 1] = a1;
		pSrc[2 * l] = p0 + p1;
		pSrc[2 * l + 1] = p2 - p3;
		i++;
	}

	TwMove <<= 1u;
	for (k = n2; k > 2; k = k >> 1)
	{
		n1 = n2;
		n2 = n2 >> 1;
		ia = 0;
		j = 0;
		do
		{
			cosVal = Tw[ia * 2];
			sinVal = Tw[(ia * 2) + 1];
			ia += TwMove;
			i = j;
			do
			{
				l = i + n2;
				a0 = pSrc[2 * i] + pSrc[2 * l];
				xt = pSrc[2 * i] - pSrc[2 * l];
				yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];
				a1 = pSrc[2 * l + 1] + pSrc[2 * i + 1];
				p0 = _ParaMpy(xt, cosVal);
				p1 = _ParaMpy(yt, sinVal);
				p2 = _ParaMpy(yt, cosVal);
				p3 = _ParaMpy(xt, sinVal);
				pSrc[2 * i] = a0;
				pSrc[2 * i + 1] = a1;
				pSrc[2 * l] = p0 + p1;
				pSrc[2 * l + 1] = p2 - p3;
				i += n1;
			} while (i < FFT_len);
			j++;
		} while (j < n2);
		TwMove <<= 1u;
	}

	for (i = 0; i < FFT_len; i += 2)
	{
		a0 = pSrc[2 * i] + pSrc[2 * i + 2];
		xt = pSrc[2 * i] - pSrc[2 * i + 2];
		yt = pSrc[2 * i + 1] - pSrc[2 * i + 3];
		a1 = pSrc[2 * i + 3] + pSrc[2 * i + 1];
		pSrc[2 * i] = a0;
		pSrc[2 * i + 1] = a1;
		pSrc[2 * i + 2] = xt;
		pSrc[2 * i + 3] = yt;
	}

}


void _OSlwToolSignal_IBF_R2(
	ParaType * pSrc,
	lw_u32 FFT_len,
	ParaType * Tw,
	uint16_t TwMove,
	ParaType oneDivLen)
{
	lw_u32 i, j, k, l;
	lw_u32 n1, n2, ia;
	ParaType xt, yt, cosVal, sinVal;
	ParaType p0, p1, p2, p3;
	ParaType a0, a1;
	n2 = FFT_len >> 1;
	ia = 0;
	for (i = 0; i < n2; i++)
	{
		cosVal = Tw[ia * 2];
		sinVal = Tw[(ia * 2) + 1];
		ia += TwMove;
		l = i + n2;
		a0 = pSrc[2 * i] + pSrc[2 * l];
		xt = pSrc[2 * i] - pSrc[2 * l];
		yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];
		a1 = pSrc[2 * l + 1] + pSrc[2 * i + 1];
		p0 = _ParaMpy(xt, cosVal);
		p1 = _ParaMpy(yt, sinVal);
		p2 = _ParaMpy(yt, cosVal);
		p3 = _ParaMpy(xt, sinVal);
		pSrc[2 * i] = a0;
		pSrc[2 * i + 1] = a1;
		pSrc[2 * l] = p0 - p1;
		pSrc[2 * l + 1] = p2 + p3;
	}

	TwMove <<= 1u;
	for (k = FFT_len / 2; k > 2; k = k >> 1)
	{
		n1 = n2;
		n2 = n2 >> 1;
		ia = 0;
		j = 0;
		do
		{
			cosVal = Tw[ia * 2];
			sinVal = Tw[(ia * 2) + 1];
			ia += TwMove;
			i = j;
			do
			{
				l = i + n2;
				a0 = pSrc[2 * i] + pSrc[2 * l];
				xt = pSrc[2 * i] - pSrc[2 * l];
				yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];
				a1 = pSrc[2 * l + 1] + pSrc[2 * i + 1];
				p0 = _ParaMpy(xt, cosVal);
				p1 = _ParaMpy(yt, sinVal);
				p2 = _ParaMpy(yt, cosVal);
				p3 = _ParaMpy(xt, sinVal);
				pSrc[2 * i] = a0;
				pSrc[2 * i + 1] = a1;
				pSrc[2 * l] = p0 - p1;
				pSrc[2 * l + 1] = p2 + p3;
				i += n1;
			} while (i < FFT_len);
			j++;
		} while (j < n2);

		TwMove <<= 1u;
	}

	for (i = 0; i < FFT_len; i += 2)
	{
		a0 = pSrc[2 * i] + pSrc[2 * i + 2];
		xt = pSrc[2 * i] - pSrc[2 * i + 2];
		a1 = pSrc[2 * i + 3] + pSrc[2 * i + 1];
		yt = pSrc[2 * i + 1] - pSrc[2 * i + 3];
		p0 = _ParaMpy(a0, oneDivLen);
		p2 = _ParaMpy(xt, oneDivLen);
		p1 = _ParaMpy(a1, oneDivLen);
		p3 = _ParaMpy(yt, oneDivLen);
		pSrc[2 * i] = p0;
		pSrc[2 * i + 1] = p1;
		pSrc[2 * i + 2] = p2;
		pSrc[2 * i + 3] = p3;
	}

}




#if OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_NOUSED
OSlwToolSignalFFT_Type pOSlwToolSignalFFT = NULL;
#else
OSlwToolSignalFFT_Type pOSlwToolSignalFFT = OSlwToolSignalFFT;
#endif

void * OSlwToolSignalFFT(ParaType *x, lw_u32 len, lw_u8 BitRev_flag, lw_u8 iFFT_flag, void *temp)
{
	lw_u32 TwMove, BitMove, RunR4FFT = 0, errflag = 0;
	lw_u16 *BitRevTable;
	ParaType *Tw, oneDivLen;

	Tw = (ParaType *)OSlwToolSignalFFT_Coef;
	oneDivLen = _ParaDiv(_ParaFint(1), _ParaFint(len));

	switch (len) {
#if OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_NOUSED

	default:OSLW_assert(1);errflag = 1;break;

#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_16

	case (lw_u32)16:TwMove = (lw_u32)1;BitMove = (lw_u32)1;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[0]);break;
	default:OSLW_assert(1);errflag = 1;break;

#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_32

	case (lw_u32)32:TwMove = (lw_u32)1;BitMove = (lw_u32)1;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[0]);break;
	case (lw_u32)16:TwMove = (lw_u32)2;BitMove = (lw_u32)2;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[1]);break;
	default:OSLW_assert(1);errflag = 1;break;

#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_64

	case (lw_u32)64:TwMove = (lw_u32)1;BitMove = (lw_u32)1;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[0]);break;
	case (lw_u32)32:TwMove = (lw_u32)2;BitMove = (lw_u32)2;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[1]);break;
	case (lw_u32)16:TwMove = (lw_u32)4;BitMove = (lw_u32)4;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[3]);break;
	default:OSLW_assert(1);errflag = 1;break;

#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_128

	case (lw_u32)128:TwMove = (lw_u32)1;BitMove = (lw_u32)1;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[0]);break;
	case (lw_u32)64:TwMove = (lw_u32)2;BitMove = (lw_u32)2;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[1]);break;
	case (lw_u32)32:TwMove = (lw_u32)4;BitMove = (lw_u32)4;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[3]);break;
	case (lw_u32)16:TwMove = (lw_u32)8;BitMove = (lw_u32)8;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[7]);break;
	default:OSLW_assert(1);errflag = 1;break;

#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_256

	case (lw_u32)256:TwMove = (lw_u32)1;BitMove = (lw_u32)1;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[0]);break;
	case (lw_u32)128:TwMove = (lw_u32)2;BitMove = (lw_u32)2;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[1]);break;
	case (lw_u32)64:TwMove = (lw_u32)4;BitMove = (lw_u32)4;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[3]);break;
	case (lw_u32)32:TwMove = (lw_u32)8;BitMove = (lw_u32)8;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[7]);break;
	case (lw_u32)16:TwMove = (lw_u32)16;BitMove = (lw_u32)16;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[15]);break;
	default:OSLW_assert(1);errflag = 1;break;

#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_512

	case (lw_u32)512:TwMove = (lw_u32)1;BitMove = (lw_u32)1;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[0]);break;
	case (lw_u32)256:TwMove = (lw_u32)2;BitMove = (lw_u32)2;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[1]);break;
	case (lw_u32)128:TwMove = (lw_u32)4;BitMove = (lw_u32)4;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[3]);break;
	case (lw_u32)64:TwMove = (lw_u32)8;BitMove = (lw_u32)8;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[7]);break;
	case (lw_u32)32:TwMove = (lw_u32)16;BitMove = (lw_u32)16;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[15]);break;
	case (lw_u32)16:TwMove = (lw_u32)32;BitMove = (lw_u32)32;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[31]);break;
	default:OSLW_assert(1);errflag = 1;break;

#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_1024

	case (lw_u32)1024:TwMove = (lw_u32)1;BitMove = (lw_u32)1;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[0]);break;
	case (lw_u32)512:TwMove = (lw_u32)2;BitMove = (lw_u32)2;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[1]);break;
	case (lw_u32)256:TwMove = (lw_u32)4;BitMove = (lw_u32)4;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[3]);break;
	case (lw_u32)128:TwMove = (lw_u32)8;BitMove = (lw_u32)8;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[7]);break;
	case (lw_u32)64:TwMove = (lw_u32)16;BitMove = (lw_u32)16;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[15]);break;
	case (lw_u32)32:TwMove = (lw_u32)32;BitMove = (lw_u32)32;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[31]);break;
	case (lw_u32)16:TwMove = (lw_u32)64;BitMove = (lw_u32)64;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[63]);break;
	default:OSLW_assert(1);errflag = 1;break;

#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_2048

	case (lw_u32)2048:TwMove = (lw_u32)1;BitMove = (lw_u32)1;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[0]);break;
	case (lw_u32)1024:TwMove = (lw_u32)2;BitMove = (lw_u32)2;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[1]);break;
	case (lw_u32)512:TwMove = (lw_u32)4;BitMove = (lw_u32)4;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[3]);break;
	case (lw_u32)256:TwMove = (lw_u32)8;BitMove = (lw_u32)8;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[7]);break;
	case (lw_u32)128:TwMove = (lw_u32)16;BitMove = (lw_u32)16;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[15]);break;
	case (lw_u32)64:TwMove = (lw_u32)32;BitMove = (lw_u32)32;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[31]);break;
	case (lw_u32)32:TwMove = (lw_u32)64;BitMove = (lw_u32)64;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[63]);break;
	case (lw_u32)16:TwMove = (lw_u32)128;BitMove = (lw_u32)128;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[127]);break;
	default:OSLW_assert(1);errflag = 1;break;

#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_4096

	case (lw_u32)4096:TwMove = (lw_u32)1;BitMove = (lw_u32)1;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[0]);break;
	case (lw_u32)2048:TwMove = (lw_u32)2;BitMove = (lw_u32)2;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[1]);break;
	case (lw_u32)1024:TwMove = (lw_u32)4;BitMove = (lw_u32)4;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[3]);break;
	case (lw_u32)512:TwMove = (lw_u32)8;BitMove = (lw_u32)8;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[7]);break;
	case (lw_u32)256:TwMove = (lw_u32)16;BitMove = (lw_u32)16;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[15]);break;
	case (lw_u32)128:TwMove = (lw_u32)32;BitMove = (lw_u32)32;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[31]);break;
	case (lw_u32)64:TwMove = (lw_u32)64;BitMove = (lw_u32)64;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[63]);break;
	case (lw_u32)32:TwMove = (lw_u32)128;BitMove = (lw_u32)128;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[127]);break;
	case (lw_u32)16:TwMove = (lw_u32)256;BitMove = (lw_u32)256;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[255]);break;
	default:OSLW_assert(1);errflag = 1;break;

#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_8192

	case (lw_u32)8192:TwMove = (lw_u32)1;BitMove = (lw_u32)1;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[0]);break;
	case (lw_u32)4096:TwMove = (lw_u32)2;BitMove = (lw_u32)2;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[1]);break;
	case (lw_u32)2048:TwMove = (lw_u32)4;BitMove = (lw_u32)4;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[3]);break;
	case (lw_u32)1024:TwMove = (lw_u32)8;BitMove = (lw_u32)8;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[7]);break;
	case (lw_u32)512:TwMove = (lw_u32)16;BitMove = (lw_u32)16;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[15]);break;
	case (lw_u32)256:TwMove = (lw_u32)32;BitMove = (lw_u32)32;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[31]);break;
	case (lw_u32)128:TwMove = (lw_u32)64;BitMove = (lw_u32)64;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[63]);break;
	case (lw_u32)64:TwMove = (lw_u32)128;BitMove = (lw_u32)128;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[127]);break;
	case (lw_u32)32:TwMove = (lw_u32)256;BitMove = (lw_u32)256;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[255]);break;
	case (lw_u32)16:TwMove = (lw_u32)512;BitMove = (lw_u32)512;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[511]);break;
	default:OSLW_assert(1);errflag = 1;break;

#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_16384

	case (lw_u32)16384:TwMove = (lw_u32)1;BitMove = (lw_u32)1;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[0]);break;
	case (lw_u32)8192:TwMove = (lw_u32)2;BitMove = (lw_u32)2;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[1]);break;
	case (lw_u32)4096:TwMove = (lw_u32)4;BitMove = (lw_u32)4;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[3]);break;
	case (lw_u32)2048:TwMove = (lw_u32)8;BitMove = (lw_u32)8;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[7]);break;
	case (lw_u32)1024:TwMove = (lw_u32)16;BitMove = (lw_u32)16;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[15]);break;
	case (lw_u32)512:TwMove = (lw_u32)32;BitMove = (lw_u32)32;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[31]);break;
	case (lw_u32)256:TwMove = (lw_u32)64;BitMove = (lw_u32)64;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[63]);break;
	case (lw_u32)128:TwMove = (lw_u32)128;BitMove = (lw_u32)128;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[127]);break;
	case (lw_u32)64:TwMove = (lw_u32)256;BitMove = (lw_u32)256;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[255]);break;
	case (lw_u32)32:TwMove = (lw_u32)512;BitMove = (lw_u32)512;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[511]);break;
	case (lw_u32)16:TwMove = (lw_u32)1024;BitMove = (lw_u32)1024;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[1023]);break;
	default:OSLW_assert(1);errflag = 1;break;

#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_32768

	case (lw_u32)32768:TwMove = (lw_u32)1;BitMove = (lw_u32)1;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[0]);break;
	case (lw_u32)16384:TwMove = (lw_u32)2;BitMove = (lw_u32)2;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[1]);break;
	case (lw_u32)8192:TwMove = (lw_u32)4;BitMove = (lw_u32)4;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[3]);break;
	case (lw_u32)4096:TwMove = (lw_u32)8;BitMove = (lw_u32)8;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[7]);break;
	case (lw_u32)2048:TwMove = (lw_u32)16;BitMove = (lw_u32)16;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[15]);break;
	case (lw_u32)1024:TwMove = (lw_u32)32;BitMove = (lw_u32)32;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[31]);break;
	case (lw_u32)512:TwMove = (lw_u32)64;BitMove = (lw_u32)64;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[63]);break;
	case (lw_u32)256:TwMove = (lw_u32)128;BitMove = (lw_u32)128;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[127]);break;
	case (lw_u32)128:TwMove = (lw_u32)256;BitMove = (lw_u32)256;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[255]);break;
	case (lw_u32)64:TwMove = (lw_u32)512;BitMove = (lw_u32)512;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[511]);break;
	case (lw_u32)32:TwMove = (lw_u32)1024;BitMove = (lw_u32)1024;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[1023]);break;
	case (lw_u32)16:TwMove = (lw_u32)2048;BitMove = (lw_u32)2048;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[2047]);break;
	default:OSLW_assert(1);errflag = 1;break;

#elif OSLW_TOOL_FFT_MAX_LEN==OSLW_TOOL_FFT_MAXLEN_65536

	case (lw_u32)65536:TwMove = (lw_u32)1;BitMove = (lw_u32)1;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[0]);break;
	case (lw_u32)32768:TwMove = (lw_u32)2;BitMove = (lw_u32)2;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[1]);break;
	case (lw_u32)16384:TwMove = (lw_u32)4;BitMove = (lw_u32)4;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[3]);break;
	case (lw_u32)8192:TwMove = (lw_u32)8;BitMove = (lw_u32)8;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[7]);break;
	case (lw_u32)4096:TwMove = (lw_u32)16;BitMove = (lw_u32)16;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[15]);break;
	case (lw_u32)2048:TwMove = (lw_u32)32;BitMove = (lw_u32)32;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[31]);break;
	case (lw_u32)1024:TwMove = (lw_u32)64;BitMove = (lw_u32)64;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[63]);break;
	case (lw_u32)512:TwMove = (lw_u32)128;BitMove = (lw_u32)128;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[127]);break;
	case (lw_u32)256:TwMove = (lw_u32)256;BitMove = (lw_u32)256;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[255]);break;
	case (lw_u32)128:TwMove = (lw_u32)512;BitMove = (lw_u32)512;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[511]);break;
	case (lw_u32)64:TwMove = (lw_u32)1024;BitMove = (lw_u32)1024;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[1023]);break;
	case (lw_u32)32:TwMove = (lw_u32)2048;BitMove = (lw_u32)2048;RunR4FFT = 0;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[2047]);break;
	case (lw_u32)16:TwMove = (lw_u32)4096;BitMove = (lw_u32)4096;RunR4FFT = 1;BitRevTable = (lw_u16 *)&(OSlwToolSignalFFT_BitRevTab[4095]);break;
	default:OSLW_assert(1);errflag = 1;break;

#endif
	}

	if (errflag == 1) {
		return NULL;
	}

	if (iFFT_flag == 0)
	{
		if (RunR4FFT) {
			_OSlwToolSignal_BF_R4(x, len, Tw, TwMove);
		}
		else {
			_OSlwToolSignal_BF_R2(x, len, Tw, TwMove);
		}
	}
	else
	{
		if (RunR4FFT) {
			_OSlwToolSignal_IBF_R4(x, len, Tw, TwMove, oneDivLen);
		}
		else {
			_OSlwToolSignal_IBF_R2(x, len, Tw, TwMove, oneDivLen);
		}
	}

	if (BitRev_flag == 1)
	{
		_OSlwToolSignal_FFT_BitRev(x, len, BitMove, BitRevTable);
	}


	return x;

}

void * OSlwToolSignalFFTCompelex(ParaType *x_complex, ParaType *x, lw_u32 FFT_len)
{
	lw_32 i;

	OSLW_assert(!(x_complex));
	OSLW_assert(!(x));

	for (i = FFT_len - 1; i >= 0; i--)
	{
		x_complex[i << 1] = x[i];
		x_complex[(i << 1) + 1] = _ParaFrom(0);
	}
	return x_complex;
}

void * OSlwToolSignalFFTReal(ParaType *x, ParaType *x_complex, lw_u32 FFT_len)
{
	lw_u32 i;

	OSLW_assert(!(x_complex));
	OSLW_assert(!(x));

	for (i = 0; i < FFT_len; i++)
	{
		x[i] = x_complex[i << 1];
	}
	return x;
}


void * OSlwToolSignalFFTShift(ParaType *x1, ParaType *x2, lw_u32 FFT_len)
{
	lw_u32 divm, modm;
	ParaType *px1, *px2;
	OSLW_assert(!(x1));
	OSLW_assert(!(x2));
	OSLW_assert(!(FFT_len));

	divm = 1 * FFT_len / CACHE_READ_NUM, modm = 1 * FFT_len%CACHE_READ_NUM;

	if (x1 == x2) {
		px1 = x1;
		px2 = x1 + FFT_len;
		LwVectSwap(px1, px2, divm, modm);
	}
	else {
		px1 = x1;
		px2 = x2 + FFT_len;
		LwVectCopy(px1, px2, divm, modm);

		px1 = x1 + FFT_len;
		px2 = x2;
		LwVectCopy(px1, px2, divm, modm);
	}

	return x1;
}
void *OSlwToolSignalDwtKern(ParaType *cA, ParaType *cD, ParaType *xin, ParaType *LoD, ParaType *HiD, lw_u32 xin_Len, lw_u32 kern_Len, ParaType *temp_work)
{
	lw_32 i;
	lw_u32 period;
	lw_32 Expa_len;
	LwMatLenType div_xin, mod_xin, div_filter, mod_filter;
	ParaType *p1, *p2, *pHiD, *pLoD;
	ParaType dot1, dot2;

	//计算对称延拓信号的真实周期
	period = ((lw_u32)xin_Len << 1) - 2;
	Expa_len = kern_Len - 1;//与matlab保持一致
	//total_len = xin_Len + (Expa_len << 1);


	div_xin = xin_Len / CACHE_READ_NUM, mod_xin = xin_Len%CACHE_READ_NUM;
	div_filter = kern_Len / CACHE_READ_NUM, mod_filter = kern_Len%CACHE_READ_NUM;

	//补齐xin的数据 采用对称法
	p1 = temp_work;
	for (i = -Expa_len; i < 0; i++) {
		*p1++ = xin[_CentDataIndex(i, xin_Len, period)];
	}

	p2 = xin;
	LwVectCopy(p1, p2, div_xin, mod_xin);

	for (i = xin_Len; i < (lw_32)(Expa_len + xin_Len + 1); i++) {
		*p1++ = xin[_CentDataIndex(i, xin_Len, period)];
	}

	//卷积 直接以2个间隔
	for (i = 1; i < (lw_32)(xin_Len + Expa_len); i += 2)
	{
		p1 = temp_work + i;
		p2 = temp_work + i;
		pLoD = LoD;
		pHiD = HiD;

		LwVectDot(dot1, p1, pLoD, div_filter, mod_filter);
		LwVectDot(dot2, p2, pHiD, div_filter, mod_filter);

		*cA++ = dot1;
		*cD++ = dot2;
	}

	return cA;
}

void *OSlwToolSignalIDwtKern(ParaType *xout, ParaType *cA, ParaType *cD, ParaType *LoR, ParaType *HiR, lw_u32 c_Len, lw_u32 kern_Len, ParaType *temp_work)
{
	lw_32 i;
	lw_u32 period;
	lw_u32 Expa_len, total_len;
	LwMatLenType div_filter, mod_filter;
	ParaType *p1, *p2;
	ParaType dot1;

	//计算对称延拓信号的真实周期
	period = ((lw_u32)c_Len << 1) - 2;
	Expa_len = kern_Len >> 1;
	total_len = (c_Len << 1) - (kern_Len - 2);
	div_filter = kern_Len / CACHE_READ_NUM, mod_filter = kern_Len%CACHE_READ_NUM;


	//还原cA,先进行延拓与上采样
	p1 = temp_work;
	//根据实验结果不需要往前延拓
	//for (i = -Expa_len; i < 0; i++)
	//{
	//	*p1++ = _ParaFrom(0);
	//	*p1++ = cA[_CentDataIndex(i, c_Len, period)];
	//}

	p2 = cA;
	for (i = 0; i < (lw_32)c_Len; i++)
	{
		*p1++ = _ParaFrom(0);
		*p1++ = *p2++;
	}

	for (i = c_Len; i < (lw_32)(Expa_len + c_Len); i++)
	{
		*p1++ = _ParaFrom(0);
		*p1++ = cA[_CentDataIndex(i, c_Len, period)];
	}

	for (i = 0; i < (lw_32)total_len; i++)
	{
		p1 = temp_work + i;
		p2 = LoR;
		LwVectDot(dot1, p1, p2, div_filter, mod_filter);
		xout[i] = dot1;
	}

	//如果cD为NULL 不对细节进行处理
	if (cD == NULL)
		return xout;

	//还原cD,先进行延拓与上采样
	p1 = temp_work;
	//根据实验结果不需要往前延拓
	//for (i = -Expa_len; i < 0; i++)
	//{
	//	*p1++ = _ParaFrom(0);
	//	*p1++ = cD[_CentDataIndex(i, c_Len, period)];
	//}

	p2 = cD;
	for (i = 0; i < (lw_32)c_Len; i++)
	{
		*p1++ = _ParaFrom(0);
		*p1++ = *p2++;
	}

	for (i = c_Len; i < (lw_32)(Expa_len + c_Len); i++)
	{
		*p1++ = _ParaFrom(0);
		*p1++ = cD[_CentDataIndex(i, c_Len, period)];
	}

	for (i = 0; i < (lw_32)total_len; i++)
	{
		p1 = temp_work + i;
		p2 = HiR;
		LwVectDot(dot1, p1, p2, div_filter, mod_filter);
		xout[i] = _ParaAdd(xout[i], dot1);
	}

	return xout;
}

//得到极值
void _OSlwToolSignalEmdGetPeak(
	ParaType *signal,
	lw_32 signal_len,

	lw_32 *sig_top,
	lw_32 *sig_top_len,
	lw_32 *sig_bot,
	lw_32 *sig_bot_len
)
{
	lw_32 i;
	lw_32 top_len = 0, bot_len = 0;
	ParaType d1, d2, d3;

	OSLW_assert(!(signal));
	OSLW_assert(!(sig_top));
	OSLW_assert(!(sig_top_len));
	OSLW_assert(!(sig_bot));
	OSLW_assert(!(sig_bot_len));

	for (i = 0; i < signal_len - 2; i++)
	{
		d1 = signal[i];
		d2 = signal[i + 1];
		d3 = signal[i + 2];

		//这个是上极值
		if (d2 > d1 && d2 > d3) {
			*sig_top++ = (i + 1);
			top_len++;
		}

		//这个是下极值
		if (d2 < d1 && d2 < d3) {
			*sig_bot++ = (i + 1);
			bot_len++;
		}

	}
	sig_top_len[0] = top_len;
	sig_bot_len[0] = bot_len;
}

//得到样条插值的补充值
void _OSlwToolSignalEmdGetPoint(
	ParaType *signal,
	lw_32 signal_len,

	lw_32 sig_left_top_i,
	lw_32 sig_left_bot_i,
	lw_32 sig_right_top_i,
	lw_32 sig_right_bot_i,

	ParaType *sig_left_top_x,
	ParaType *sig_left_top_y,

	ParaType *sig_left_bot_x,
	ParaType *sig_left_bot_y,

	ParaType *sig_right_top_x,
	ParaType *sig_right_top_y,

	ParaType *sig_right_bot_x,
	ParaType *sig_right_bot_y,

	lw_32 add_len
)
{
	ParaType top_l, bot_l, top_r, bot_r, end_l, end_r;
	ParaType A, P, M;
	ParaType t1, t2, t3;
	lw_32 d1, d2, loc, td1, td2;
	lw_32 i;
	lw_32 p_int;


	top_l = signal[sig_left_top_i];
	bot_l = signal[sig_left_bot_i];
	top_r = signal[sig_right_top_i];
	bot_r = signal[sig_right_bot_i];
	end_l = signal[0];
	end_r = signal[signal_len - 1];

	OSLW_assert(sig_left_top_i == sig_left_bot_i);
	OSLW_assert(sig_right_top_i == sig_right_bot_i);

	//计算左边端点值
	A = _ParaMpy(_ParaFrom(0.5), _ParaAbs(top_l - bot_l));
	P = _ParaMpy(_ParaFrom(2), _ParaAbs(_ParaFrom(sig_left_top_i) - _ParaFrom(sig_left_bot_i)));
	p_int = _ParaInt(P);
	t1 = _ParaMpy(_ParaFrom(2 * PI), _ParaFrom(1 - sig_left_top_i - 1));
	t1 = _ParaDiv(t1, P);
	M = end_l - _ParaMpy(_ParaCos(t1), A);
	//计算补充数值
	t2 = M + A;
	t3 = M - A;
	//就是那跳跃间隔 对称映射
	d1 = ((1 - sig_left_top_i - 1) / p_int);
	d1 = (d1 < 0) ? (d1 + 1) : (d1 + 0);
	d1 *= p_int;
	td1 = -p_int;

	d2 = ((1 - sig_left_bot_i - 1) / p_int);
	d2 = (d2 < 0) ? (d2 + 1) : (d2 + 0);
	d2 *= p_int;
	td2 = -p_int;

	i = 0;loc = sig_left_top_i + d1;
	while (i < add_len) {
		loc += td1;
		if (loc != 0) {
			sig_left_top_x[add_len - i - 1] = _ParaFrom(loc);
			sig_left_top_y[add_len - i - 1] = t2;
			i++;
		}
	}

	i = 0;loc = sig_left_bot_i + d2;
	while (i < add_len) {
		loc += td2;
		if (loc != 0) {
			sig_left_bot_x[add_len - i - 1] = _ParaFrom(loc);
			sig_left_bot_y[add_len - i - 1] = t3;
			i++;
		}
	}

	//计算右边端点值
	A = _ParaMpy(_ParaFrom(0.5), _ParaAbs(top_r - bot_r));
	P = _ParaMpy(_ParaFrom(2), _ParaAbs(_ParaFrom(sig_right_top_i) - _ParaFrom(sig_right_bot_i)));
	p_int = _ParaInt(P);
	t1 = _ParaMpy(_ParaFrom(2 * PI), _ParaFrom(signal_len - sig_right_top_i - 1));
	t1 = _ParaDiv(t1, P);
	M = end_r - _ParaMpy(_ParaCos(t1), A);
	t2 = M + A;
	t3 = M - A;
	d1 = ((signal_len - sig_right_top_i - 1) / p_int);
	d1 = (d1 < 0) ? (d1 + 1) : (d1 + 0);
	d1 *= p_int;
	td1 = p_int;

	d2 = ((signal_len - sig_right_bot_i - 1) / p_int);
	d2 = (d2 < 0) ? (d2 + 1) : (d2 + 0);
	d2 *= p_int;
	td2 = p_int;

	i = 0;loc = sig_right_top_i + d1;
	while (i < add_len) {
		loc += td1;
		if (loc != signal_len - 1) {
			sig_right_top_x[i] = _ParaFrom(loc);
			sig_right_top_y[i] = t2;
			i++;
		}
	}

	i = 0;loc = sig_right_bot_i + d2;
	while (i < add_len) {
		loc += td2;
		if (loc != signal_len - 1) {
			sig_right_bot_x[i] = _ParaFrom(loc);
			sig_right_bot_y[i] = t3;
			i++;
		}
	}
}


lw_32 OSlwToolSignalEmdSift(
	ParaType *res_sig,
	ParaType *mean_val,
	lw_32 signal_len,
	lw_32 peak_len[2],
	ParaType sig_rms[2],
	lw_mem pmem
)
{
	//补充的信号长度
#define N 3

	//极值点的ID
	lw_32 *top_arr;
	lw_32 *bot_arr;

	//极值点数量
	lw_32 toplen, botlen;

	//补充信号
	ParaType left_top_ex_x[N], left_top_ex_y[N];
	ParaType left_bot_ex_x[N], left_bot_ex_y[N];
	ParaType right_top_ex_x[N], right_top_ex_y[N];
	ParaType right_bot_ex_x[N], right_bot_ex_y[N];

	lw_32 itemp1, i;

	lw_u32 divm, modm;

	//样条插值与迭代器
	OSlwToolSignalSplineSTU sp_top, sp_bot;
	LwInterpIter iter_top = LwInterpIterDefault(&sp_top, N);
	LwInterpIter iter_bot = LwInterpIterDefault(&sp_bot, N);

	ParaType *x_top, *y_top, *sp_coff_top;
	ParaType *x_bot, *y_bot, *sp_coff_bot;
	ParaType *px, *py;
	ParaType temp_top, temp_bot, temp_x;

	ParaType mean_rms, rsig_rms;

	OSLW_assert(!(mean_val));
	OSLW_assert(!(res_sig));
	OSLW_assert(!(pmem));
	OSLW_assert(signal_len <= 0);

	top_arr = pmem->Malloc(pmem, sizeof(lw_u32)*signal_len / 2);
	bot_arr = pmem->Malloc(pmem, sizeof(lw_u32)*signal_len / 2);

	divm = signal_len / CACHE_READ_NUM, modm = signal_len%CACHE_READ_NUM;

	//查找极值
	_OSlwToolSignalEmdGetPeak(res_sig, signal_len, top_arr, &toplen, bot_arr, &botlen);

	itemp1 = toplen - botlen;
	itemp1 = (itemp1 < 0) ? -itemp1 : itemp1;

	if (toplen == 0 || botlen == 0) {
		return 1;
	}
	if (itemp1 >= 2) {
		return 2;
	}

	//得到补充点
	_OSlwToolSignalEmdGetPoint(
		res_sig, signal_len,
		top_arr[0], bot_arr[0],
		top_arr[toplen - 1], bot_arr[botlen - 1],
		left_top_ex_x, left_top_ex_y,
		left_bot_ex_x, left_bot_ex_y,
		right_top_ex_x, right_top_ex_y,
		right_bot_ex_x, right_bot_ex_y,
		N
	);

	//分配样条插值内存
	x_top = pmem->Malloc(pmem, PARA_MEM_CAL(toplen + N * 2 + 1));
	y_top = pmem->Malloc(pmem, PARA_MEM_CAL(toplen + N * 2 + 1));
	sp_coff_top = pmem->Malloc(pmem, PARA_MEM_CAL(toplen + N * 2 + 2) * 4);

	x_bot = pmem->Malloc(pmem, PARA_MEM_CAL(botlen + N * 2 + 1));
	y_bot = pmem->Malloc(pmem, PARA_MEM_CAL(botlen + N * 2 + 1));
	sp_coff_bot = pmem->Malloc(pmem, PARA_MEM_CAL(botlen + N * 2 + 2) * 4);

	//信号赋值
	px = x_top, py = y_top;
	for (i = 0; i < N; i++) {
		*px++ = left_top_ex_x[i];
		*py++ = left_top_ex_y[i];
	}
	for (i = 0; i < toplen; i++) {
		*px++ = _ParaFrom(top_arr[i]);
		*py++ = res_sig[top_arr[i]];
	}
	for (i = 0; i < N; i++) {
		*px++ = right_top_ex_x[i];
		*py++ = right_top_ex_y[i];
	}

	px = x_bot, py = y_bot;
	for (i = 0; i < N; i++) {
		*px++ = left_bot_ex_x[i];
		*py++ = left_bot_ex_y[i];
	}
	for (i = 0; i < botlen; i++) {
		*px++ = _ParaFrom(bot_arr[i]);
		*py++ = res_sig[bot_arr[i]];
	}
	for (i = 0; i < N; i++) {
		*px++ = right_bot_ex_x[i];
		*py++ = right_bot_ex_y[i];
	}

	//计算样条插值
	OSlwToolSignalSplineNew(
		&sp_top, x_top, y_top,
		toplen + N * 2,
		NULL, sp_coff_top, pmem
	);

	OSlwToolSignalSplineNew(
		&sp_bot, x_bot, y_bot,
		botlen + N * 2,
		NULL, sp_coff_bot, pmem
	);


	//计算中间值
	for (i = 0; i < signal_len; i++) {
		temp_x = _ParaFint(i);
		temp_top = LwInterpIterGet(&iter_top, temp_x);
		temp_bot = LwInterpIterGet(&iter_bot, temp_x);
		mean_val[i] = _ParaDiv(temp_bot + temp_top, _ParaFrom(2));
	}

	//得到2范数
	px = mean_val;
	py = res_sig;
	LwVectNorm2(mean_rms, px, divm, modm);
	LwVectNorm2(rsig_rms, py, divm, modm);

	//减去均值
	px = mean_val;
	py = res_sig;
	LwVectAddKX(py, _ParaFrom(-1), px, divm, modm);

	if (peak_len) {
		peak_len[0] = toplen;
		peak_len[1] = botlen;
	}

	if (sig_rms) {
		sig_rms[0] = mean_rms;
		sig_rms[1] = rsig_rms;
	}

	//别忘了free
	pmem->Free(pmem, top_arr);
	pmem->Free(pmem, bot_arr);

	pmem->Free(pmem, x_top);
	pmem->Free(pmem, y_top);
	pmem->Free(pmem, sp_coff_top);

	pmem->Free(pmem, x_bot);
	pmem->Free(pmem, y_bot);
	pmem->Free(pmem, sp_coff_bot);

	return 0;

#undef N
}


ParaType OSlwToolSignalEmdKern(
	ParaType *imf,
	ParaType *res_sig,
	lw_32 signal_len,
	lw_u32 Max_iter,
	ParaType rtol,
	lw_mem pmem
)
{
	lw_u32 i;
	lw_32 peak_len[2];
	lw_32 re;
	ParaType rtol_now;
	ParaType sig_rms[2];
	ParaType *meam_temp;
	lw_u32 divm, modm;

	OSLW_assert(!(imf));
	OSLW_assert(!(res_sig));
	OSLW_assert(!(pmem));
	OSLW_assert(signal_len <= 0);

	divm = signal_len / CACHE_READ_NUM, modm = signal_len%CACHE_READ_NUM;

	meam_temp = pmem->Malloc(pmem, PARA_MEM_CAL(signal_len));

	memcpy(imf, res_sig, PARA_MEM_CAL(signal_len));

	for (i = 0; i < Max_iter; i++)
	{
		re = OSlwToolSignalEmdSift(imf, meam_temp, signal_len, peak_len, sig_rms, pmem);

		if (re) {
			break;
		}
		rtol_now = _ParaDiv(sig_rms[0], sig_rms[1] + 1e-8);
		rtol_now = _ParaMpy(rtol_now, rtol_now);

		if (rtol_now < rtol) {
			break;
		}
	}

	pmem->Free(pmem, meam_temp);


	if (re) {
		return _ParaFrom(-re);
	}

	LwVectAddKX(res_sig, _ParaFrom(-1), imf, divm, modm);

	return rtol_now;

}


void OSlwToolSignalEmdSimple(
	LwMat *res,
	ParaType *signal,
	lw_u32 Imf_Max_iter,
	ParaType imf_rtol,
	lw_mem pmem
)
{
	lw_u32 i;
	lw_32 signal_len;
	lw_32 iter_num;
	ParaType rtol_now;

	OSLW_assert(!(res));
	OSLW_assert(!(signal));
	OSLW_assert(!(pmem));

	iter_num = res->row;
	signal_len = res->col;



	for (i = 0; i < (lw_u32)iter_num; i++) {
		rtol_now = OSlwToolSignalEmdKern(
			OSlwMatGetR(*res, i), signal, signal_len,
			Imf_Max_iter, imf_rtol,
			pmem
		);

		//没有极点查询了
		if (rtol_now < 0) {
			break;
		}
	}

	//循环次数不足 修改结果矩阵 并且保存残差
	if (iter_num != i) {
		LwMatInit(res, i, signal_len, res->a);
	}

}


//uk=uk+uh1-uh2
void _OSlwToolSignalVmdCalUk(
	ParaType *uk,//复数 真实长度 2*Len
	ParaType *uh1,//复数 真实长度 2*Len
	ParaType *uh2,//复数 真实长度 2*Len
	lw_u32 Len//数据的长度 
)
{
	lw_u32 divm, modm, i;
	//ParaType *p1, *p2, *p3;
	divm = Len / 4, modm = Len % 4;

	i = divm;
	while (i--) {
		uk[0] = _ParaSub(_ParaAdd(uk[0], uh1[0]), uh2[0]);
		uk[1] = _ParaSub(_ParaAdd(uk[1], uh1[1]), uh2[1]);
		uk[2] = _ParaSub(_ParaAdd(uk[2], uh1[2]), uh2[2]);
		uk[3] = _ParaSub(_ParaAdd(uk[3], uh1[3]), uh2[3]);

		uk[4] = _ParaSub(_ParaAdd(uk[4], uh1[4]), uh2[4]);
		uk[5] = _ParaSub(_ParaAdd(uk[5], uh1[5]), uh2[5]);
		uk[6] = _ParaSub(_ParaAdd(uk[6], uh1[6]), uh2[6]);
		uk[7] = _ParaSub(_ParaAdd(uk[7], uh1[7]), uh2[7]);

		uk += 8;uh1 += 8;uh2 += 8;
	}
	i = modm;
	while (i--) {
		uk[0] = _ParaSub(_ParaAdd(uk[0], uh1[0]), uh2[0]);
		uk[1] = _ParaSub(_ParaAdd(uk[1], uh1[1]), uh2[1]);

		uk += 2;uh1 += 2;uh2 += 2;
	}

}
//返回值为前后差异的均方根
ParaType _OSlwToolSignalVmdCalUh(
	ParaType *uh,//复数 真实长度 2*Len
	ParaType *freq_cpx,//复数 真实长度 2*Len
	ParaType *uk,//复数 真实长度 2*Len
	ParaType *lamda,//复数 真实长度 2*Len
	ParaType *freq,//实数 真实长度 Len
	ParaType alpha,
	ParaType omega,
	lw_u32 Len
)
{
	lw_u32 divm, modm, i;
	ParaType t1, t2, f1;
	ParaType sum1 = 0;
	ParaType temp1, temp2;
	divm = Len / 4, modm = Len % 4;
#define CAL_T1_T2(N) 		t1 = _ParaSub(freq_cpx[N], uk[N]);\
	t1 = _ParaSub(t1, _ParaDiv(lamda[N], _ParaFrom(2)));\
	t2 = _ParaSub(f1, omega);\
	t2 = _ParaMpy(t2, t2);\
	t2 = _ParaAdd(_ParaFrom(1), _ParaMpy(alpha, t2));

	i = divm;
	while (i--) {
		f1 = freq[0];
		CAL_T1_T2(0);
		temp1 = uh[0];
		uh[0] = _ParaDiv(t1, t2);
		temp1 = _ParaSub(temp1, uh[0]);
		temp1 = _ParaMpy(temp1, temp1);
		sum1 = _ParaAdd(sum1, temp1);
		CAL_T1_T2(1);
		temp2 = uh[1];
		uh[1] = _ParaDiv(t1, t2);
		temp2 = _ParaSub(temp2, uh[1]);
		temp2 = _ParaMpy(temp2, temp2);
		sum1 = _ParaAdd(sum1, temp2);

		f1 = freq[1];
		CAL_T1_T2(2);
		temp1 = uh[2];
		uh[2] = _ParaDiv(t1, t2);
		temp1 = _ParaSub(temp1, uh[2]);
		temp1 = _ParaMpy(temp1, temp1);
		sum1 = _ParaAdd(sum1, temp1);
		CAL_T1_T2(3);
		temp2 = uh[3];
		uh[3] = _ParaDiv(t1, t2);
		temp2 = _ParaSub(temp2, uh[3]);
		temp2 = _ParaMpy(temp2, temp2);
		sum1 = _ParaAdd(sum1, temp2);

		f1 = freq[2];
		CAL_T1_T2(4);
		temp1 = uh[4];
		uh[4] = _ParaDiv(t1, t2);
		temp1 = _ParaSub(temp1, uh[4]);
		temp1 = _ParaMpy(temp1, temp1);
		sum1 = _ParaAdd(sum1, temp1);
		CAL_T1_T2(5);
		temp2 = uh[5];
		uh[5] = _ParaDiv(t1, t2);
		temp2 = _ParaSub(temp2, uh[5]);
		temp2 = _ParaMpy(temp2, temp2);
		sum1 = _ParaAdd(sum1, temp2);

		f1 = freq[3];
		CAL_T1_T2(6);
		temp1 = uh[6];
		uh[6] = _ParaDiv(t1, t2);
		temp1 = _ParaSub(temp1, uh[6]);
		temp1 = _ParaMpy(temp1, temp1);
		sum1 = _ParaAdd(sum1, temp1);
		CAL_T1_T2(7);
		temp2 = uh[7];
		uh[7] = _ParaDiv(t1, t2);
		temp2 = _ParaSub(temp2, uh[7]);
		temp2 = _ParaMpy(temp2, temp2);
		sum1 = _ParaAdd(sum1, temp2);

		uh += 8;freq_cpx += 8;lamda += 8;freq += 4;uk += 8;
	}
	i = modm;
	while (i--) {
		f1 = freq[0];
		CAL_T1_T2(0);
		temp1 = uh[0];
		uh[0] = _ParaDiv(t1, t2);
		temp1 = _ParaSub(temp1, uh[0]);
		temp1 = _ParaMpy(temp1, temp1);
		sum1 = _ParaAdd(sum1, temp1);
		CAL_T1_T2(1);
		temp2 = uh[1];
		uh[1] = _ParaDiv(t1, t2);
		temp2 = _ParaSub(temp2, uh[1]);
		temp2 = _ParaMpy(temp2, temp2);
		sum1 = _ParaAdd(sum1, temp2);

		uh += 2;freq_cpx += 2;lamda += 2;freq += 1;uk += 2;
	}

	sum1 = _ParaDiv(sum1, _ParaFint(Len * 2));

	return _ParaSqrt(sum1);
#undef CAL_T1_T2
}


ParaType _OSlwToolSignalVmdCalOmega(
	ParaType *freqs_half,
	ParaType *uh_half,//频域的长度 真实长度 2*Len
	lw_u32 Len//频域长度的一半
)
{
	ParaType sum1 = 0, sum2 = 0;
	ParaType t1, t2, tabs, t3;
	lw_u32 divm, modm, i;


	divm = Len / 4, modm = Len % 4;

#define CAL_SUM1_SUM2(N) 		t1 = uh_half[0];\
	t2 = uh_half[1];\
	tabs = _ParaAdd(_ParaMpy(t1, t1), _ParaMpy(t2, t2));\
	t3 = _ParaMpy(freqs_half[N], tabs);\
	uh_half+=2;


	i = divm;
	while (i--) {
		CAL_SUM1_SUM2(0);
		sum1 = _ParaAdd(sum1, t3);
		sum2 = _ParaAdd(sum2, tabs);

		CAL_SUM1_SUM2(1);
		sum1 = _ParaAdd(sum1, t3);
		sum2 = _ParaAdd(sum2, tabs);

		CAL_SUM1_SUM2(2);
		sum1 = _ParaAdd(sum1, t3);
		sum2 = _ParaAdd(sum2, tabs);

		CAL_SUM1_SUM2(3);
		sum1 = _ParaAdd(sum1, t3);
		sum2 = _ParaAdd(sum2, tabs);

		freqs_half += 4;
	}
	i = modm;
	while (i--) {
		CAL_SUM1_SUM2(0);
		sum1 = _ParaAdd(sum1, t3);
		sum2 = _ParaAdd(sum2, tabs);
		freqs_half++;
	}

#undef CAL_SUM1_SUM2
	return _ParaDiv(sum1, sum2);
}


void _OSlwToolSignalVmdCalLamda(
	ParaType *lamda,//复数 真实长度 2*Len
	ParaType *uh_arr,//复数矩阵 真实长度 uh_row*Len*2
	ParaType *freq_cpx,//复数 真实长度 2*Len
	ParaType tau,
	lw_u32 uh_row,//分解的数量
	lw_u32 Len//长度
)
{
	lw_u32 divm, modm, i;
	ParaType *py, *px;
	ParaType ntau;

	divm = Len * 2 / CACHE_READ_NUM, modm = (Len * 2) % CACHE_READ_NUM;
	ntau = _ParaMpy(tau, _ParaFrom(-1));

	py = lamda, px = freq_cpx;
	LwVectAddKX(py, ntau, px, divm, modm);

	px = uh_arr;
	for (i = 0; i < uh_row; i++) {
		py = lamda;
		LwVectAddKX(py, tau, px, divm, modm);
	}

}



void _OSlwToolSignalVmdT2W(
	ParaType *sig,//原始信号 实数 长度Len 
	ParaType *freq_cpx,//信号频域 Len*2*2
	ParaType *freq,//
	lw_u32 Len//长度
)
{
	lw_u32 i;
	ParaType *p, *q, temp1, delt;
	lw_u32 divm, modm;

	divm = Len / CACHE_READ_NUM, modm = Len%CACHE_READ_NUM;
	p = freq_cpx;

	for (i = 0; i < Len / 2; i++)
	{
		*p++ = sig[Len / 2 - i - 1];
	}

	q = sig;
	LwVectCopy(p, q, divm, modm);

	for (i = 0; i < Len / 2; i++)
	{
		*p++ = sig[Len - i - 1];
	}

	//对称之后长度是2倍
	LwFFTcomplex(freq_cpx, freq_cpx, Len * 2);

	LwFFT(freq_cpx, 2 * Len);

	//仅留下一半用于计算
	//divm *= 2;
	//p = freq_cpx;
	//q = freq_cpx + 2 * Len;
	//LwVectCopy(p, q, divm, modm);

	delt = _ParaDiv(_ParaFrom(1), _ParaFrom(2 * Len));
	temp1 = 0;
	for (i = 0; i < Len; i++)
	{
		freq[i] = temp1;
		temp1 = _ParaAdd(temp1, delt);
	}


	//LwFFTshift(freq_cpx, freq_cpx, 2 * Len);

}


void _OSlwToolSignalVmdW2T(
	ParaType *x,//时域信号 Len/2
	ParaType *freq_cpx,//计算完成的uh的一行 应该是一半的频域 Len
	ParaType *freq_temp,//用于保存中间值 长度2*Len
	lw_u32 Len//FFT的长度对于1024点的最原始信号应该是2048 
)
{
	lw_u32 i, divm, modm;
	ParaType *p, *q;

	divm = Len / CACHE_READ_NUM, modm = Len%CACHE_READ_NUM;

	p = freq_temp;
	q = freq_cpx;
	LwVectCopy(p, q, divm, modm);
	//共轭
	*p++ = freq_cpx[Len - 2];
	*p++ = _ParaMpy(freq_cpx[Len - 1], _ParaFrom(-1));

	for (i = 0; i < Len - 2; i += 2)
	{
		//*p++ = _ParaMpy(freq_cpx[Len - 1 - i], _ParaFrom(1));
		*p++ = freq_cpx[Len - 2 - i];
		*p++ = _ParaMpy(freq_cpx[Len - 1 - i], _ParaFrom(-1));
	}

	LwIFFT(freq_temp, Len);


	p = x;
	q = freq_temp + Len / 2;
	for (i = 0; i < Len / 2;)
	{
		*p++ = q[i];
		i += 2;
	}

}



ParaType OSlwToolSignalVmdSimple(
	LwMat *res_mat,
	ParaType *signal,
	ParaType alpha,
	ParaType tau,
	ParaType rtol,
	lw_u32 Max_loopN,
	LwMat *omega_mat,
	lw_mem pmem
)
{

	lw_u32 Len;
	lw_u32 dsize;
	lw_u32 loopN;
	lw_u32 i, n;
	lw_u32 i_temp;
	ParaType dsum, delt1;
	ParaType *px;

	ParaType *Freqs;
	ParaType *Freq_cpx;
	ParaType *omega_l;
	ParaType *uh;
	ParaType *uk;
	ParaType *lamda;
	ParaType *res;
	lw_u32 omega_save_len;
	ParaType *omega_mem;

	OSLW_assert(!(res_mat));
	OSLW_assert(!(signal));
	OSLW_assert(!(Max_loopN));
	OSLW_assert(!(pmem));

	//初始化数值
	Len = res_mat->col;
	dsize = res_mat->row;
	loopN = Max_loopN;

	//分配内存空间
	Freqs = pmem->Malloc(pmem, PARA_MEM_CAL(Len));
	Freq_cpx = pmem->Malloc(pmem, PARA_MEM_CAL(Len * 4ul));
	omega_l = pmem->Malloc(pmem, PARA_MEM_CAL(dsize));
	uh = pmem->Calloc(pmem, PARA_MEM_CAL(dsize * 2ul * Len));
	uk = pmem->Calloc(pmem, PARA_MEM_CAL(2ul * Len));
	lamda = pmem->Calloc(pmem, PARA_MEM_CAL(2ul * Len));
	//如果是空 就给他分配一个
	if (res_mat->a == NULL) {
		res = pmem->Calloc(pmem, PARA_MEM_CAL(dsize * Len));
	}
	else {
		res = res_mat->a;
	}

	//不需要保存 中心频率的变化
	if (omega_mat == NULL)
	{
		omega_save_len = 0;
		omega_mem = NULL;
	}
	else
	{
		//频率矩阵的列必须是分解次数
		if (omega_mat->col != dsize) {
			OSLW_assert(1);
			return 0;
		}

		//知道最大保存多少行
		omega_save_len = omega_mat->row;

		if (omega_mat->a == NULL) {
			omega_mem = pmem->Malloc(pmem, PARA_MEM_CAL(dsize*omega_save_len));
		}
		else {
			omega_mem = omega_mat->a;
		}
	}


	i_temp = dsize - 1;
	for (i = 0; i < dsize; i++) {
		omega_l[i] = _ParaMpy(_ParaFint(i), _ParaDiv(_ParaFrom(0.5), _ParaFint(dsize)));
	}
	//镜像延拓 + 扩展 +FFT
	_OSlwToolSignalVmdT2W(
		signal,
		Freq_cpx, Freqs,
		Len
	);

	px = omega_mem;
	for (n = 0; n < loopN; n++)
	{
		//保存数据
		if (n < omega_save_len) {
			for (i = 0; i < dsize; i++) {
				*px++ = omega_l[i];
			}
		}

		dsum = 0;
		for (i = 0; i < dsize; i++)
		{
			//计算uk
			_OSlwToolSignalVmdCalUk(
				uk,
				uh + i_temp * 2ul * Len,
				uh + i * 2ul * Len,
				Len
			);

			//计算Uh与误差的均方根
			delt1 = _OSlwToolSignalVmdCalUh(
				uh + i * 2ul * Len,
				Freq_cpx, uk, lamda,
				Freqs, alpha, omega_l[i], Len
			);
			//计算频率
			omega_l[i] = _OSlwToolSignalVmdCalOmega(
				Freqs, uh + i * 2ul * Len, Len
			);
			i_temp = i;
			dsum = _ParaAdd(dsum, delt1);

		}
		if (dsum < rtol) {
			break;
		}

		_OSlwToolSignalVmdCalLamda(
			lamda, uh, Freq_cpx,
			tau,
			dsize, Len
		);
	}

	for (i = 0; i < dsize; i++) {
		_OSlwToolSignalVmdW2T(
			res + i*Len, uh + i * 2ul * Len,
			Freq_cpx, 2 * Len
		);
	}

	//调节矩阵大小
	LwMatInit(res_mat, (LwMatRowType)n, (LwMatColType)(res_mat->col), res);
	if (omega_mat != NULL) {
		omega_mat->a = omega_mem;
	}

	//释放内存
	Freqs = pmem->Free(pmem, Freqs);
	Freq_cpx = pmem->Free(pmem, Freq_cpx);
	omega_l = pmem->Free(pmem, omega_l);
	uh = pmem->Free(pmem, uh);
	uk = pmem->Free(pmem, uk);
	lamda = pmem->Free(pmem, lamda);

	return dsum;
}

#endif // !(OSLW_SIMPLE_LEVEL >= 3)
