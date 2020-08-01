/*(Ver.=0.97)
* OSLW_tool.c
*
*  Created on: 2017-7-14
*      Author: slw
*/

#include "OSLW_include.h"
#if !(OSLW_SIMPLE_LEVEL >= 4)

#if OSLW_TOOL_IMPORT_MATH || OSLW_TOOL_IMPORT_ALL

OSlwToolMatrixSTU* OSlwToolMatrixInitial(OSlwToolMatrixSTU *m, LwMatRowType row, LwMatColType col, ParaType *a)
{
    OSLW_assert(!(m));
    m->a = a;
    m->row = row;
    m->col = col;
    m->length = row*col;
	return m;
}



OSLW_TOOL_FUN(
	OSlwToolMatrixSTU*, 
	OSlwToolMatrixAdd,          
	(OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *m1, OSlwToolMatrixSTU *m2),         
	OSlwToolMatrixAdd             
)
{

	register lw_u32 d, t_div, t_mod;
    register ParaType *ps, *pm1, *pm2, data;
    OSLW_assert(!(s));
    OSLW_assert(!(m1));
    OSLW_assert(!(m2));

    d = s->length;

    ps = s->a;
    pm1 = m1->a;
    pm2 = m2->a;
	t_div = d / CACHE_READ_NUM, t_mod = d%CACHE_READ_NUM;

    if (s->length == m1->length && m1->length == m2->length)//向量相加
    {
		LwVectCopy(ps, pm1, t_div, t_mod);
		ps = s->a;
		LwVectAddKX(ps, _ParaFint(1), pm2, t_div, t_mod);
    }
    else if(s->length == m1->length && m2->length == 1)
    {
        data = *(m2->a);
		LwVectSet(ps, data, t_div, t_mod);
		ps = s->a;
		LwVectAddKX(ps, _ParaFint(1), pm1, t_div, t_mod);
    }
    else if (s->length == m2->length && m1->length == 1)
    {
        data = *(m1->a);
		LwVectSet(ps, data, t_div, t_mod);
		ps = s->a;
		LwVectAddKX(ps, _ParaFint(1), pm2, t_div, t_mod);
    }
    else
    {
        return NULL;
    }

    return s;


}

OSLW_TOOL_FUN(
	OSlwToolMatrixSTU*, 
	OSlwToolMatrixSub,        
	(OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *m1, OSlwToolMatrixSTU *m2),         
	OSlwToolMatrixSub            
)
{
    
	register lw_u32 d, t_div, t_mod;
    register ParaType *ps, *pm1, *pm2, data;
    OSLW_assert(!(s));
    OSLW_assert(!(m1));
    OSLW_assert(!(m2));

    d = s->length;

    ps = s->a;
    pm1 = m1->a;
    pm2 = m2->a;

	t_div = d / CACHE_READ_NUM, t_mod =d%CACHE_READ_NUM;

    if (s->length == m1->length && m1->length == m2->length)
    {
		LwVectCopy(ps, pm1, t_div, t_mod);
		ps = s->a;
		LwVectAddKX(ps, _ParaFint(-1), pm2, t_div, t_mod);
    }
    else if (s->length == m1->length && m2->length == 1)
    {
		data = -(*(m2->a));//-m2+m1
		LwVectSet(ps, data, t_div, t_mod);
		ps = s->a;
		LwVectAddKX(ps, _ParaFint(-1), pm1, t_div, t_mod);

    }
    else if (s->length == m2->length && m1->length == 1)
    {
		data = *(m1->a);
		LwVectSet(ps, data, t_div, t_mod);
		ps = s->a;
		LwVectAddKX(ps, _ParaFint(-1), pm2, t_div, t_mod);
    }
    else
    {
        return NULL;
    }
    return s;
}

OSLW_TOOL_FUN(
	OSlwToolMatrixSTU*, 
	OSlwToolMatrixDot,          
	(OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *m1, OSlwToolMatrixSTU *m2),          
	OSlwToolMatrixDot 
)
{
	register lw_u32 d, t_div, t_mod;

    register ParaType *ps, *pm1, *pm2, data;

    OSLW_assert(!(s));
    OSLW_assert(!(m1));
    OSLW_assert(!(m2));

    d = s->length;

    ps = s->a;
    pm1 = m1->a;
    pm2 = m2->a;

	t_div = d / CACHE_READ_NUM, t_mod = d%CACHE_READ_NUM;

    if (s->length == m1->length && m1->length == m2->length)
    {
        while (d--)
        {
            *ps++ = _ParaMpy(*pm1++, *pm2++);
        }

    }
    else if (s->length == m1->length && m2->length == 1)
    {
        data = *(m2->a);
		LwVectCopy(ps, pm1, t_div, t_mod);
		ps = s->a;
		LwVectScale(ps, data, t_div, t_mod);
    }
    else if (s->length == m2->length && m1->length == 1)
    {
		data = *(m1->a);
		LwVectCopy(ps, pm2, t_div, t_mod);
		ps = s->a;
		LwVectScale(ps, data, t_div, t_mod);
    }
    else
    {
		OSLW_assert(1);
        return NULL;
    }
    return s;
}

OSLW_TOOL_FUN(
	OSlwToolMatrixSTU*, 
	OSlwToolMatrixMpy,
	(OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *m1, OSlwToolMatrixSTU *m2),
	OSlwToolMatrixMpy
)
{
    register LwMatColType j, row, col, row1;
	register LwMatLenType i;
	register ParaType temp, *_s, *_m1, *_m2, *_m1buf, *_sbuf, *_m2buf;
	OSLW_assert(!(s));
	OSLW_assert(!(m1));
	OSLW_assert(!(m2));

	if (s->row == m1->row && s->col == m2->col && m1->col == m2->row)//满足相乘条件
	{

#if defined(OSLW_USING_CBLAS) && OSLW_USING_CBLAS


		cblas_sgemm(
			CblasRowMajor, 
			CblasNoTrans, CblasNoTrans, 
			s->row, s->col, m1->col, 
			_ParaFint(1), m1->a, m1->col, 
			m2->a, m2->col, 
			(0), s->a, s->col
		);


#else
#if L1_L2_CACHE_OPTIM == 1
		lw_u32 div8, mod8;
		//lw_u32 div7, mod7;
		row = s->row;
		col = s->col;
		row1 = m2->row;
		_sbuf = s->a;
		_m1buf = m1->a;
		_m2buf = m2->a;

		div8 = col / CACHE_READ_NUM, mod8 = col % CACHE_READ_NUM;
		//div7 = row1 / 4, mod7 = row1 % 4;

		LW_MAT_CLR(s);

		for (i = 0, _m1 = _m1buf; i < row; ++i)
		{

			for (j = 0, _m2 = _m2buf; j < row1; ++j, ++_m1)
			{
				temp = *_m1;
				//改用VECT作为底层
				_s = _sbuf;
				LwVectAddKX(_s, temp, _m2, div8, mod8);
			}

			//_m2 = _m2buf;
			//for (j = 0; j < div7; ++j,_m1+=4)
			//{
			//	temp = _m1[0];_s = _sbuf;
			//	LwVectAddKX(_s, temp, _m2, div8, mod8);
			//	temp = _m1[1];_s = _sbuf;
			//	LwVectAddKX(_s, temp, _m2, div8, mod8);
			//	temp = _m1[2];_s = _sbuf;
			//	LwVectAddKX(_s, temp, _m2, div8, mod8);
			//	temp = _m1[3];_s = _sbuf;
			//	LwVectAddKX(_s, temp, _m2, div8, mod8);
			//	//temp = _m1[4];_s = _sbuf;
			//	//LwVectAddKX(_s, temp, _m2, div8, mod8);
			//	//temp = _m1[5];_s = _sbuf;
			//	//LwVectAddKX(_s, temp, _m2, div8, mod8);
			//	//temp = _m1[6];_s = _sbuf;
			//	//LwVectAddKX(_s, temp, _m2, div8, mod8);
			//	//temp = _m1[7];_s = _sbuf;
			//	//LwVectAddKX(_s, temp, _m2, div8, mod8);
			//	//temp = _m1[8];_s = _sbuf;
			//	//LwVectAddKX(_s, temp, _m2, div8, mod8);
			//	//temp = _m1[9];_s = _sbuf;
			//	//LwVectAddKX(_s, temp, _m2, div8, mod8);
			//	//temp = _m1[10];_s = _sbuf;
			//	//LwVectAddKX(_s, temp, _m2, div8, mod8);
			//	//temp = _m1[11];_s = _sbuf;
			//	//LwVectAddKX(_s, temp, _m2, div8, mod8);
			//	//temp = _m1[12];_s = _sbuf;
			//	//LwVectAddKX(_s, temp, _m2, div8, mod8);
			//	//temp = _m1[13];_s = _sbuf;
			//	//LwVectAddKX(_s, temp, _m2, div8, mod8);
			//	//temp = _m1[14];_s = _sbuf;
			//	//LwVectAddKX(_s, temp, _m2, div8, mod8);
			//	//temp = _m1[15];_s = _sbuf;
			//	//LwVectAddKX(_s, temp, _m2, div8, mod8);
			//}
			//for (j = 0; j < mod7; ++j,++_m1)
			//{
			//	temp = *_m1;_s = _sbuf;
			//	LwVectAddKX(_s, temp, _m2, div8, mod8);
			//}

			_sbuf = _s;
		}

#else
		row = s->row;
		col = m2->col;
		row1 = m2->row;
		_s = s->a;
		_m1buf = m1->a;

		for (i = 0;i<s->row;i++)
		{
			for (j = 0;j<s->col;j++)
			{

				//_m1=m1->a+i*m1->col;//m1的指针归位 m1归位于[n,1]
				_m1 = _m1buf;
				_m2 = m2->a + j;//m2的指针归位 m2归位于[1,n]
				temp = _ParaFrom(0);
				for (k = 0;k<row1;k++)
				{
					temp = _ParaAdd(temp, _ParaMpy(*_m1, *_m2));
					_m1++;
					_m2 += col;
				}//行向量*列向量
				*_s++ = temp;
			}
			_m1buf += m1->col;//m1的指针归位 m1归位于[n,1]
		}

#endif // L1_L2_CACHE_OPTIM == 1
#endif

	}
	else if ((m1->length == 1 || m2->length == 1) && (s->length == m1->length || s->length == m2->length) && s->length)
	{
		_s = s->a;
		if (m1->length == 1)
		{
			i = m2->length;
			_m1 = m2->a;
			temp = *(m1->a);
		}
		else
		{
			i = m1->length;
			_m1 = m1->a;
			temp = *(m2->a);
		}
		while (i--) *_s++ = _ParaMpy(temp, *_m1++);
	}
	else
	{
		return NULL;
	}
	return s;
}



OSLW_TOOL_FUN(
	OSlwToolMatrixSTU*, 
	OSlwToolMatrixTurn,          
	(OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *buf),          
	OSlwToolMatrixTurn             
)
{

 
    OSLW_assert(!(s));
	OSLW_assert(!(buf));

	if (s->row == buf->col && s->col >= buf->row && s->a != buf->a)
	{
		lw_u16 i, j;
		ParaType *_ms, *_mb, *_msb, *_mbb;
		lw_u16 sc, bc;
		_msb = s->a;
		_mbb = buf->a;
		sc = s->col;
		bc = buf->col;

		for (i = 0; i < buf->row; i++)
		{
			_ms = _msb;
			_mb = _mbb;
			for (j = 0;j < buf->col;j++)
			{
				*_ms = *_mb;
				_ms += sc;
				_mb++;
			}
			_msb++;
			_mbb += bc;
		}

	}
	else
	{
		OSLW_assert(1);
	}

    return s;


}



OSLW_TOOL_FUN(
	OSlwToolMatrixSTU*, 
	OSlwToolMatrixReSize,          
	(OSlwToolMatrixSTU *s, LwMatRowType row, LwMatColType col),
	OSlwToolMatrixReSize            
)
{

    OSLW_assert(!(s));
    if (row <= s->row && col <= s->col)
    {
        s->row = row;
        s->col = col;
        s->length = row*col;
    }



    return s;


}


OSLW_TOOL_FUN(OSlwToolMatrixSTU*, OSlwToolMatrixMaxMin,
              (OSlwToolMatrixSTU *s, ParaType *pMax, lw_u16 *pMaxIndex, ParaType *pMin, lw_u16 *pMinIndex),
              OSlwToolMatrixMaxMin
             )
{
    register lw_u16 i;
    register ParaType dmax, dmin, *p;

    OSLW_assert(!(s));

    if ((pMax && pMaxIndex) && !(pMin && pMinIndex))
    {
        p = s->a;
        dmax = s->a[0];
        *pMaxIndex = 0;
        for (i = 1; i<s->length; i++)
        {
            if (p[i] >= dmax)
            {
                dmax = p[i];
                *pMaxIndex = i;
            }
        }
        *pMax = dmax;
    }
    else if (!(pMax && pMaxIndex) && (pMin && pMinIndex))
    {

        p = s->a;
        dmin = s->a[0];
        *pMinIndex = 0;
        for (i = 1; i<s->length; i++)
        {
            if (p[i] < dmin)
            {
                dmin = p[i];
                *pMinIndex = i;
            }
        }
        *pMin = dmin;

    }
    else if ((pMax && pMaxIndex) && (pMin && pMinIndex))
    {
        p = s->a;
        dmax = s->a[0];
        dmin = s->a[0];
        *pMaxIndex = 0;
        *pMinIndex = 0;
        for (i = 1; i<s->length; i++)
        {
            if (p[i] > dmax)
            {
                dmax = p[i];
                *pMaxIndex = i;
            }

            if (p[i] < dmin)
            {
                dmin = p[i];
                *pMinIndex = i;
            }

        }

        *pMax = dmax;
        *pMin = dmin;
    }
    else;


    return s;
}

OSLW_TOOL_FUN(OSlwMartixResNum, OSlwToolMatrixCmp,
              (OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *m, lw_u8 *pbuf),
              OSlwToolMatrixCmp
             )
{
	OSLW_assert(!(s));
	OSLW_assert(!(m));

    if (pbuf == NULL)
    {
        if (s->length == m->length && (s->a) && (m->a))
        {
            return memcmp(s->a, m->a, PARA_MEM_CAL(s->length)) == 0 ? OSlwMartixRes_Find : OSlwMartixRes_NoFind;
        }
        else
        {
            return OSlwMartixRes_NoFind;
        }
    }

    return OSlwMartixRes_Error;
}

OSLW_TOOL_FUN(OSlwMartixResNum, OSlwToolMatrixFind,
              (OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *m, lw_u16 SpacingLen, lw_u16 First, lw_u16 *pres),
              OSlwToolMatrixFind
             )
{
    register ParaType *ps, *pa, *pbottom;
    register lw_u16 len, count = 0;


    len = SpacingLen ? SpacingLen : m->length;

    if ((lw_32)(s->length - First) >= m->length)//搜索目标的长度要大于被搜索的长度 不然没有搜索的意义
    {
        ps = s->a + First;
        pa = m->a;
        pbottom = ps + s->length;

        do
        {
            if (memcmp(ps, pa, PARA_MEM_CAL(len)) == 0)
            {
                len = 0;
                break;
            }
            count++;
            ps += len;
        } while (ps < pbottom);
        if (!len && pres)//说明找到了
        {
            *pres = count;
            return OSlwMartixRes_Find;
        }
        else
        {
            return OSlwMartixRes_NoFind;
        }

    }
    else
    {
        return OSlwMartixRes_Over;
    }

    
}


OSLW_TOOL_FUN(
	OSlwMartixResNum, 
	OSlwToolMatrixIndex,          
	(OSlwToolMatrixSTU *s, ParaType data, lw_u16 *pindex, lw_u16 *pmaxlen),          
	OSlwToolMatrixIndex 
)
{
    register lw_u16 i, maxlen;
    OSLW_assert(!(s));
    OSLW_assert(!(pindex));
    OSLW_assert(!(pmaxlen));


    maxlen = *pmaxlen;
    for (i = 0; i < s->length; i++)
    {
        if (s->a[i] == data)
        {
            *pindex++ = i;
            if (!(--maxlen))
                break;
        }
    }
    (*pmaxlen) -= maxlen;
    return OSlwMartixRes_Find;
}


OSLW_TOOL_FUN(OSlwToolMatrixSTU*, OSlwToolMatrixSet,
              (OSlwToolMatrixSTU *s, ParaType data, OSlwToolMatrixSTU *a),
              OSlwToolMatrixSet
             )
{
	register lw_u32 d, t_div, t_mod;
	register ParaType *ps, *pa;
    OSLW_assert(!(s));

	d = s->length;
	ps = s->a;
	pa = a->a;
	t_div = d / CACHE_READ_NUM, t_mod = d%CACHE_READ_NUM;

    if (a)//如果定义a 相当于复制构造函数
    {
        if (s->length == a->length)
        {
			LwVectCopy(ps, pa, t_div, t_mod);
            //memcpy(s->a, a->a, PARA_MEM_CAL(s->length));
        }
        else
        {
            OSLW_assert(1);
        }

    }
    else//使用常量初始化
    {
		data = *pa;
		LwVectSet(ps, data, t_div, t_mod);
    }

    return s;
}




OSLW_TOOL_FUN(OSlwToolMatrixSTU*, OSlwToolMatrixJoin,
(OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *m1, OSlwToolMatrixSTU *m2,lw_u8 dim), OSlwToolMatrixJoin)
{
	register LwMatLenType i, tdiv1, tmod1, tdiv2, tmod2;
	register ParaType *sa, *m1a, *m2a;
	OSLW_assert(!(s));
	OSLW_assert(!(m1));
	OSLW_assert(!(m2));
	OSLW_assert((s==m2));

	if (s == m1 && s->length >= m2->length)
	{
		i = m2->length;
		sa = s->a + (s->length - m2->length);
		m2a = m2->a;
		while (i--)
		{
			*sa++ = *m2a++;
		}
		return s;
	}

	sa = s->a;
	m1a = m1->a;
	m2a = m2->a;
	OSLW_assert((sa == m1a));
	OSLW_assert((sa == m2a));

	switch (dim)
	{
	//按照向量拼接
	case 0x00:
		tdiv1 = m1->length / CACHE_READ_NUM, tmod1 = m1->length%CACHE_READ_NUM;
		tdiv2 = m2->length / CACHE_READ_NUM, tmod2 = m2->length%CACHE_READ_NUM;;
		if (s->length >= (m1->length + m2->length))//满足拼接条件
		{
			LwVectCopy(sa, m1a, tdiv1, tmod1);
			LwVectCopy(sa, m2a, tdiv2, tmod2);
		}
		break;

	//行拼接 
	/*
	a11 a12 a13...|b11 b12 b13...
	a21 a22 a23...|b21 b22 b23...
	...
	*/
	case 0x01:
		tdiv1 = m1->col / CACHE_READ_NUM, tmod1 = m1->col%CACHE_READ_NUM;
		tdiv2 = m2->col / CACHE_READ_NUM, tmod2 = m2->col%CACHE_READ_NUM;;
		if (s->length >= (m1->length + m2->length) && (m1->row == m2->row))//满足拼接条件
		{
			for (i = 0; i < m1->row; i++)
			{
				LwVectCopy(sa, m1a, tdiv1, tmod1);
				LwVectCopy(sa, m2a, tdiv2, tmod2);
			}
		}
		break;
	//列拼接 要求s的列数>m1与m2的列数
	/*
	a11 a12 a13 a14 a15 s16 s17.. 
	a21 a22 a23 a24 a25 s26 s27.. 
	....
	b11 b12 b13 sn4 sn5 
	...
	*/
	case 0x02:
		if (s->row >= (m1->row + m2->row) && (s->col >= m1->col) && (s->col >= m2->col))//满足拼接条件
		{
			tdiv1 = m1->col / CACHE_READ_NUM, tmod1 = m1->col%CACHE_READ_NUM;
			tdiv2 = m2->col / CACHE_READ_NUM, tmod2 = m2->col%CACHE_READ_NUM;;

			for (i = 0; i < m1->row; i++)
			{
				sa = OSlwMatGetR(*s, i);
				LwVectCopy(sa, m1a, tdiv1, tmod1);
			}
			for (i = 0; i < m2->row; i++)
			{
				sa = OSlwMatGetR(*s, i + m1->row);
				LwVectCopy(sa, m2a, tdiv2, tmod2);
			}


		}
		break;
	default:
		break;
	}




	return s;
}



OSLW_TOOL_FUN(ParaType, OSlwToolMatrixLossSquare,
              (OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *ref, OSlwToolMatrixSTU *pre),
              OSlwToolMatrixLossSquare
             )
{

    lw_u16 i = 0;
    ParaType sum = _ParaFint(0), temp;
    OSLW_assert(!(ref));
    OSLW_assert(!(pre));
    if (ref->length == pre->length)
    {
		if (!s)
		{
			for (i = 0; i<ref->length; i++)
			{
				temp = ref->a[i] - pre->a[i];
				sum = _ParaAdd(sum, _ParaMpy(_ParaFrom(0.5), _ParaMpy(temp, temp)));
			}
		}
		else
		{
			if (s->length == ref->length)
			{
				for (i = 0; i<ref->length; i++)
				{
					temp = ref->a[i] - pre->a[i];
					sum = _ParaAdd(sum, _ParaMpy(_ParaFrom(0.5), _ParaMpy(temp, temp)));
					s->a[i] = temp;
				}
			}
		}


    }

    return sum;
}


OSLW_TOOL_FUN(ParaType, OSlwToolMatrixLossCrossEntropyForSoftMax,
(OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *ref, OSlwToolMatrixSTU *pre),
OSlwToolMatrixLossCrossEntropyForSoftMax
)
{

	lw_u16 i = 0, j , col;
	ParaType sum = _ParaFint(0);
	ParaType *_pre_b, *_s_b, *_ref_b;
	OSLW_assert(!(ref));
	OSLW_assert(!(pre));
	OSLW_assert(!(s));
	if (ref->length == pre->length)
	{
			if (s->length == ref->length)
			{

#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C
				_s_b = s->a;
				_ref_b = ref->a;
				_pre_b = pre->a;
				col = s->col;
				for (; i < s->row; i++)
				{			
					for ( j = 0; j < col; ++j, ++_s_b, ++_ref_b, ++_pre_b)
					{
						sum+= _ParaMpy(_ParaFrom(*_ref_b), _ParaLn(*_pre_b+ OSLW_GLOBAL_MATH_DELT));
						*_s_b= *_ref_b - *_pre_b;
					}
				}

#elif OSLW_TOOL_NN_DATA_FRAME == OSLW_TOOL_NN_D_FRAME_F
				_s_b = s->a;
				_ref_b = ref->a;
				_pre = pre->a;
				for (i = 0; i < s->col; i++)
				{
					_s = _s_b;
					_ref = _ref_b;
					_pre = _pre_b;

					//查找最大值
					_n_max = *_ref;
					_i_max = 0;
					for (j = 0; j < s->row; j++)
					{
						if (*_ref > _n_max)
						{
							_n_max = *_ref;
							_i_max = j;
						}

						*_s = *_pre;
						_s += col;
						_pre += col;
						_ref += col;
					}

					*(_s_b + col*_i_max) = _ParaSub(*(_s_b + col*_i_max), _ParaFint(1));

					_s_b++;
					_ref_b++;
					_pre_b++;
				}
#endif

			}
		}

	

	return -sum;
}
/*
OSLW_TOOL_FUN(ParaType, OSlwToolMatrixSum,
(OSlwToolMatrixSTU *s),
OSlwToolMatrixSum
)
{

	ParaType *_s, sum = _ParaFrom(0);
	lw_u32 i;
	OSLW_assert(s);

	i = s->length;
	_s = s->a;
	while (i--)
	{
		sum = _ParaAdd(sum, *_s++);
	}


	return sum;
}


OSLW_TOOL_FUN(ParaType, OSlwToolMatrixVar,
(OSlwToolMatrixSTU *s, ParaType *pMean, OSlwToolMatrixSTU *_normal_res,ParaType epsi),
OSlwToolMatrixVar
)
{
	ParaType _mean;
	ParaType _var = _ParaFint(0);
	ParaType *_s, *_nr;
	ParaType _one_div;
	lw_u32 i;

	if (pMean)
	{
		_mean = *pMean;
	}
	else
	{
		_mean = _ParaDiv(pOSlwToolMatrixSum(s), _ParaFint(s->length));
	}


	i = s->length;
	_s = s->a;

	while (i--)
	{
		_var = _ParaAdd(_var, _ParaMpy(_ParaSub(*_s, _mean), _ParaSub(*_s, _mean)));
		_s++;
	}

	_var = _ParaDiv(_var, _ParaFint(s->length));

	if (_normal_res)
	{
		i = _normal_res->length;
		_s = s->a;
		_nr = _normal_res->a;
		_one_div = _ParaDiv(_ParaFint(1), _ParaSqrt(_ParaAdd(_var, epsi)));

		while (i--)
		{
			*_nr++ = _ParaMpy(_ParaSub(*_s, _mean), _one_div);
			_s++;
		}
	}

	return _var;


}
*/

OSLW_TOOL_FUN(ParaType, OSlwToolMatrixSum,
(OSlwToolMatrixSTU *DST, OSlwToolMatrixSTU *SRC, lw_u8 dim),
OSlwToolMatrixSum
)
{
	register ParaType _sum = 0;
	register LwMatRowType r;
	register LwMatColType c;
	register LwMatLenType L;
	register LwMatLenType t_div, t_mod;
	register lw_u32 i, j;
	register ParaType *s, *d, *si;

	OSLW_assert(!(SRC));
	L = SRC->length;
	r = SRC->row;
	c = SRC->col;
	s = SRC->a;
	d = DST->a;
	switch (dim)
	{
	case 0x00:
		t_div = L / CACHE_READ_NUM, t_mod = L%CACHE_READ_NUM;
		LwVectSum(_sum, s, t_div, t_mod);

		break;

	case 0x10:
		OSLW_assert(!(DST));
		t_div = L / CACHE_READ_NUM, t_mod = L%CACHE_READ_NUM;
		LwVectSum(_sum, s, t_div, t_mod);

		*d = _ParaAdd(*d, _sum);
		break;

	case 0x01:
		OSLW_assert(!(DST));
		OSLW_assert(!(DST->length >= r));

		d = DST->a;
		t_div = c / CACHE_READ_NUM, t_mod = c%CACHE_READ_NUM;
		while (r--)
		{
			LwVectSum(_sum, s, t_div, t_mod);
			*d++ = _sum;
		}
		break;

	case 0x11:
		OSLW_assert(!(DST));
		OSLW_assert(!(DST->length >= r));
		d = DST->a;
		t_div = c / CACHE_READ_NUM, t_mod = c%CACHE_READ_NUM;
		while (r--)
		{
			LwVectSum(_sum, s, t_div, t_mod);
			*d = _ParaAdd(*d, _sum);
			++d;
		}
		break;

	case 0x02:
		OSLW_assert(!(DST));
		OSLW_assert(!(DST->length >= c));
		d = DST->a;
		j = c;
		while (j--)
		{
			i = r;
			_sum = _ParaFrom(0);
			si = s++;
			while (i--)
			{
				_sum = _ParaAdd(_sum, *si);
				si += c;
			}
			*d++ = _sum;
		}
		break;

	case 0x12:
		OSLW_assert(!(DST));
		OSLW_assert(!(DST->length >= c));
		d = DST->a;
		j = c;
		while (j--)
		{
			i = r;
			_sum = _ParaFrom(0);
			si = s++;
			while (i--)
			{
				_sum = _ParaAdd(_sum, *si);
				si += c;
			}
			*d = _ParaAdd(*d, _sum);
			++d;
		}
		break;

	default:
		OSLW_assert(1);
		break;
	}
	return _sum;
}


//s=x*we+bi
//神经网络中(放弃)
// xout=(Weight * xin) +Bias
OSLW_TOOL_FUN(OSlwToolMatrixSTU*, OSlwToolMatrixMPYA,
(OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *x, OSlwToolMatrixSTU *we, OSlwToolMatrixSTU *bi),
OSlwToolMatrixMPYA
)
{
	//
	//lw_u16 i, j, k, row, col, row1;
	//ParaType sum, *_s, *_x, *_we, *_xbuf, *_bi;
	//ParaType _we_a, _bi_a;
	//ParaType stas;
	//OSLW_assert(!(s));
	//OSLW_assert(!(x));
	//OSLW_assert(!(we));
	//OSLW_assert(!(bi));
	////神经网络前向传播
	//if (s->row == x->row && s->col == we->col && x->col == we->row && s->length == bi->length)//满足相乘条件
	//{
	//	row = s->row;
	//	col = we->col;
	//	row1 = we->row;
	//	_s = s->a;
	//	_xbuf = x->a;
	//	_bi = bi->a;
	//	for (i = 0; i<s->row; i++)
	//	{
	//		for (j = 0; j<s->col; j++)
	//		{
	//			//_x=x->a+i*x->col;//x的指针归位 x归位于[n,1]
	//			_x = _xbuf;
	//			_we = we->a + j;//we的指针归位 we归位于[1,n]
	//			sum = _ParaFrom(0);
	//			for (k = 0; k<row1; k++)
	//			{
	//				sum = _ParaAdd(sum, _ParaMpy(*_x, *_we));
	//				_x++;
	//				_we += col;
	//			}//行向量*列向量
	//			*_s++ = _ParaAdd(sum, *_bi);
	//			_bi++;
	//		}
	//		_xbuf += x->col;//x的指针归位 x归位于[n,1]
	//	}
	//}
	////
	//else if((bi->col == bi->row && bi->row == bi->length && bi->length >= 2) && (s->row == x->row && s->col == we->col && x->col == we->row))//特殊统计命令
	//{
	//	row = s->row;
	//	col = we->col;
	//	row1 = we->row;
	//	_s = s->a;
	//	_xbuf = x->a;
	//	stas = _ParaFint(0);
	//	for (i = 0; i<s->row; i++)
	//	{
	//		for (j = 0; j<s->col; j++)
	//		{
	//			//_x=x->a+i*x->col;//x的指针归位 x归位于[n,1]
	//			_x = _xbuf;
	//			_we = we->a + j;//we的指针归位 we归位于[1,n]
	//			sum = _ParaFrom(0);
	//			for (k = 0; k<row1; k++)
	//			{
	//				sum = _ParaAdd(sum, _ParaMpy(*_x, *_we));
	//				_x++;
	//				_we += col;
	//			}//行向量*列向量
	//			*_s++ = sum;
	//			stas = _ParaAdd(stas, sum);//统计累加
	//		}
	//		_xbuf += x->col;//x的指针归位 x归位于[n,1]
	//	}
	//	bi->a[0] = _ParaDiv(stas, _ParaFrom(s->length));//计算平均值
	//}
	//else if (s->length == x->length && we->length ==1)
	//{
	//	//矩阵s=数字w*矩阵x+矩阵b
	//	if (bi->length == s->length)
	//	{
	//		_we_a = *(we->a);
	//		_bi = bi->a;
	//		_s = s->a;
	//		_xbuf = x->a;
	//		i = s->length;
	//		while (i--)
	//		{
	//			*_s = _ParaAdd(*_bi, _ParaMpy((*_xbuf), _we_a));
	//			
	//			_s++;
	//			_xbuf++;
	//			_bi++;
	//		}
	//	}
	//	//矩阵s=数字w*矩阵x+数字b
	//	else if (bi->length == 1)
	//	{
	//		_we_a = *(we->a);
	//		_bi_a = *(bi->a);
	//		_s = s->a;
	//		_xbuf = x->a;
	//		i = s->length;
	//		while (i--)
	//		{
	//			*_s = _ParaAdd(_bi_a, _ParaMpy((*_xbuf), _we_a));
	//			_s++;
	//			_xbuf++;
	//		}
	//	}
	//	else
	//	{
	//		OSLW_assert(1);
	//	}
	//}
	//else
	//{
	//	OSLW_assert(1);
	//}
	return s;
}
OSLW_TOOL_FUN(
	OSlwToolMatrixSTU*,
	OSlwToolMatrixWeXBi,
	(OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *we, OSlwToolMatrixSTU *x, OSlwToolMatrixSTU *bi),
	OSlwToolMatrixWeXBi
)
{
	OSLW_assert(1);
	return NULL;
}
OSLW_TOOL_FUN(
	OSlwToolMatrixSTU*,
	OSlwToolMatrixXWeBi,
	(OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *we, OSlwToolMatrixSTU *x, OSlwToolMatrixSTU *bi),
	OSlwToolMatrixXWeBi
)
{

#if L1_L2_CACHE_OPTIM == 1
	//register LwMatLenType i, j, k, row, col, row1;
	//register ParaType temp, *_s, *_sbuf, *_x, *_we, *_web, *_xb, *_bi, *_bibuf;

	register LwMatColType  row, col/*, row1*/;
	register LwMatLenType i;
	register ParaType  *_s,/* *_m1buf,*/ *_sbuf,/* *_m2buf,*/ *_bibuf, *_bi;
	register LwMatLenType div8, mod8;
	OSLW_assert(!(s));
	OSLW_assert(!(x));
	OSLW_assert(!(we));
	OSLW_assert(!(bi));
	//神经网络前向传播
	if (s->col == we->col && s->row == x->row && we->row == x->col && s->col == bi->length)//满足神经网络前向条件
	{
		row = s->row;
		col = s->col;
		//row1 = x->col;
		_bibuf = bi->a;
		_sbuf = s->a;
		//_m1buf = x->a;
		//_m2buf = we->a;
		div8 = col / CACHE_READ_NUM, mod8 = col%CACHE_READ_NUM;
		//先载入偏置
		if (_bibuf)
		{
			for (i = 0, _s = _sbuf; i < row; i++)
			{
				_bi = _bibuf;
				//for (j = 0 ; j < col; j++)
				//	*_s++ = *_bi++;
				LwVectCopy(_s, _bi, div8, mod8);
			}
		}
		else
		{
			LW_MAT_CLR(s);
		}

		LwMatTurnMpy(s, x, we, 0x10);


		//for (i = 0, _m1 = _m1buf; i < row; ++i)
		//{
		//	for (j = 0, _m2 = _m2buf; j < row1; ++j, ++_m1)
		//	{
		//		temp = *_m1;
		//		for (k = 0, _s = _sbuf; k < col; ++k, ++_m2, ++_s)
		//		{
		//			*_s = _ParaAdd(*_s, _ParaMpy(temp, *_m2));
		//		}
		//	}
		//	_sbuf = _s;
		//}
	}
#else
	register lw_u16 i, j, k, row, col, row1;
	register ParaType sum, *_s, *_x, *_we, *_web, *_xb, *_bi;
	register ParaType *_we_a, *_bi_a;
	OSLW_assert(!(s));
	OSLW_assert(!(x));
	OSLW_assert(!(we));
	OSLW_assert(!(bi));
	//神经网络前向传播
	if (s->col == we->col && s->row == x->row && we->row == x->col && s->col == bi->length)//满足神经网络前向条件
	{
		row = s->row;
		col = we->col;
		row1 = we->row;
		_s = s->a;
		_xb = x->a;
		_web = we->a;
		_bi = bi->a;
		_we_a = we->a;
		_bi_a = bi->a;
		for (i = 0; i<row; i++)
		{
			for (j = 0; j<col; j++)
			{
				_we = _web;
				_x = _xb;
				sum = _ParaFrom(0);
				for (k = 0; k<row1; k++)
				{
					sum = _ParaAdd(sum, _ParaMpy(*_x, *_we));
					_we += col;
					_x++;
				}//行向量*列向量
				*_s++ = _ParaAdd(sum, *_bi);
				_bi++;
				_web++;
			}
			_xb += row1;
			_bi = _bi_a;
			_web = _we_a;
		}
	}
#endif

	return s;
}


OSLW_TOOL_FUN(
	OSlwToolMatrixSTU*,
	OSlwToolMatrixTurnMpy,
	(OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *m1, OSlwToolMatrixSTU *m2, lw_u8 flag),
	OSlwToolMatrixTurnMpy
)
{
#if defined(OSLW_USING_CBLAS) && OSLW_USING_CBLAS
	OSLW_assert(!(s));
	OSLW_assert(!(m1));
	OSLW_assert(!(m2));


	switch (flag)
	{
	case 0x00://都不用转置
		if (s->row == m1->row && s->col == m2->col && m1->col == m2->row)
		{

			cblas_sgemm(
				CblasRowMajor,
				CblasNoTrans, CblasNoTrans,
				s->row, s->col, m2->row,
				_ParaFint(1), m1->a, m1->col,
				m2->a, m2->col,
				_ParaFint(0), s->a, s->col
			);

		}
		else
		{
			OSLW_assert(1);
			return NULL;
		}
	case 0x10:
		if (s->row == m1->row && s->col == m2->col && m1->col == m2->row)
		{
			cblas_sgemm(
				CblasRowMajor,
				CblasNoTrans, CblasNoTrans,
				s->row, s->col, m2->row,
				_ParaFint(1), m1->a, m1->col,
				m2->a, m2->col,
				_ParaFint(1), s->a, s->col
			);

		}
		else
		{
			OSLW_assert(1);
			return NULL;
		}
		break;
	case 0x01://m2转置
		if (s->row == m1->row && s->col == m2->row && m1->col == m2->col)
		{
			cblas_sgemm(
				CblasRowMajor,
				CblasNoTrans, CblasTrans,
				s->row, s->col, m1->col,
				_ParaFint(1), m1->a, m1->col,
				m2->a, m2->col,
				_ParaFint(0), s->a, s->col
			);

		}
		else
		{
			OSLW_assert(1);
			return NULL;
		}
	case 0x11:
		if (s->row == m1->row && s->col == m2->row && m1->col == m2->col)
		{
			cblas_sgemm(
				CblasRowMajor,
				CblasNoTrans, CblasTrans,
				s->row, s->col, m1->col,
				_ParaFint(1), m1->a, m1->col,
				m2->a, m2->col,
				_ParaFint(1), s->a, s->col
			);
		}
		else
		{
			OSLW_assert(1);
			return NULL;
		}
		break;

	case 0x02://m1转置
		if (s->row == m1->col && s->col == m2->col && m1->row == m2->row)
		{
			cblas_sgemm(
				CblasRowMajor,
				CblasTrans, CblasNoTrans,
				s->row, s->col, m1->col,
				_ParaFint(1), m1->a, m1->col,
				m2->a, m2->col,
				_ParaFint(0), s->a, s->col
			);	

		}
		else
		{
			OSLW_assert(1);
			return NULL;
		}
	case 0x12:
		if (s->row == m1->col && s->col == m2->col && m1->row == m2->row)
		{
			cblas_sgemm(
				CblasRowMajor,
				CblasTrans, CblasNoTrans,
				s->row, s->col, m1->col,
				_ParaFint(1), m1->a, m1->col,
				m2->a, m2->col,
				_ParaFint(1), s->a, s->col
			);
		}
		else
		{
			OSLW_assert(1);
			return NULL;
		}
		break;

	case 0x03://都转置
		if (s->row == m1->col && s->col == m2->row && m1->row == m2->col)
		{
			cblas_sgemm(
				CblasRowMajor,
				CblasTrans, CblasNoTrans,
				s->row, s->col, m1->row,
				_ParaFint(1), m1->a, m1->col,
				m2->a, m2->col,
				_ParaFint(0), s->a, s->col
			);

		}
		else
		{
			OSLW_assert(1);
			return NULL;
		}
	case 0x13:
		if (s->row == m1->col && s->col == m2->row && m1->row == m2->col)
		{
			cblas_sgemm(
				CblasRowMajor,
				CblasTrans, CblasNoTrans,
				s->row, s->col, m1->row,
				_ParaFint(1), m1->a, m1->col,
				m2->a, m2->col,
				_ParaFint(1), s->a, s->col
			);
		}
		else
		{
			OSLW_assert(1);
			return NULL;
		}
		break;

	default:
		OSLW_assert(1);
		break;
	}

#else

#if L1_L2_CACHE_OPTIM == 1

	OSLW_assert(!(s));
	OSLW_assert(!(m1));
	OSLW_assert(!(m2));

	switch (flag)
	{
	case 0x00://都不用转置
		LW_MAT_CLR(s);
	case 0x10:
		if (s->row == m1->row && s->col == m2->col && m1->col == m2->row)
		{
			register LwMatLenType i, j, row = s->row, col = s->col, row1 = m2->row;
			register ParaType temp, *_s, *_m1, *_m2, *_m1buf= m1->a, *_sbuf= s->a, *_m2buf= m2->a;
			register LwMatLenType div8, mod8;
			div8 = col / CACHE_READ_NUM, mod8 = col % CACHE_READ_NUM;
			for (i = 0, _m1 = _m1buf; i < row; ++i)
			{
				for (j = 0, _m2 = _m2buf; j < row1; ++j, ++_m1)
				{
					temp = *_m1;
					//for (k = 0, _s = _sbuf; k < col; ++k, ++_m2, ++_s)
					//{
					//	*_s = _ParaAdd(*_s, _ParaMpy(temp, *_m2));
					//}
					_s = _sbuf;
					LwVectAddKX(_s, temp, _m2, div8, mod8);
				}
				_sbuf = _s;
			}

		}
		else
		{
			OSLW_assert(1);
			return NULL;
		}
		break;
	case 0x01://m2转置
		LW_MAT_CLR(s);		
	case 0x11:
		if (s->row == m1->row && s->col == m2->row && m1->col == m2->col)
		{
			register LwMatLenType i, j, row = s->row, col = s->col, row1 = m1->col;
			register ParaType temp, *_s, *_m1, *_m2, *_m1buf = m1->a, *_sbuf = s->a, *_m2buf = m2->a;
			register LwMatLenType div8, mod8;
			div8 = row1 / CACHE_READ_NUM, mod8 = row1 % CACHE_READ_NUM;
			for (i = 0, _s = _sbuf; i < row; ++i)
			{
				for (j = 0, _m2 = _m2buf; j < col; ++j,++_s)
				{
					//temp = _ParaFint(0);
					//for (k = 0, _m1 = _m1buf; k < row1; ++k, ++_m2,++_m1)
					//{
					//	temp = _ParaAdd(temp, _ParaMpy(*_m1, *_m2));
					//}
					_m1 = _m1buf;
					LwVectDot(temp, _m1, _m2, div8, mod8);
					*_s = _ParaAdd(*_s, temp);
				}
				_m1buf = _m1;
			}
		}
		else
		{
			OSLW_assert(1);
			return NULL;
		}
		break;



	case 0x02://m1转置
		LW_MAT_CLR(s);
	case 0x12:
		if (s->row == m1->col && s->col == m2->col && m1->row == m2->row)
		{
			register LwMatLenType i, j, row = s->row, col = s->col, row1 = m2->row;
			register ParaType temp, *_s, *_m1, *_m2, *_m1buf = m1->a, *_sbuf = s->a, *_m2buf = m2->a;
			register LwMatLenType div8, mod8;
			div8 = col / CACHE_READ_NUM, mod8 = col % CACHE_READ_NUM;

			for (i = 0, _m1 = _m1buf; i < row1; ++i)
			{
				for (j = 0, _s = _sbuf; j < row; ++j, ++_m1)
				{	
					temp = *_m1;
					//for (k = 0,_m2=_m2buf; k < col; ++k, ++_s, ++_m2)
					//{
					//	*_s = _ParaAdd(*_s, _ParaMpy(temp, *_m2));
					//}
					_m2 = _m2buf;
					LwVectAddKX(_s, temp, _m2, div8, mod8);
				}
				_m2buf = _m2;
			}

		}
		else
		{
			OSLW_assert(1);
			return NULL;
		}
		break;

	case 0x03://都转置
		LW_MAT_CLR(s);
	case 0x13:
		if (s->row == m1->col && s->col == m2->row && m1->row == m2->col)
		{
			register LwMatLenType i, j, k, row = s->row, col = s->col, row1 = m1->row;
			register ParaType temp, *_s, *_m1, *_m2, *_m1buf = m1->a,/* *_sbuf1 = s->a,*/ *_sbuf2 = s->a, *_m2buf = m2->a;
			//register LwMatLenType div8, mod8;
			//div8 = row / CACHE_READ_NUM, mod8 = row % CACHE_READ_NUM;

			for (i = 0, _m2 = _m2buf; i < col; ++i, ++_sbuf2)
			{
				for (j = 0, _m1 = _m1buf; j < row1; ++j, ++_m2)
				{
					temp = *_m2;
					//s有间隔 无法使用Vect函数
					for (k = 0, _s = _sbuf2; k < row; ++k, ++_m1, _s += col)
					{
						*_s = _ParaAdd(*_s, _ParaMpy(temp, *_m1));
					}
				}
			}
		}
		else
		{
			OSLW_assert(1);
			return NULL;
		}
		break;

	default:
		OSLW_assert(1);
		break;
	}
#else

	register LwMatLenType i, j, k, row, col, row1;
	register LwMatLenType m1d1, m1d3, m2d2, m2d3;
	register ParaType sum, *_s, *_m1, *_m2, *_m1b, *_m2b;
	register ParaType _we_a, _bi_a;

	OSLW_assert(!(s));
	OSLW_assert(!(m1));
	OSLW_assert(!(m2));


	switch (flag & 0x03)
	{
	case 0://都不用转置
		if (s->row == m1->row && s->col == m2->col && m1->col == m2->row)
		{
			m1d1 = m1->col;
			m1d3 = 1;
			m2d2 = 1;
			m2d3 = m2->col;

			row1 = m1->col;

		}
		else
		{
			OSLW_assert(1);
			return NULL;
		}
		break;

	case 1://m2转置
		if (s->row == m1->row && s->col == m2->row && m1->col == m2->col)
		{
			m1d1 = m1->col;
			m1d3 = 1;

			m2d2 = m2->col;
			m2d3 = 1;

			row1 = m1->col;

		}
		else
		{
			OSLW_assert(1);
			return NULL;
		}

		break;

	case 2://m1转置

		if (s->row == m1->col && s->col == m2->col && m1->row == m2->row)
		{
			m1d1 = 1;
			m1d3 = m1->col;

			m2d2 = 1;
			m2d3 = m2->col;

			row1 = m1->row;
		}
		else
		{
			OSLW_assert(1);
			return NULL;
		}
		break;

	case 3://都转置
		if (s->row == m1->col && s->col == m2->row && m1->row == m2->col)
		{
			m1d1 = 1;
			m1d3 = m1->col;
			m2d2 = m2->col;
			m2d3 = 1;

			row1 = m1->row;
		}
		else
		{
			OSLW_assert(1);
			return NULL;
		}
		break;

	default:
		OSLW_assert(1);
		break;
	}

	_s = s->a;
	_m1 = _m1b = m1->a;
	_m2 = _m2b = m2->a;

	if (flag & 0x04)
	{
		for (i = 0;i<s->row;i++)
		{
			for (j = 0;j<s->col;j++)
			{
				_m1 = _m1b;
				_m2 = _m2b;
				sum = _ParaFrom(0);
				for (k = 0;k<row1;k++)
				{
					sum = _ParaAdd(sum, _ParaMpy(*_m1, *_m2));
					_m1 += m1d3;
					_m2 += m2d3;
				}//行向量*列向量
				*_s = _ParaAdd(*_s, sum);
				_s++;
				_m2b += m2d2;
			}

			_m1b += m1d1;
			_m2b = m2->a;
		}
	}
	else
	{
		for (i = 0;i<s->row;i++)
		{
			for (j = 0;j<s->col;j++)
			{
				_m1 = _m1b;
				_m2 = _m2b;
				sum = _ParaFrom(0);
				for (k = 0;k<row1;k++)
				{
					sum = _ParaAdd(sum, _ParaMpy(*_m1, *_m2));
					_m1 += m1d3;
					_m2 += m2d3;
				}//行向量*列向量
				*_s++ = sum;
				_m2b += m2d2;
			}

			_m1b += m1d1;
			_m2b = m2->a;
		}
	}
#endif

#endif

	return s;
}


OSLW_TOOL_FUN(
	void *,
	OSlwToolMatrixConv2,
	(
		OSlwToolMatrixSTU *s, //目标的
		OSlwToolMatrixSTU *m_kernal, //卷积核
		OSlwToolMatrixSTU *m2,//被卷积 
		lw_u16 move_x, lw_u16 move_y,//横向纵向移动距离
		lw_u8 EqualModel, //赋值模式 1:直接复制 0:相加
		OSlwToolMatrixConvMethodNUM MoveModel, //移动模式 'v'/'f'
		lw_u8 KernalModel, //核模式 0/180 180+‘f’=数学二维卷积
		ParaType *fast_buf//快速卷积内存区
	),
	OSlwToolMatrixConv2
)
{
	lw_u8 model_flag = ((MoveModel == OSlwToolMatrixConvMethod_Valid) << 1) | (KernalModel == 0);
	
	ParaType sum_buf;

	ParaType *s_a;
	ParaType *k_a, *k_p;
	ParaType *m_r, *m_c, *m_p;

	lw_32 sum_jump, i, j, k, l, kern_r, kern_c, out_r, out_c, m_row, m_col;

	OSLW_assert(!(s));
	OSLW_assert(!(m_kernal));
	OSLW_assert(!(m2));
	OSLW_assert(!(move_x));
	OSLW_assert(!(move_y));
	s_a = s->a;
	
	m_r = m2->a;
	sum_jump = m2->col - m_kernal->col;
	out_r = s->row;
	out_c = s->col;
	kern_r = m_kernal->row;
	kern_c = m_kernal->col;
	m_row = m2->row;
	m_col = m2->col;
	if (fast_buf==NULL)
	{
		switch (model_flag)
		{
		//完全数学卷积
		case 0:
			k_a = m_kernal->a + m_kernal->length - 1;
			out_r = m_row + kern_r - 1;
			out_c = m_col + kern_c - 1;
			if (EqualModel)
			{
				for (i = 0; i < out_r; i+=move_x)
				{
					for (j = 0; j < out_c; j+=move_y)
					{
						//求和
						sum_buf = _ParaFint(0);
						k_p = k_a;
						for (k = i- kern_c+1; k <= i; k++)
						{
							l = j - kern_r + 1;
							for (m_c= m_r + k*m_col + l; l <= j; ++l, --k_p, ++m_c)
							{
								if (!(k<0 || k>=m_col || l<0 || l>=m_row))
								{
									sum_buf += _ParaMpy(*k_p, *m_c);
								}				
							}
						}
						*s_a++ = sum_buf;
					}
				}
			}
			else
			{
				for (i = 0; i < out_r; i += move_x)
				{
					for (j = 0; j < out_c; j += move_y)
					{
						//求和
						sum_buf = _ParaFint(0);
						k_p = k_a;
						for (k = i - kern_c + 1; k <= i; k++)
						{
							l = j - kern_r + 1;
							for (m_c = m_r + k*m_col + l; l <= j; ++l, --k_p, ++m_c)
							{
								if (!(k<0 || k >= m_col || l<0 || l >= m_row))
								{
									sum_buf += _ParaMpy(*k_p, *m_c);
								}
							}
						}
						*s_a += sum_buf;
						s_a++;
					}
				}
			}
			break;

		case 1:
			OSLW_assert(1);//先不管
			break;

		case 2:
			OSLW_assert(1);//先不管
			break;

		//CNN 标准模式
		case 3:
			k_a = m_kernal->a;
			move_y *= m2->col;
			if (EqualModel)
			{
				register LwMatLenType div8, mod8;
				div8 = kern_c / CACHE_READ_NUM, mod8 = kern_c % CACHE_READ_NUM;

				for (i = 0,m_r=m2->a; i < out_r; i++,m_r+= move_y)
				{
					for ( j = 0,m_c=m_r; j < out_c; j++,m_c+=move_x)
					{
						//求和
						//sum_buf = _ParaFint(0);
						k_p = k_a;
						m_p = m_c;
						for (k = 0; k < kern_c; k++, m_p += sum_jump)
						{
							//for (l = 0; l < kern_r; l++)
							//	sum_buf += _ParaMpy(*k_p++, *m_p++);
							LwVectDot(sum_buf, k_p, m_p, div8, mod8);
						}
						*s_a++ = sum_buf;
					}
				}
			}
			else
			{
				register LwMatLenType div8, mod8;
				div8 = kern_c / CACHE_READ_NUM, mod8 = kern_c % CACHE_READ_NUM;

				for (i = 0, m_r = m2->a; i < out_r; i++, m_r += move_y)
				{
					for (j = 0, m_c = m_r; j < out_c; j++, m_c += move_x)
					{
						//求和
						k_p = k_a;
						m_p = m_c;
						for (k = 0; k < kern_c; k++, m_p += sum_jump)
						{
							//for (l = 0; l < kern_r; l++)
							//	sum_buf += _ParaMpy(*k_p++, *m_p++);
							LwVectDot(sum_buf, k_p, m_p, div8, mod8);
						}
						*s_a += sum_buf;
						s_a++;
					}
				}
			}
			break;

		default:
			break;
		}
	}
	else
	{
		//考虑到嵌入式系统特性 暂时不进行优化 
		//而且CNN优化算法将卷积变为矩阵相乘 并没有改变运算量 适用于GPU
		OSLW_assert(1);
	}

	return s;

}

//只计算一个batch
OSLW_TOOL_FUN(
	void*, OSlwToolMatrixConvFastMultCh,
	(
		OSlwToolMatrixSTU *m_out, //输出 row-col 代表一个通道 length代表真正大小
		OSlwToolMatrixSTU *m_kernal, //卷积核 row-col 代表一个通道 length 代表一个核真正大小 [2,2,4] row:2 col:2 length:16
		OSlwToolMatrixSTU *m_in,//被卷积 row-col 代表一个通道
		OSlwToolMatrixSTU *bias,//偏置 row-col-length 无所谓
		lw_u16 in_high,//输入高度 
		lw_u16 out_high,//输出高度
		lw_u16 move_x, lw_u16 move_y,//横向纵向移动距离
		OSlwToolMatrixConvMethodNUM conv_method,
		lw_u8 FD_1_or_BK_0,//前向传递或者反向传递
		ParaType *fast_buf//缓冲区
		),
	OSlwToolMatrixConvFastMultCh
)
{

	lw_32 i, j, k, L, n;
	lw_32 out_r, out_c;
	lw_32 sum_jump1, sum_jump2, out_jump, kern_len;
	lw_32 kern_r, kern_c, kern_one_len;
	lw_32 in_r, in_c;

	ParaType temp, sum_buf;
	ParaType *o_a, *o_pic;
	ParaType *k_a, *k_p, *k_p2;
	ParaType *m_r, *m_c, *m_p, *m_px, *m_py;
	ParaType *pbuf, *pbuf2;

	LwMatLenType div8, mod8;

	o_a = m_out->a;
	out_r = m_out->row;
	out_c = m_out->col;
	kern_r = m_kernal->row;
	kern_c = m_kernal->col;
	in_r = m_in->row;
	in_c = m_in->col;
	k_a = m_kernal->a;
	kern_len = m_kernal->length;
	kern_one_len = kern_c*kern_r;
	
	//先加上偏置
	if (bias)
	{
		if (bias->a)
		{
			L = m_out->col*m_out->row;
			div8 = L / CACHE_READ_NUM, mod8 = L % CACHE_READ_NUM;
			for (i = 0; i < out_high; i++)
			{
				temp = bias->a[i];
				LwVectSet(o_a, temp, div8, mod8);
			}
			o_a = m_out->a;
		}
		else
		{
			LW_MAT_CLR(m_out);
		}


	}
	else
	{
		LW_MAT_CLR(m_out);
	}


	if (FD_1_or_BK_0)
	{
		move_y *= m_in->col;
		sum_jump1 = m_in->col;
		sum_jump2 = in_c*in_r;
		out_jump = out_c*out_r;

		div8 = kern_len / CACHE_READ_NUM, mod8 = kern_len % CACHE_READ_NUM;

		for (i = 0, m_r = m_in->a; i < out_r; i++, m_r += move_y)
		{
			for (j = 0, m_c = m_r; j < out_c; j++, m_c += move_x)
			{

				//抠图
				pbuf = fast_buf;//得到缓冲区
				for (k = 0, m_py = m_c; k < in_high; k++,m_py+=sum_jump2)
					for (L = 0, m_px = m_py; L < kern_r; L++, m_px += sum_jump1)
					{
						//考虑到kern_c往往比较小 不进行提取了
						for (n = 0, m_p = m_px; n < kern_c; n++)
							*pbuf++ = *m_p++;
					}
				//求和
				
				for (k = 0, k_p = k_a,o_pic=o_a; k < out_high; k++, o_pic += out_jump)
				{
					//for (sum_buf = _ParaFint(0), pbuf = fast_buf, l = 0; l < kern_len; l++)
					//{
					//	sum_buf+= _ParaMpy(*k_p++, *pbuf++);
					//}
					pbuf = fast_buf;
					LwVectDot(sum_buf, k_p, pbuf, div8, mod8);
					(*o_pic) += sum_buf;
				}

				o_a++;
			}
		}
	}
	else
	{
		sum_jump1 = kern_len + kern_one_len;
		sum_jump2 = out_c*out_r;
		m_r = m_in->a;
		out_jump = in_c*in_r;
		for (i = 0; i < out_r; i += move_x)
		{
			for (j = 0; j < out_c; j += move_y)
			{
				//抠图
				pbuf = fast_buf;//得到缓冲区
				for (k = i - kern_c + 1; k <= i; k++)
				{
					L = j - kern_r + 1;
					for (m_c = m_r + k*in_c + L; L <= j; ++L, ++m_c)
					{
						pbuf2 = pbuf++;						
						if (!(k<0 || k >= in_c || L<0 || L >= in_r))
						{
							//连续几张图片都采样这个点
							for ( n = 0, m_p = m_c; n < in_high; n++, pbuf2+=kern_one_len, m_p += out_jump)
							{
								*pbuf2 = *m_p;
							}
						}
						else
						{
							for (n = 0; n < in_high; n++, pbuf2 += kern_one_len)
							{
								*pbuf2 = _ParaFint(0);
							}
						}
					}
				}

				//求和				
				//循环每一个维度
				for ( k = 0, o_pic=o_a,k_p=k_a; k < out_high; k++,k_p+=kern_one_len)
				{
					//循环每一个通道
					for ( L = 0, pbuf = fast_buf, k_p2 = k_p + kern_one_len - 1, sum_buf = _ParaFint(0); L < in_high; L++, k_p2 += sum_jump1)
					{
						for ( n = 0; n < kern_one_len; n++)
						{
							sum_buf += _ParaMpy(*k_p2--, *pbuf++);
						}
					}

					*o_pic += sum_buf;
					o_pic += sum_jump2;

				}
				o_a++;
			}
		}
	}


	return m_out;
}


//只计算一个batch
OSLW_TOOL_FUN(
	void*, OSlwToolMatrixConvFastMultChIm2ColFd,
	(
		OSlwToolMatrixSTU *m_out, //输出 row-col 代表一个通道 length代表真正大小
		OSlwToolMatrixSTU *m_kernal, //卷积核 row-col 代表一个通道 length 代表一个核真正大小 [2,2,4] row:2 col:2 length:16
		OSlwToolMatrixSTU *m_in,//被卷积 row-col 代表一个通道
		OSlwToolMatrixSTU *bias,//偏置 row-col-length 无所谓
		lw_u16 in_high,//输入高度 
		lw_u16 out_high,//输出高度
		lw_u16 move_x, lw_u16 move_y,//横向纵向移动距离
		OSlwToolMatrixConvMethodNUM conv_method,
		lw_u32 now_flow_len,//现实缓冲区的大小
		ParaType *fast_buf//缓冲区
		),
	OSlwToolMatrixConvFastMultChIm2ColFd
)
{

	lw_32 i, j, k, L, n;
	lw_32 out_r, out_c;
	lw_32 sum_jump1, sum_jump2 /*, out_jump, kern_len*/;
	lw_32 kern_r, kern_c, kern_one_len;
	lw_32 in_r, in_c;

	lw_u32 col_count = 0, col_cmax, im2col_one_len;

	ParaType temp;
	ParaType *o_a;
	ParaType *k_a;
	ParaType *m_r, *m_c, *m_p, *m_px, *m_py;
	ParaType *pbuf;

	LwMatLenType div8, mod8;

	LwMat A, B, C;
	
	o_a = m_out->a;
	out_r = m_out->row;
	out_c = m_out->col;
	kern_r = m_kernal->row;
	kern_c = m_kernal->col;
	in_r = m_in->row;
	in_c = m_in->col;
	k_a = m_kernal->a;
	//kern_len = m_kernal->length;
	kern_one_len = kern_c*kern_r;
	im2col_one_len = (kern_one_len*in_high);
	col_cmax = now_flow_len / im2col_one_len;
	col_cmax = col_cmax >= (lw_u32)((out_c*out_r)) ? (out_c*out_r) : col_cmax;
	OSLW_assert(!(col_cmax));



	//先加上偏置
	if (bias)
	{
		if (bias->a)
		{

			L = m_out->col*m_out->row;
			div8 = L / CACHE_READ_NUM, mod8 = L % CACHE_READ_NUM;
			for (i = 0; i < out_high; i++)
			{
				temp = bias->a[i];
				LwVectSet(o_a, temp, div8, mod8);
			}

			o_a = m_out->a;
		}
		else
		{
			LW_MAT_CLR(m_out);
		}
	}
	else
	{
		LW_MAT_CLR(m_out);
	}

	move_y *= m_in->col;
	sum_jump1 = m_in->col;
	sum_jump2 = in_c*in_r;
	//out_jump = out_c*out_r;
	pbuf = fast_buf;//得到缓冲区

#if !(defined(OSLW_USING_CBLAS) && OSLW_USING_CBLAS)
	LwMatInit(&A, out_high, im2col_one_len, k_a);
	LwMatInit(&B, col_cmax, im2col_one_len, fast_buf);
	LwMatInit(&C, out_high, col_cmax, o_a);
#endif


	for (i = 0, m_r = m_in->a; i < out_r; i++, m_r += move_y)
	{
		for (j = 0, m_c = m_r; j < out_c; j++, m_c += move_x)
		{
			//抠图
			for (k = 0, m_py = m_c; k < in_high; k++, m_py += sum_jump2)
				for (L = 0, m_px = m_py; L < kern_r; L++, m_px += sum_jump1)
					for (n = 0, m_p = m_px; n < kern_c; n++)
						*pbuf++ = *m_p++;

			++col_count;
			if (col_count==col_cmax)
			{
				col_count = 0;
				pbuf = fast_buf;
#if defined(OSLW_USING_CBLAS) && OSLW_USING_CBLAS
				//计算矩阵乘法
				cblas_sgemm(
					CblasRowMajor,
					CblasNoTrans, CblasTrans,
					out_high, col_cmax, im2col_one_len,
					_ParaFint(1), k_a, im2col_one_len,
					pbuf, im2col_one_len,
					_ParaFint(1), o_a, out_c*out_r//out_jump
				);

#else
				C.a = o_a;
				B.a = pbuf;
				LwMatTurnMpy(&C, &A, &B, 0x11);

#endif
				o_a += col_cmax;
			}
		}
	}

	//处理残余项
	if (col_count)
	{
		pbuf = fast_buf;	
#if defined(OSLW_USING_CBLAS) && OSLW_USING_CBLAS
		//计算矩阵乘法
		cblas_sgemm(
			CblasRowMajor,
			CblasNoTrans, CblasTrans,
			out_high, col_count, im2col_one_len,
			_ParaFint(1), k_a, im2col_one_len,
			pbuf, im2col_one_len,
			_ParaFint(1), o_a, out_c*out_r//out_jump
		);
#else
		//自定义的矩阵乘法
		LwMatInit(&A, out_high, im2col_one_len, k_a);
		LwMatInit(&B, col_count, im2col_one_len, pbuf);
		LwMatInit(&C, out_high, col_count, o_a);

		LwMatTurnMpy(&C, &A, &B, 0x11);
#endif
		//o_a += col_cmax;
	}


	return m_out;
}





OSLW_TOOL_FUN(void*, OSlwToolMatrixMoments,
(
	OSlwToolMatrixSTU *src,
	ParaType *mean,
	ParaType *var,
	lw_u8 dim
	), 
	OSlwToolMatrixMoments)
{

	register ParaType _sum1, _sum2;
	register ParaType *p, *pbase;
	register ParaType _div_m;
	register lw_u32 i, j;
	register lw_u32 r, c;
	LwMatLenType t_div, t_mod;
	OSLW_assert(!(src));
	OSLW_assert(!(mean));
	OSLW_assert(!(var));

	r = src->row;
	c = src->col;
	
	t_div = c / CACHE_READ_NUM, t_mod = c%CACHE_READ_NUM;

	if (dim==0x01)
	{
		pbase = src->a;
		_div_m = _ParaDiv(_ParaFrom(1), _ParaFint(c));
		for ( i = 0; i < r; ++i)
		{
			p = pbase;
			
			//求和			
			//for (j = 0, _sum1 = _ParaFrom(0); j < c; ++j, ++p)
			//{
			//	_sum1 = _ParaAdd(_sum1, *p);
			//}
			LwVectSum(_sum1, p, t_div, t_mod);



			//求平均
			_sum1 = _ParaMpy(_sum1, _div_m);
			//保存均值
			*mean++ = _sum1;

			//方差
			for (j = 0, _sum2 = _ParaFrom(0); j < c; ++j, ++pbase)
			{
				_sum2 = _ParaAdd(
					_sum2,
					_ParaMpy(_ParaSub(*pbase, _sum1), _ParaSub(*pbase, _sum1))
				);
			}
			//求方差
			_sum2 = _ParaMpy(_sum2, _div_m);
			//保存方差
			*var++ = _sum2;
		}

	}
	else
	{
		OSLW_assert(1);
	}



	return src;
}


OSLW_TOOL_FUN(void*, OSlwToolMatrixVectShift,
(
	OSlwToolMatrixSTU *y,
	OSlwToolMatrixSTU *we,
	OSlwToolMatrixSTU *x,
	OSlwToolMatrixSTU *bi
	),
	OSlwToolMatrixVectShift)
{
	LwMatRowType i, _r;
	LwMatColType j, _c;
	ParaType *ya, *xa, *wa, *ba, *wa_b, *ba_b;
	ParaType temp_w, temp_b;
	OSLW_assert(!(y));
	OSLW_assert(!(we));
	OSLW_assert(!(x));

	if (bi)
	{
		ya = y->a;
		xa = x->a;
		wa_b = we->a;
		ba_b = bi->a;
		if (we->length==1)
		{
			temp_w = *wa_b;
			temp_b = *ba_b;
			for (i = 0, _r = y->row, _c = y->col; i < _r; i++)
			{
				for (j = 0; j < _c; ++j, ++ya, ++xa)
				{
					*ya = _ParaAdd(_ParaMpy(temp_w, *xa), temp_b);
				}
			}
		}
		else
		{
			for (i = 0, _r = y->row, _c = y->col; i < _r; i++)
			{
				for (j = 0, wa = wa_b, ba = ba_b; j < _c; ++j, ++ya, ++wa, ++xa, ++ba)
				{
					*ya = _ParaAdd(_ParaMpy(*wa, *xa), *ba);
				}
			}
		}
	}
	else
	{
		ya = y->a;
		xa = x->a;
		wa_b = we->a;
		ba_b = bi->a;
		if (we->length == 1)
		{
			temp_w = *wa_b;
			temp_b = *ba_b;
			for (i = 0, _r = y->row, _c = y->col; i < _r; i++)
			{
				for (j = 0; j < _c; ++j, ++ya, ++xa)
				{
					*ya = _ParaMpy(temp_w, *xa);
				}
			}
		}
		else
		{
			for (i = 0, _r = y->row, _c = y->col; i < _r; i++)
			{
				for (j = 0, wa = wa_b; j < _c; ++j, ++ya, ++wa, ++xa)
				{
					*ya = _ParaMpy(*wa, *xa);
				}
			}
		}
	}

	return y;

}


OSLW_TOOL_FUN(ParaType, OSlwToolMatrixDotSum,
(
	OSlwToolMatrixSTU *y,
	OSlwToolMatrixSTU *x1,
	OSlwToolMatrixSTU *x2,
	lw_u8 dim
	),
	OSlwToolMatrixDotSum)
{

	LwMatRowType i, _r;
	//LwMatColType _c;
	//LwMatLenType L1, _L;
	LwMatLenType t_div, t_mod;
	ParaType _sum = 0;
	ParaType *ya, *ya_b;
	ParaType *x1a;
	ParaType *x2a;

	
	OSLW_assert(!(x1));
	OSLW_assert(!(x2));


	switch (dim)
	{

	case 0x00://当做向量点积
		x1a = x1->a;
		x2a = x2->a;
		t_div = x1->length / CACHE_READ_NUM, t_mod = x1->length%CACHE_READ_NUM;

		//_sum = _ParaFint(0);
		//for (L1 = 0,_L=x1->length; L1 < _L; ++L1,++x1a,++x2a)
		//{
		//	_sum = _ParaAdd(_sum, (_ParaMpy(*x1a, *x2a)));
		//}

		LwVectDot(_sum, x1a, x2a, t_div, t_mod);
		if (y!=NULL)
			y->a[0] = _sum;
		
		break;

	case 0x10://当做向量点积 并加到y上面
		OSLW_assert(!(y));
		x1a = x1->a;
		x2a = x2->a;
		t_div = x1->length / CACHE_READ_NUM, t_mod = x1->length%CACHE_READ_NUM;

		//_sum = _ParaFint(0);
		//for (L1 = 0,_L=x1->length; L1 < _L; ++L1,++x1a,++x2a)
		//{
		//	_sum = _ParaAdd(_sum, (_ParaMpy(*x1a, *x2a)));
		//}

		LwVectDot(_sum, x1a, x2a, t_div, t_mod);
		y->a[0] = _ParaAdd(y->a[0], _sum);
		break;

	case 0x01://按照每一行求点积
		OSLW_assert(!(y));
		x1a = x1->a;
		x2a = x2->a;
		ya = y->a;
		t_div = x1->col / CACHE_READ_NUM, t_mod = x1->col%CACHE_READ_NUM;
		for (i = 0, _r = x1->row; i < _r; ++i, ++ya)
		{
			//for (j = 0, _sum = _ParaFint(0); j < _c; ++j, ++x1a, ++x2a)
			//	_sum = _ParaAdd(_sum, (_ParaMpy(*x1a, *x2a)));
			LwVectDot(_sum, x1a, x2a, t_div, t_mod);

			*ya = _sum;
		}
		break;

	case 0x11://按照每一行求点积 并把结果加上去
		OSLW_assert(!(y));
		x1a = x1->a;
		x2a = x2->a;
		ya = y->a;
		t_div = x1->col / CACHE_READ_NUM, t_mod = x1->col%CACHE_READ_NUM;
		for (i = 0, _r = x1->row; i < _r; ++i, ++ya)
		{
			//for (j = 0, _sum = _ParaFint(0); j < _c; ++j, ++x1a, ++x2a)
			//	_sum = _ParaAdd(_sum, (_ParaMpy(*x1a, *x2a)));
			LwVectDot(_sum, x1a, x2a, t_div, t_mod);

			*ya = _ParaAdd(*ya, _sum);
		}
		break;

	case 0x02://按照每一列求点积
		LW_MAT_CLR(y);
	case 0x12://把结果加上去
		OSLW_assert(!(y));
		x1a = x1->a;
		x2a = x2->a;
		ya_b = y->a;
		t_div = x1->col / CACHE_READ_NUM, t_mod = x1->col%CACHE_READ_NUM;
		for (i = 0, _r = x1->row; i < _r; ++i, ++ya)
		{
			//for (j = 0, ya = ya_b; j < _c; ++j, ++x1a, ++x2a, ++ya)
			//	*ya = _ParaAdd(*ya, (_ParaMpy(*x1a, *x2a)));
			ya = ya_b;
			LwVectDotSum(ya, x1a, x2a, t_div, t_mod);		
		}
		break;

	default:
		OSLW_assert(1);
		break;
	}

	return _sum;

}



OSLW_TOOL_FUN(OSlwToolMatrixSTU*, OSlwToolMatrix_RATIO_ADD,
(OSlwToolMatrixSTU *s,ParaType a, OSlwToolMatrixSTU *m1, ParaType b, OSlwToolMatrixSTU *m2),
OSlwToolMatrix_RATIO_ADD
)
{
	register lw_u32 i;
	register ParaType *_sa, *_m1a, *_m2a;
	OSLW_assert(!(s));
	OSLW_assert(!(m1));
	OSLW_assert(!(m2));

	if (s->length == m1->length && s->length == m2->length)//满足相乘条件
	{
		i = s->length;
		_sa = s->a;
		_m1a = m1->a;
		_m2a = m2->a;
		while (i--)
		{
			*_sa++ = _ParaAdd(_ParaMpy(a, *_m1a), _ParaMpy(b, *_m2a));
			_m1a++;
			_m2a++;
		}
		
	}
	else
	{
		return NULL;
	}


	return s;
}


#if OSLW_GLOBAL_MATH_TYPE==OSLW_GLOBAL_MATH_FLOAT || OSLW_GLOBAL_MATH_TYPE==OSLW_GLOBAL_MATH_DOUBLE
#define LW_SVD_MAX(A,B) ((A)>(B)?(A):(B))
#define LW_SVD_MIN(A,B) ((A)<(B)?(A):(B))

#if OSLW_GLOBAL_MATH_TYPE == OSLW_GLOBAL_MATH_FLOAT
#define LW_CHYPOT(X,Y,Z) do{\
	(Z)=sqrtf((X)*(X)+(Y)*(Y));\
}while(0)
#else
#define LW_CHYPOT(X,Y,Z) do{\
	(Z)=sqrt((X)*(X)+(Y)*(Y));\
}while(0)
#endif

inline int LW_svd_IsFinite(ParaType x)
{
#if OSLW_GLOBAL_MATH_TYPE == OSLW_GLOBAL_MATH_FLOAT
	int32_t *hx;
	hx = (void *)(&x);
	return (int32_t)((uint32_t)(((*hx) & 0x7fffffff) - 0x7f800000) >> 31 != 0);
#else
	return isfinite(x);
#endif
}

inline static void LW_svd_rotg(ParaType *x, ParaType *y, ParaType *c, ParaType *s)
{
	ParaType rho, r, z, absx, absy;

	rho = ((absx = _ParaAbs(*x)) > (absy = _ParaAbs(*y))) ? *x : *y;
	LW_CHYPOT(*x, *y, r);
	r = (rho > 0.0F) ? r : -r;
	*c = (r == 0.0F) ? 1.0F : *x / r;
	*s = (r == 0.0F) ? 0.0F : *y / r;
	z = (absx > absy) ? *s : 1.0F;
	z = (absy >= absx && *c != 0.0F) ? 1.0F / *c : z;
	*x = r;
	*y = z;
}


inline static void LW_rot_real(
	int	n,	
	ParaType	c,	
	ParaType	s, 	
	ParaType	*x,	
	ParaType	*y
)
{
	ParaType t;
	if (n <= 0) {
		return;
	}

	while (n--) {
		t = c * *x + s * *y;
		*y = c * *y - s * *x;
		*x++ = t;
		y++;
	}
}

OSLW_TOOL_FUN(lw_16, OSlwToolMatrix_SVD,
(
	OSlwMat *U,
	OSlwMat *S,
	OSlwMat *V,
	OSlwMat *xin,
	ParaType *temp_run
),
OSlwToolMatrix_SVD
)
{
	OSLW_assert(!(xin));
	OSLW_assert(!(U));
	OSLW_assert(!(S));
	OSLW_assert(!(temp_run));

	ParaType *x = xin->a;
	lw_32 n = (lw_32)(xin->row);
	lw_32 p = (lw_32)(xin->col);
	ParaType *e = temp_run;
	ParaType *s = S->a;
	ParaType *work_temp = temp_run + xin->col;
	ParaType *u = U->a;
	ParaType *v;
	lw_u16 get_v_flag;

	lw_32 nm1, np1;
	lw_32 i_iter, k_case, j, k, kp1;
	lw_32 l, lp1, lm1, ls, lu;
	lw_32 m, mm, mm1, mm2;
	lw_32 info = 0;
	lw_32 nct, ncu, nrt, nml;
	lw_32 ii;
	lw_u32 pll, plj, pil;
	ParaType t, t2, r;
	ParaType sm, smm1, emm1, el, sl, b, c, cs, sn, scale, t1, f;
	ParaType test, ztest, snorm, g, shift;
	ParaType *pxll, *pxlj, *pel, *pel1, *psl, *pull, *pvll;
	ParaType *tp1, *tp2, temp, temp1;

	lw_32 nml_div, nml_mod;
	lw_32 p_lp1_div, p_lp1_mod;
	lw_32 n_lp1_div, n_lp1_mod;

	lw_32 n_div, n_mod;
	lw_32 p_div, p_mod;

	n_div = n / CACHE_READ_NUM, n_mod = n%CACHE_READ_NUM;
	p_div = p / CACHE_READ_NUM, p_mod = p%CACHE_READ_NUM;

#if OSLW_GLOBAL_MATH_TYPE == OSLW_GLOBAL_MATH_FLOAT
	const ParaType one_const = 1.0F, zero_const = 0.0F;
	const ParaType eps_const = FLT_EPSILON;
	const ParaType  tiny_const = FLT_MIN / FLT_EPSILON;
#else
	const ParaType one_const = 1.0, zero_const = 0.0;
	const ParaType eps_const = DBL_EPSILON;
	const ParaType  tiny_const = DBL_MIN / DBL_EPSILON;
#endif


	if (V==NULL)
	{
		get_v_flag = 0;
	}
	else
	{
		get_v_flag = 1;
		v = V->a;
	}
	

	ncu = LW_SVD_MIN(n, p);
	np1 = n + 1;
	nm1 = n - 1;
	nct = LW_SVD_MIN(nm1, p);
	nrt = LW_SVD_MAX(0, LW_SVD_MIN(p - 2, n));
	lu = LW_SVD_MAX(nct, nrt);
	for (l = 0; l<lu; l++) {
		nml = n - l;
		lp1 = l + 1;
		pll = l * np1;
		pxll = x + pll;		
		psl = s + l;		
		pel = e + l;	
		pel1 = pel + 1;	
		nml_div = nml / CACHE_READ_NUM, nml_mod = nml%CACHE_READ_NUM;
		if (l < nct) {
			ParaType tttemp1;
			*psl = zero_const;
			tp1 = pxll;
			LwVectNorm2(tttemp1, tp1, nml_div, nml_mod);
			*psl = tttemp1;
			if (_ParaAbs(*psl) != zero_const) {
				if (_ParaAbs(*pxll) != zero_const) {

					*psl = (*pxll >= zero_const) ? _ParaAbs(*psl) : -_ParaAbs(*psl);

				}

				temp1 = one_const / *psl;
				tp1 = pxll;
				LwVectScale(tp1, temp1, nml_div, nml_mod);

				*pxll += one_const;
			}
			*psl = -*psl;
		}
		for (j = lp1; j<p; j++) {
			plj = j * n + l;
			pxlj = x + plj;	
			if (l < nct && _ParaAbs(*psl) != zero_const) {

				tp1 = pxll;
				tp2 = pxlj;
				LwVectDot(t, tp1, tp2, nml_div, nml_mod);
				t = -t;

				t /= *pxll;


				tp1 = pxlj;
				tp2 = pxll;
				LwVectAddKX(tp1, t, tp2, nml_div, nml_mod);


			}

			*(e + j) = *pxlj;
		}

		if (get_v_flag && l < nct) {
			tp1 = u + pll;
			tp2 = pxll;
			LwVectCopy(tp1, tp2, nml_div, nml_mod);
		}
		if (l < nrt) {
			ParaType tttemp1;
			p_lp1_div = (p - lp1) / CACHE_READ_NUM, p_lp1_mod = (p - lp1) % CACHE_READ_NUM;

			*pel = zero_const;
			tp1 = pel1;
			LwVectNorm2(tttemp1, tp1, p_lp1_div, p_lp1_mod);
			*pel = tttemp1;


			if (_ParaAbs(*pel) != zero_const) {
				if (_ParaAbs(*pel1) != zero_const) {

					*pel = (*pel1 >= zero_const) ? _ParaAbs(*pel) : -_ParaAbs(*pel);

				}

				temp1 = one_const / *pel;
				tp1 = pel1;
				LwVectScale(tp1, temp1, p_lp1_div, p_lp1_mod);

				*(pel + 1) += one_const;
			}
			*pel = -*pel;
			if (lp1 < n && _ParaAbs(*pel) != zero_const) {
				n_lp1_div = (n - lp1) / CACHE_READ_NUM, n_lp1_mod = (n - lp1) % CACHE_READ_NUM;
				tp1 = work_temp + lp1;

				LwVectSet(tp1, zero_const, n_lp1_div, n_lp1_mod);

				for (j = lp1; j<p; j++) {
					plj = j*n + lp1;		

					tp1 = work_temp + lp1;
					tp2 = x + plj;
					LwVectAddKX(tp1, *(e + j), tp2, n_lp1_div, n_lp1_mod);


				}
				for (j = lp1; j<p; j++) {
					t2 = -*(e + j);
					t = t2 / *pel1;
					plj = j*n + lp1;		

					tp1 = x + plj;
					tp2 = work_temp + lp1;
					LwVectAddKX(tp1, t, tp2, n_lp1_div, n_lp1_mod);

				}
			}
			if (get_v_flag) {
				pll = lp1 + l * p;  
				tp1 = pel1;
				tp2 = v + pll;
				for (ii = 0; ii<p - lp1; ii++) {
					*tp2++ = *tp1++;
				}
			}
		}
	}

	mm1 = m = LW_SVD_MIN(p, np1);
	mm1--;
	if (nct < p) {
		pil = nct * np1;	
		*(s + nct) = *(x + pil);
	}
	if (n < m) {
		*(s + mm1) = zero_const;
	}
	if (nrt < mm1) {
		pil = nrt + n * mm1;	
		*(e + nrt) = *(x + pil);
	}
	*(e + mm1) = zero_const;

	if (get_v_flag) {
		for (j = nct; j<ncu; j++) {
			tp1 = u + j*n;
			for (ii = 0; ii<n; ii++) {
				*tp1++ = zero_const;
			}
			*(u + j*np1) = one_const;
		}
		for (l = nct - 1; l >= 0; l--) {
			nml = n - l;
			pll = l * np1;
			pull = u + pll;	
			nml_div = nml / CACHE_READ_NUM, nml_mod = nml%CACHE_READ_NUM;
			if (_ParaAbs(*(s + l)) != zero_const) {
				lp1 = l + 1;
				for (j = lp1; j<ncu; j++) {
					plj = j*n + l;	

					tp1 = pull;
					tp2 = u + plj;
					LwVectDot(t, tp1, tp2, nml_div, nml_mod);
					t = -t;
					t /= *pull;
					tp1 = u + plj;
					tp2 = pull;
					LwVectAddKX(tp1, t, tp2, nml_div, nml_mod);
				}

				tp1 = pull;
				LwVectScale(tp1, _ParaFrom(-1), nml_div, nml_mod);


				*pull += one_const;
				if (l >= 1) {
					tp1 = pull - l;
					for (ii = 0; ii<l; ii++) {
						*tp1++ = zero_const;
					}
				}
			}
			else {
				tp1 = pull - l;
				for (ii = 0; ii<n; ii++) {
					*tp1++ = zero_const;
				}
				*pull = one_const;
			}
		}
	}
	if (get_v_flag) {
		for (l = p - 1; l >= 0; l--) {
			lp1 = l + 1;
			pll = l*p + lp1;
			pvll = v + pll;	
			p_lp1_div = (p - lp1) / CACHE_READ_NUM, p_lp1_mod = (p - lp1) % CACHE_READ_NUM;
			if (l < nrt && _ParaAbs(*(e + l)) != zero_const) {
				for (j = lp1; j<p; j++) {
					plj = j*p + lp1;	

					tp1 = pvll;
					tp2 = v + plj;
					LwVectDot(t, tp1, tp2, p_lp1_div, p_lp1_mod);
					t = -t;

					t /= *pvll;

					tp1 = v + plj;
					tp2 = pvll;
					LwVectAddKX(tp1, t, tp2, p_lp1_div, p_lp1_mod);
				}
			}
			tp1 = pvll - lp1;
			for (ii = 0; ii<p; ii++) {
				*tp1++ = zero_const;
			}
			*(pvll - 1) = one_const;		
		}
	}


	for (l = 0; l<m; l++) {
		lp1 = l + 1;
		psl = s + l;		
		pel = e + l;		
		t = _ParaAbs(*psl);
		if (t != zero_const) {
			r = *psl / t;
			*psl = t;
			if (lp1 < m) {
				*pel /= r;
			}
			if (get_v_flag && l < n) {

				tp1 = u + l*n;
				LwVectScale(tp1, r, n_div, n_mod);

			}
		}
		if (lp1 == m) break;		
		t = _ParaAbs(*pel);
		if (t != zero_const) {
			temp = t;
			r = temp / *pel;
			*pel = t;
			psl++;		
			*psl = *psl * r;
			if (get_v_flag) {
				tp1 = v + p*lp1;
				LwVectScale(tp1, r, p_div, p_mod);
			}
		}
	}

	mm = m;
	i_iter = 0;
	snorm = zero_const;
	for (l = 0; l<m; l++) {
		snorm = LW_SVD_MAX(snorm, LW_SVD_MAX(_ParaAbs(*(s + l)), _ParaAbs(*(e + l))));
	}

	while (m != 0 && i_iter <= 20) 
	{

		mm1 = m - 1;
		mm2 = m - 2;
		for (l = mm2; l >= 0; l--) {
			test = _ParaAbs(*(s + l)) + _ParaAbs(*(s + l + 1));
			ztest = _ParaAbs(*(e + l));
			if (!LW_svd_IsFinite(test) || !LW_svd_IsFinite(ztest)) 
			{
				info = -1;
				return(info);
			}
			if ((ztest <= eps_const*test) || (ztest <= tiny_const) ||(i_iter > 20 && ztest <= eps_const*snorm))
			{
				*(e + l) = zero_const;
				break;		
			}
		}
		if (l == mm2) 
		{
			k_case = 4;
		}
		else 
		{
			lp1 = l + 1;
			for (ls = m; ls>lp1; ls--)
			{
				test = zero_const;
				if (ls != m) 
					test += _ParaAbs(*(e + ls - 1));
				if (ls != l + 2) 
					test += _ParaAbs(*(e + ls - 2));
				ztest = _ParaAbs(*(s + ls - 1));
				if (!LW_svd_IsFinite(test) || !LW_svd_IsFinite(ztest))
				{
					return(info);
				}
				if ((ztest <= eps_const*test) || (ztest <= tiny_const)) 
				{
					*(s + ls - 1) = zero_const;
					break;			
				}
			}
			if (ls == lp1) 
			{
				k_case = 3;
			}
			else if (ls == m) 
			{
				k_case = 1;
			}
			else 
			{
				k_case = 2;
				l = ls - 1;
			}
		}
		lm1 = ++l - 1;

		switch (k_case) 
		{

		case 1:		
			f = *(e + mm2);
			*(e + mm2) = zero_const;
			for (k = mm2; k >= l; k--) 
			{
				t1 = *(s + k);
				LW_svd_rotg(&t1, &f, &cs, &sn);
				*(s + k) = t1;
				if (k != l) 
				{
					f = -sn * *(e + k - 1);
					*(e + k - 1) *= cs;
				}
				if (get_v_flag)
				{
					LW_rot_real(p, cs, sn, v + k*p, v + mm1*p);
				}
			}
			break;

		case 2:		
			f = *(e + lm1);
			*(e + lm1) = zero_const;
			for (k = l; k<m; k++) 
			{
				t1 = *(s + k);
				LW_svd_rotg(&t1, &f, &cs, &sn);
				*(s + k) = t1;
				f = -sn * *(e + k);
				*(e + k) *= cs;
				if (get_v_flag) 
				{
					LW_rot_real(n, cs, sn, u + n*k, u + n*lm1);
				}
			}
			break;

		case 3:				
			scale = LW_SVD_MAX(_ParaAbs(*(s + mm1)), LW_SVD_MAX(_ParaAbs(*(s + mm2)), _ParaAbs(*(e + mm2))));
			scale = LW_SVD_MAX(_ParaAbs(scale), LW_SVD_MAX(_ParaAbs(*(s + l)), _ParaAbs(*(e + l))));
			sm = *(s + mm1) / scale;
			smm1 = *(s + mm2) / scale;
			emm1 = *(e + mm2) / scale;
			sl = *(s + l) / scale;
			el = *(e + l) / scale;
			b = ((smm1 + sm) * (smm1 - sm) + emm1 * emm1) / 2.0F;
			c = sm * emm1;
			c *= c;
			shift = zero_const;
			if (b != zero_const || c != zero_const) 
			{
				shift = _ParaSqrt(b * b + c);
				if (b < zero_const) shift = -shift;
				shift = c / (b + shift);
			}
			f = (sl + sm) * (sl - sm) + shift;
			g = sl * el;

			for (k = l; k<mm1; k++) 
			{
				kp1 = k + 1;
				LW_svd_rotg(&f, &g, &cs, &sn);
				if (k != l) *(e + k - 1) = f;
				f = cs * *(s + k) + sn * *(e + k);
				*(e + k) = cs * *(e + k) - sn * *(s + k);
				g = sn * *(s + kp1);
				*(s + kp1) *= cs;
				if (get_v_flag) 
				{
					LW_rot_real(p, cs, sn, v + k*p, v + kp1*p);
				}
				LW_svd_rotg(&f, &g, &cs, &sn);
				*(s + k) = f;
				f = cs * *(e + k) + sn * *(s + kp1);
				*(s + kp1) = -sn * *(e + k) + cs * *(s + kp1);
				g = sn * *(e + kp1);
				*(e + kp1) *= cs;
				if (get_v_flag && k < nm1)
				{
					LW_rot_real(n, cs, sn, u + n*k, u + n*kp1);
				}
			}
			*(e + mm2) = f;
			++i_iter;
			break;

		case 4: 		
			if (*(s + l) < zero_const) 
			{
				*(s + l) = -*(s + l);
				if (get_v_flag) 
				{

					tp1 = v + l*p;
					for (ii = 0; ii<p; ii++) 
					{
						*tp1 = -(*tp1);
						tp1++;
					}

				}
			}
			while (l != mm - 1 && *(s + l) < *(s + l + 1)) {
				lp1 = l + 1;
				t = *(s + l);
				*(s + l) = *(s + lp1);
				*(s + lp1) = t;
				if (get_v_flag && lp1 < p)
				{

					tp1 = v + l*p;
					tp2 = v + lp1*p;
					LwVectSwap(tp1, tp2, p_lp1_div, p_lp1_mod);

				}
				if (get_v_flag && lp1 < n)
				{

					tp1 = u + l*n;
					tp2 = u + lp1*n;
					LwVectSwap(tp1, tp2, n_lp1_div, n_lp1_mod);

				}
				++l;
			}
			i_iter = 0;
			m--;
			break;

		default:
			break;
		}
		info = m;
	}

	return(info);
}

#endif//END SVD


//对扁平矩阵或者方阵化上三角
static OSlwMatSolveStatusNUM _OSlwToolMatrixRowOptUpTrig(ParaType *a,LwMatRowType row,LwMatColType col)
{
	LwMatLenType i, j;
	LwMatLenType temp_i, res_col, res_row, tdiv, tmod;
	ParaType *pa, *pb, *p1, *p2;
	ParaType temp1, temp2;

	//暂时只对扁平的与正方形的矩阵进行处理
	if (row>col)
	{
		return OSlwMatSolveShapeError;
	}

	for (i = 0; i < row - 1; i++)
	{
		//非零的列数 没有处理的行数
		res_col = col - i;
		res_row = row - i;
		tdiv = res_col / CACHE_READ_NUM, tmod = res_col%CACHE_READ_NUM;
		p1 = a + i*col + i;

		//查找主元素 就是一列中最大的数字 间隔为列数 长度为行数-当前行
		temp_i = LwVectAmax(p1, res_row, col);
		j = i + temp_i;//temp_i是相对偏移 得到绝对行数
		p2 = a + j*col + i;

		//主元素为0 代表不满秩 返回错误
		if (*p2 == _ParaFrom(0))
			return OSlwMatSolveZeroError;
		
		temp1 = _ParaDiv(_ParaFrom(-1), *p2);

		//当前行不是主行 交换行
		if (temp_i != 0)
		{		
			pa = p1, pb = p2;
			LwVectSwap(pa, pb, tdiv, tmod);
		}

		//将下方的元素消去 高斯消元
		pb = p1 + col - i;
		for (j = 0; j < res_row-1; j++)
		{
			pa = p1;
			pb += i;
			temp2 = _ParaMpy(*pb, temp1);
			LwVectAddKX(pb, temp2, pa, tdiv, tmod);
		}
		
	}

	return OSlwMatSolveSucces;

}

//对扁平矩阵或者方阵的上三角矩阵按照主对角归一化 对_OSlwToolMatrixRowOptUpTrig结果处理
static OSlwMatSolveStatusNUM _OSlwToolMatrixUpTrigNormal(ParaType *a, LwMatRowType row, LwMatColType col)
{
	LwMatLenType i;
	LwMatLenType  res_col, tdiv, tmod;
	ParaType *pa;
	ParaType temp1;
	//暂时只对扁平的与正方形的矩阵进行处理
	if (row>col)
	{
		return OSlwMatSolveShapeError;
	}

	for (i = 0; i < row; i++)
	{
		res_col = col - i;
		tdiv = res_col / CACHE_READ_NUM, tmod = res_col%CACHE_READ_NUM;
		pa = a + i*col + i;
		temp1 = _ParaDiv(_ParaFrom(1), *pa);
		LwVectScale(pa, temp1, tdiv, tmod);
	}

	return OSlwMatSolveSucces;
}

//对扁平矩阵或者方阵的主对角归一化的上三角矩阵转化为单位阵 对_OSlwToolMatrixUpTrigNormal结果处理
static OSlwMatSolveStatusNUM _OSlwToolMatrixUpTrigToElem(ParaType *a, LwMatRowType row, LwMatColType col)
{
	LwMatLenType i, j;
	LwMatLenType  res_col, res_row, tdiv, tmod;
	ParaType *pa, *pb, *p1;
	ParaType temp1;

	//暂时只对扁平的与正方形的矩阵进行处理
	if (row>col)
	{
		return OSlwMatSolveShapeError;
	}

	for (i = row - 1; i > 0; i--)
	{
		//非零的列数 没有处理的行数
		res_col = col - i;
		res_row = i;

		p1 = a + i*col + i;				
		for (j = 0; j < res_row; j++)
		{
			pa = p1;
			pb = a + j*col + i;
			tdiv = res_col / CACHE_READ_NUM, tmod = res_col%CACHE_READ_NUM;
			temp1 = _ParaMpy(_ParaFrom(-1), *pb);
			LwVectAddKX(pb, temp1, pa, tdiv, tmod);
			//*pb = _ParaAdd(*pb, _ParaMpy(temp1, *pa));

		}
	}

	return OSlwMatSolveSucces;
}


OSLW_TOOL_FUN(OSlwMatSolveStatusNUM, OSlwToolMatrixSolve,
(OSlwToolMatrixSTU *s),
OSlwToolMatrixSolve
)
{
	OSlwMatSolveStatusNUM status;
	ParaType *a;
	LwMatRowType r;
	LwMatColType c;

	OSLW_assert(!(s));

	a = s->a, r = s->row, c = s->col;
	status = _OSlwToolMatrixRowOptUpTrig(a, r, c);
	if (status!=OSlwMatSolveSucces){
		return status;
	}

	status = _OSlwToolMatrixUpTrigNormal(a, r, c);
	if (status != OSlwMatSolveSucces){
		return status;
	}

	status = _OSlwToolMatrixUpTrigToElem(a, r, c);
	if (status != OSlwMatSolveSucces) {
		return status;
	}

	return OSlwMatSolveSucces;
}

OSLW_TOOL_FUN(OSlwMatSolveStatusNUM, OSlwToolMatrixInv,
(OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *m1,ParaType *temp_work),
OSlwToolMatrixInv
)
{
	OSlwMatSolveStatusNUM status;
	LwMatRowType i, r;
	LwMatColType c1;
	LwMatLenType tdiv, tmod;
	ParaType *pt, *pma;
	LwMat TempRun;
	
	OSLW_assert(!(s));
	OSLW_assert(!(m1));
	OSLW_assert(!(temp_work));

	r = m1->row;
	c1 = m1->col;

	if (r!=c1)
	{
		return OSlwMatSolveShapeError;
	}

	tdiv = c1 / CACHE_READ_NUM, tmod = c1%CACHE_READ_NUM;
	pt = temp_work;
	pma = m1->a;
	LwMatInit(&TempRun, r, 2 * c1, temp_work);

	for (i = 0; i < r; i++)
	{
		//先复制m1
		LwVectCopy(pt, pma, tdiv, tmod);

		//清空
		LwVectSet(pt, _ParaFrom(0), tdiv, tmod);

		//放一个1
		*OSlwMatGet(TempRun, i, i + r) = _ParaFrom(1);

	}
	
	status = LwMatSolve(&TempRun);
	if (status != OSlwMatSolveSucces) {
		return status;
	}

	pma = s->a;

	for (i = 0; i < r; i++)
	{
		//定位
		pt = OSlwMatGet(TempRun, i, r);
		//copy出来
		LwVectCopy(pma, pt, tdiv, tmod);
	}

	return OSlwMatSolveSucces;
}


OSLW_TOOL_FUN(OSlwMatSolveStatusNUM, OSlwToolMatrixSolveCrout,
(OSlwToolMatrixSTU *X, OSlwToolMatrixSTU *L, OSlwToolMatrixSTU *D, OSlwToolMatrixSTU *U, OSlwToolMatrixSTU *Y),
OSlwToolMatrixSolveCrout
)
{
	lw_32 i;
	LwMatLenType len;
	ParaType *a, *b, *c, *d, *x;

	OSLW_assert(!(X));
	OSLW_assert(!(L));
	OSLW_assert(!(D));
	OSLW_assert(!(U));
	OSLW_assert(!(Y));

	len = X->length;
	x = X->a;
	a = D->a;
	b = Y->a;
	c = U->a;
	d = L->a;

	if (!((len-1==L->length) && (D->length==len) && (U->length == len-1) && (Y->length ==len))){
		return OSlwMatSolveShapeError;
	}
	for (i = 0; i < ((lw_32)len)-1; i++)
	{
		if (a[i] == _ParaFrom(0))
			return OSlwMatSolveZeroError;

		c[i] = _ParaDiv(c[i], a[i]);
		a[i + 1] = a[i + 1] - _ParaMpy(d[i], c[i]);
	}
	
	if (a[i] == _ParaFrom(0))
		return OSlwMatSolveZeroError;

	//求解y
	b[0] = _ParaDiv(b[0], a[0]);
	for (i = 1; i < (lw_32)len; i++){
		b[i] = _ParaDiv(b[i] - _ParaMpy(d[i - 1], b[i - 1]), a[i]);
	}

	x[len - 1] = b[len - 1];

	for (i = len - 2; i >= 0; i--) {
		x[i] = b[i] - _ParaMpy(c[i], x[i + 1]);
	}


	////扩展
	//for (i = len-1; i > 0; i--){
	//	a[i] = a[i - 1];
	//}
	//if (b[0]==_ParaFrom(0)){
	//	return OSlwMatSolveZeroError;
	//}
	//c[0] = _ParaDiv(c[0], b[0]);
	//f[0] = _ParaDiv(f[0], b[0]);
	//for (i = 1; i < len - 1; i++)
	//{
	//	if (b[i] == _ParaFrom(0))
	//		return OSlwMatSolveZeroError;
	//	b[i] -= _ParaDiv(_ParaMpy(a[i], f[i - 1]), b[i]);
	//	c[i] = _ParaDiv(c[i], b[i]);
	//	f[i] = _ParaDiv(f[i] - _ParaMpy(a[i], f[i - 1]), b[i]);
	//}
	//if (b[i] == _ParaFrom(0))
	//	return OSlwMatSolveZeroError;
	//f[i] = _ParaDiv(
	//	f[i] - _ParaMpy(a[i], f[i - 1])
	//	,
	//	b[i] - _ParaMpy(a[i], c[i - 1])
	//);
	////整理结果
	//x[i] = f[i];
	//for (i = len - 2; i >= 0; i--) {
	//	x[i] = f[i] - c[i] * f[i + 1];
	//}

	return OSlwMatSolveSucces;
}



#endif //OSLW_TOOL_IMPORT_MATH || OSLW_TOOL_IMPORT_ALL

#endif // !(OSLW_SIMPLE_LEVEL >= 4)

