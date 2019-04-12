/*(Ver.=0.96)
* OSLW_tool.c
*
*  Created on: 2017-7-14
*      Author: slw
*/

#include "OSLW_include.h"
#if !(OSLW_SIMPLE_LEVEL >= 4)

#if OSLW_TOOL_IMPORT_MATH || OSLW_TOOL_IMPORT_ALL

void OSlwToolMatrixInitial(OSlwToolMatrixSTU *m, LwMatRowType row, LwMatColType col, ParaType *a)
{
    OSLW_assert(!(m));

    m->a = a;
    m->row = row;
    m->col = col;

    m->length = row*col;
}





OSLW_TOOL_FUN(
	OSlwToolMatrixSTU*, 
	OSlwToolMatrixAdd,          
	(OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *m1, OSlwToolMatrixSTU *m2),         
	OSlwToolMatrixAdd             
)
{

    
    register lw_u32 d;
    register ParaType *ps, *pm1, *pm2, data;
    OSLW_assert(!(s));
    OSLW_assert(!(m1));
    OSLW_assert(!(m2));

    d = s->length;

    ps = s->a;
    pm1 = m1->a;
    pm2 = m2->a;

    //if(s->row == m1->row && s->row == m2->row && s->col == m1->col && s->col == m2->col)
    if (s->length == m1->length && m1->length == m2->length)
    {
		while (d--)
		{
			*ps++ = _ParaAdd(*pm1++, *pm2++);
		}

    }
    else if(s->length == m1->length && m2->length == 1)
    {
        data = *(m2->a);
        while (d--)
        {
            *ps++ = _ParaAdd(*pm1++, data);
        }

    }
    else if (s->length == m2->length && m1->length == 1)
    {
        data = *(m1->a);
        while (d--)
        {
            *ps++ = _ParaAdd(*pm2++, data);
        }

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
    
    register lw_u32 d;
    register ParaType *ps, *pm1, *pm2, data;
    OSLW_assert(!(s));
    OSLW_assert(!(m1));
    OSLW_assert(!(m2));

    d = s->length;

    ps = s->a;
    pm1 = m1->a;
    pm2 = m2->a;

    //if(s->row == m1->row && s->row == m2->row && s->col == m1->col && s->col == m2->col)
    if (s->length == m1->length && m1->length == m2->length)
    {
        //for(i=0;i<d;i++)
        //s->a[i]=_ParaSub(m1->a[i],m2->a[i]);
        while (d--)
        {
            *ps++ = _ParaSub(*pm1++, *pm2++);
        }

    }
    else if (s->length == m1->length && m2->length == 1)
    {
        data = *(m2->a);
        while (d--)
        {
            *ps++ = _ParaSub(*pm1++, data);
        }

    }
    else if (s->length == m2->length && m1->length == 1)
    {
        data = *(m1->a);
        while (d--)
        {
            *ps++ = _ParaSub(*pm2++, data);
        }

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
    register lw_u32 d;

    register ParaType *ps, *pm1, *pm2, data;

    OSLW_assert(!(s));
    OSLW_assert(!(m1));
    OSLW_assert(!(m2));

    d = s->length;

    ps = s->a;
    pm1 = m1->a;
    pm2 = m2->a;

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
        while (d--)
        {
            *ps++ = _ParaMpy(*pm1++, data);
        }

    }
    else if (s->length == m2->length && m1->length == 1)
    {
        data = *(m1->a);
        while (d--)
        {
            *ps++ = _ParaMpy(*pm2++, data);
        }

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
    register LwMatColType j, k, row, col, row1;
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
			_ParaFint(0), s->a, s->col
		);


#else
#if L1_L2_CACHE_OPTIM == 1
		lw_u32 div8, mod8;

		row = s->row;
		col = s->col;
		row1 = m2->row;
		_sbuf = s->a;
		_m1buf = m1->a;
		_m2buf = m2->a;

		div8 = col / 8, mod8 = col % 8;

		LW_MAT_CLR(s);

		for (i = 0, _m1 = _m1buf; i < row; ++i)
		{
			for (j = 0, _m2 = _m2buf; j < row1; ++j, ++_m1)
			{
				temp = *_m1;
				for (k = 0, _s = _sbuf; k < div8; ++k)
				{
					_s[0] = _ParaAdd(_s[0], _ParaMpy(temp, _m2[0]));
					_s[1] = _ParaAdd(_s[1], _ParaMpy(temp, _m2[1]));
					_s[2] = _ParaAdd(_s[2], _ParaMpy(temp, _m2[2]));
					_s[3] = _ParaAdd(_s[3], _ParaMpy(temp, _m2[3]));

					_s[4] = _ParaAdd(_s[4], _ParaMpy(temp, _m2[4]));
					_s[5] = _ParaAdd(_s[5], _ParaMpy(temp, _m2[5]));
					_s[6] = _ParaAdd(_s[6], _ParaMpy(temp, _m2[6]));
					_s[7] = _ParaAdd(_s[7], _ParaMpy(temp, _m2[7]));

					_m2 += 8;
					_s += 8;

				}

				for (k = 0; k < mod8; ++k, ++_m2, ++_s)
				{
					*_s = _ParaAdd(*_s, _ParaMpy(temp, *_m2));
				}

			}
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
	(OSlwToolMatrixSTU *s, lw_u16 row, lw_u16 col),             
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
    register LwMatLenType i = 0;
    register ParaType *ps;
    OSLW_assert(!(s));
    if (a)//如果定义a 相当于复制构造函数
    {
        /*(Ver.=0.96)
        for (i = 0; i < s->length; i++)
        {
        s->a[i] = a->a[i];
        }
        */

        /*(Ver.=0.96)
        i = s->length;
        ps = s->a;
        pa = a->a;

        while (i--)
        {
        *ps++ = *pa++;
        }
        */
        if (s->length == a->length)
        {
            memcpy(s->a, a->a, PARA_MEM_CAL(s->length));
        }
        else
        {
            OSLW_assert(1);
        }

    }
    else//使用常量初始化
    {
        /*(Ver.=0.96)
        for (i = 0; i < s->length; i++)
        {
        s->a[i] = data;
        }
        */

        i = s->length;
        ps = s->a;

        while (i--)
        {
            *ps++ = data;
        }

    }

    return s;
}




OSLW_TOOL_FUN(OSlwToolMatrixSTU*, OSlwToolMatrixJoin,
(OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *m1, OSlwToolMatrixSTU *m2), OSlwToolMatrixJoin)
{
	register lw_u32 i, j;
	register ParaType *sa, *m1a, *m2a;
	if (s->length >= (m1->length + m2->length ))//满足拼接条件
	{
		
		sa = s->a;
		m1a = m1->a;
		m2a = m2->a;

		if (m1->row == m2->row)//行数相等采用行拼接
		{
			i = m1->row;
			while (i--)//行循环
			{
				//先复制M1
				j = m1->col;
				while (j--)
				{
					*sa++ = *m1a++;
				}
				//再复制M2
				j = m2->col;
				while (j--)
				{
					*sa++ = *m2a++;
				}
			}//end while 行循环
		}
		else//如果不是采用向量复制拼接
		{
			i = m1->length;

			while (i--)
			{
				*sa++ = *m1a++;
			}

			i = m2->length;
			
			while (i--)
			{
				*sa++ = *m2a++;
			}

		}


	}
	else if (s == m1 && s->length >= m2->length)
	{
		i = m2->length;
		sa = s->a + (s->length - m2->length);
		m2a = m2->a;
		while (i--)
		{
			*sa++ = *m2a++;
		}
	}
	else
	{
		OSLW_assert(1);
		
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
	register ParaType _sum = _ParaFint(0);
	register LwMatRowType r;
	register LwMatColType c;
	register LwMatLenType l;
	register lw_u32 i, j;
	register ParaType *s, *d, *si;

	OSLW_assert(!(SRC));
	l = SRC->length;
	r = SRC->row;
	c = SRC->col;
	s = SRC->a;
	switch (dim)
	{
	case 0x00:

#if defined(OSLW_USING_CBLAS) && OSLW_USING_CBLAS
		_sum=cblas_sasum(l, s, 1);
#else

		while (l--)
		{
			_sum = _ParaAdd(_sum, *s++);
		}
#endif

		break;

	case 0x10:
		OSLW_assert(!(DST));
		d = DST->a;
#if defined(OSLW_USING_CBLAS) && OSLW_USING_CBLAS
		_sum = cblas_sasum(l, s, 1);
#else
		while (l--)
		{
			_sum = _ParaAdd(_sum, *s++);
		}
#endif

		*d = _ParaAdd(*d, _sum);
		break;

	case 0x01:
		OSLW_assert(!(DST));
		OSLW_assert(!(DST->length >= r));

		d = DST->a;
		while (r--)
		{
			i = c;

#if defined(OSLW_USING_CBLAS) && OSLW_USING_CBLAS
			_sum = cblas_sasum(i, s, 1);
			s += i;
#else
			_sum = _ParaFrom(0);
			while (i--)
				_sum = _ParaAdd(_sum, *s++);
#endif

			*d++ = _sum;
		}
		break;

	case 0x11:
		OSLW_assert(!(DST));
		OSLW_assert(!(DST->length >= r));
		d = DST->a;
		while (r--)
		{
			i = c;

#if defined(OSLW_USING_CBLAS) && OSLW_USING_CBLAS
			_sum = cblas_sasum(i, s, 1);
			s += i;
#else
			_sum = _ParaFrom(0);
			while (i--)
				_sum = _ParaAdd(_sum, *s++);
#endif


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
#if defined(OSLW_USING_CBLAS) && OSLW_USING_CBLAS
			_sum = cblas_sasum(i, s, c);
			++s;
#else
			_sum = _ParaFrom(0);
			si = s++;
			while (i--)
			{
				_sum = _ParaAdd(_sum, *si);
				si += c;
			}
#endif

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
#if defined(OSLW_USING_CBLAS) && OSLW_USING_CBLAS
			_sum = cblas_sasum(i, s, c);
			++s;
#else
			_sum = _ParaFrom(0);
			si = s++;
			while (i--)
			{
				_sum = _ParaAdd(_sum, *si);
				si += c;
			}
#endif
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

#if defined(OSLW_USING_CBLAS) && OSLW_USING_CBLAS
	register LwMatColType j, k, row, col, row1;
	register LwMatLenType i;
	register ParaType temp, *_s, *_m1, *_m2, *_m1buf, *_sbuf, *_m2buf, *_bibuf, *_bi;
	
	OSLW_assert(!(s));
	OSLW_assert(!(x));
	OSLW_assert(!(we));
	OSLW_assert(!(bi));

	if (s->col == we->col && s->row == x->row && we->row == x->col && s->col == bi->length)//满足神经网络前向条件
	{
		row = s->row;
		col = s->col;
		row1 = x->col;
		_bibuf = bi->a;


		_sbuf = s->a;
		_m1buf = x->a;
		_m2buf = we->a;


		//先载入偏置

		if (_bibuf)
		{
			for (i = 0, _s = _sbuf; i < row; i++)
			{
				for (j = 0, _bi = _bibuf; j < col; j++)
				{
					*_s++ = *_bi++;
				}
			}
		}
		else
		{
			LW_MAT_CLR(s);
		}


		cblas_sgemm(
			CblasRowMajor,
			CblasNoTrans, CblasNoTrans,
			s->row, s->col, we->row,
			_ParaFint(1), x->a, x->col,
			we->a, we->col,
			_ParaFint(1), s->a, s->col
		);


	}


#else


#if L1_L2_CACHE_OPTIM == 1
	//register LwMatLenType i, j, k, row, col, row1;
	//register ParaType temp, *_s, *_sbuf, *_x, *_we, *_web, *_xb, *_bi, *_bibuf;
	
	register LwMatColType j, k, row, col, row1;
	register LwMatLenType i;
	register ParaType temp, *_s, *_m1, *_m2, *_m1buf, *_sbuf, *_m2buf, *_bibuf, *_bi;

	OSLW_assert(!(s));
	OSLW_assert(!(x));
	OSLW_assert(!(we));
	OSLW_assert(!(bi));

	//神经网络前向传播
	if (s->col == we->col && s->row == x->row && we->row == x->col && s->col == bi->length)//满足神经网络前向条件
	{
		row = s->row;
		col = s->col;
		row1 = x->col;
		_bibuf = bi->a;


		_sbuf = s->a;
		_m1buf = x->a;
		_m2buf = we->a;


		//先载入偏置

		if (_bibuf)
		{
			for (i = 0, _s=_sbuf; i < row; i++)
			{
				for (j = 0, _bi = _bibuf; j < col; j++)
				{
					*_s++ = *_bi++;
				}
			}
		}
		else
		{
			LW_MAT_CLR(s);
		}



		for (i = 0, _m1 = _m1buf; i < row; ++i)
		{
			for (j = 0, _m2 = _m2buf; j < row1; ++j, ++_m1)
			{
				temp = *_m1;
				for (k = 0, _s = _sbuf; k < col; ++k, ++_m2, ++_s)
				{
					*_s = _ParaAdd(*_s, _ParaMpy(temp, *_m2));
				}
			}
			_sbuf = _s;
		}
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
	case 0://都不用转置
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
	case 4:
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
	case 1://m2转置
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
	case 5:
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



	case 2://m1转置
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
	case 6:
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

	case 3://都转置
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
	case 7:
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
	case 0://都不用转置
		LW_MAT_CLR(s);
	case 4:
		if (s->row == m1->row && s->col == m2->col && m1->col == m2->row)
		{
			register LwMatLenType i, j, k, row = s->row, col = s->col, row1 = m2->row;
			register ParaType temp, *_s, *_m1, *_m2, *_m1buf= m1->a, *_sbuf= s->a, *_m2buf= m2->a;

			for (i = 0, _m1 = _m1buf; i < row; ++i)
			{
				for (j = 0, _m2 = _m2buf; j < row1; ++j, ++_m1)
				{
					temp = *_m1;
					for (k = 0, _s = _sbuf; k < col; ++k, ++_m2, ++_s)
					{
						*_s = _ParaAdd(*_s, _ParaMpy(temp, *_m2));
					}
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
	case 1://m2转置
		LW_MAT_CLR(s);		
	case 5:
		if (s->row == m1->row && s->col == m2->row && m1->col == m2->col)
		{
			register LwMatLenType i, j, k, row = s->row, col = s->col, row1 = m1->col;
			register ParaType temp, *_s, *_m1, *_m2, *_m1buf = m1->a, *_sbuf = s->a, *_m2buf = m2->a;

			for (i = 0, _s = _sbuf; i < row; ++i)
			{
				for (j = 0, _m2 = _m2buf; j < col; ++j,++_s)
				{
					temp = _ParaFint(0);
					for (k = 0, _m1 = _m1buf; k < row1; ++k, ++_m2,++_m1)
					{
						temp = _ParaAdd(temp, _ParaMpy(*_m1, *_m2));
					}

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



	case 2://m1转置
		LW_MAT_CLR(s);
	case 6:
		if (s->row == m1->col && s->col == m2->col && m1->row == m2->row)
		{
			register LwMatLenType i, j, k, row = s->row, col = s->col, row1 = m2->row;
			register ParaType temp, *_s, *_m1, *_m2, *_m1buf = m1->a, *_sbuf = s->a, *_m2buf = m2->a;

			for (i = 0, _m1 = _m1buf; i < row1; ++i)
			{
				for (j = 0, _s = _sbuf; j < row; ++j, ++_m1)
				{	
					temp = *_m1;
					for (k = 0,_m2=_m2buf; k < col; ++k, ++_s, ++_m2)
					{
						*_s = _ParaAdd(*_s, _ParaMpy(temp, *_m2));
					}
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

	case 3://都转置
		LW_MAT_CLR(s);
	case 7:
		if (s->row == m1->col && s->col == m2->row && m1->row == m2->col)
		{
			register LwMatLenType i, j, k, row = s->row, col = s->col, row1 = m1->row;
			register ParaType temp, *_s, *_m1, *_m2, *_m1buf = m1->a, *_sbuf1 = s->a, *_sbuf2 = s->a, *_m2buf = m2->a;

			for (i = 0, _m2 = _m2buf; i < col; ++i, ++_sbuf2)
			{
				for (j = 0, _m1 = _m1buf; j < row1; ++j, ++_m2)
				{
					temp = *_m2;
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
	
	ParaType sum_buf, temp;

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
				for (i = 0,m_r=m2->a; i < out_r; i++,m_r+= move_y)
				{
					for ( j = 0,m_c=m_r; j < out_c; j++,m_c+=move_x)
					{

						//求和
						sum_buf = _ParaFint(0);
						k_p = k_a;
						m_p = m_c;
						for (k = 0; k < kern_c; k++, m_p += sum_jump)
							for (l = 0; l < kern_r; l++)
								sum_buf += _ParaMpy(*k_p++, *m_p++);

						*s_a++ = sum_buf;
					}
				}
			}
			else
			{
				for (i = 0, m_r = m2->a; i < out_r; i++, m_r += move_y)
				{
					for (j = 0, m_c = m_r; j < out_c; j++, m_c += move_x)
					{

						//求和
						sum_buf = _ParaFint(0);
						k_p = k_a;
						m_p = m_c;
						for (k = 0; k < kern_c; k++, m_p += sum_jump)
							for (l = 0; l < kern_r; l++)
								sum_buf += _ParaMpy(*k_p++, *m_p++);

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

	lw_32 i, j, k, l, n;
	lw_32 out_r, out_c;
	lw_32 sum_jump1, sum_jump2, out_jump, kern_len;
	lw_32 kern_r, kern_c, kern_one_len;
	lw_32 in_r, in_c;

	ParaType temp, sum_buf;
	ParaType *o_a, *o_pic;
	ParaType *k_a, *k_p, *k_p2;
	ParaType *m_r, *m_c, *m_p, *m_px, *m_py;
	ParaType *pbuf, *pbuf2;

	lw_32 div2, mod2;

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
			l = m_out->col*m_out->row;
			for (i = 0; i < out_high; i++)
			{
				temp = bias->a[i];
				for (j = 0; j < l; j++)
				{
					*o_a++ = temp;
				}
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

	div2 = kern_len / 2, mod2 = kern_len % 2;

	if (FD_1_or_BK_0)
	{
		move_y *= m_in->col;
		sum_jump1 = m_in->col;
		sum_jump2 = in_c*in_r;
		out_jump = out_c*out_r;
		for (i = 0, m_r = m_in->a; i < out_r; i++, m_r += move_y)
		{
			for (j = 0, m_c = m_r; j < out_c; j++, m_c += move_x)
			{

				//抠图
				pbuf = fast_buf;//得到缓冲区
				for (k = 0, m_py = m_c; k < in_high; k++,m_py+=sum_jump2)
					for (l = 0, m_px=m_py; l < kern_r; l++, m_px+=sum_jump1)
						for (n = 0,m_p=m_px; n < kern_c; n++)
							*pbuf++ = *m_p++;

				//求和
				
				for (k = 0, k_p = k_a,o_pic=o_a; k < out_high; k++, o_pic += out_jump)
				{
					//for (sum_buf = _ParaFint(0), pbuf = fast_buf, l = 0; l < kern_len; l++)
					//{
					//	sum_buf+= _ParaMpy(*k_p++, *pbuf++);
					//}
#if defined(OSLW_USING_CBLAS) && OSLW_USING_CBLAS

					sum_buf = cblas_sdot(kern_len, k_p, 1, fast_buf, 1);
					k_p += kern_len;

#else
					for (sum_buf = _ParaFint(0), pbuf = fast_buf, l = 0; l < div2; l++)
					{
						sum_buf += _ParaMpy(k_p[0], pbuf[0]);
						sum_buf += _ParaMpy(k_p[1], pbuf[1]);
						k_p += 2;
						pbuf += 2;
					}

					l = mod2;
					while (l--)
					{
						sum_buf += _ParaMpy(*k_p++, *pbuf++);
					}
#endif
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
					l = j - kern_r + 1;
					for (m_c = m_r + k*in_c + l; l <= j; ++l, ++m_c)
					{
						pbuf2 = pbuf++;						
						if (!(k<0 || k >= in_c || l<0 || l >= in_r))
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
					for ( l = 0, pbuf = fast_buf, k_p2 = k_p + kern_one_len - 1, sum_buf = _ParaFint(0); l < in_high; l++, k_p2 += sum_jump1)
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

	lw_32 i, j, k, l, n;
	lw_32 out_r, out_c;
	lw_32 sum_jump1, sum_jump2, out_jump, kern_len;
	lw_32 kern_r, kern_c, kern_one_len;
	lw_32 in_r, in_c;

	lw_u32 col_count = 0, col_cmax, im2col_one_len;

	ParaType temp, sum_buf;
	ParaType *o_a;
	ParaType *k_a, *k_p, *k_p2;
	ParaType *m_r, *m_c, *m_p, *m_px, *m_py;
	ParaType *pbuf, *pbuf2;

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
	im2col_one_len = (kern_one_len*in_high);
	col_cmax = now_flow_len / im2col_one_len;
	col_cmax = col_cmax >= (out_c*out_r) ? (out_c*out_r) : col_cmax;
	OSLW_assert(!(col_cmax));

	//先加上偏置
	if (bias)
	{
		if (bias->a)
		{
			l = m_out->col*m_out->row;
			for (i = 0; i < out_high; i++)
			{
				temp = bias->a[i];
				for (j = 0; j < l; j++)
				{
					*o_a++ = temp;
				}
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
	out_jump = out_c*out_r;
	pbuf = fast_buf;//得到缓冲区

#if !(defined(OSLW_USING_CBLAS) && OSLW_USING_CBLAS)
	OSLW_assert(1);
#endif


	for (i = 0, m_r = m_in->a; i < out_r; i++, m_r += move_y)
	{
		for (j = 0, m_c = m_r; j < out_c; j++, m_c += move_x)
		{
			//抠图
			for (k = 0, m_py = m_c; k < in_high; k++, m_py += sum_jump2)
				for (l = 0, m_px = m_py; l < kern_r; l++, m_px += sum_jump1)
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
					_ParaFint(1), o_a, out_jump
				);
#endif
				o_a += col_cmax;
			}
		}
	}

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
			_ParaFint(1), o_a, out_jump
		);
#endif
		o_a += col_cmax;
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
	OSLW_assert(!(src));
	OSLW_assert(!(mean));
	OSLW_assert(!(var));

	r = src->row;
	c = src->col;
	
	if (dim==0x01)
	{
		pbase = src->a;
		_div_m = _ParaDiv(_ParaFrom(1), _ParaFint(c));
		for ( i = 0; i < r; ++i)
		{
			p = pbase;
			
			//求和

			
			for (j = 0, _sum1 = _ParaFrom(0); j < c; ++j, ++p)
			{
				_sum1 = _ParaAdd(_sum1, *p);
			}


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


OSLW_TOOL_FUN(void*, OSlwToolMatrixDotSum,
(
	OSlwToolMatrixSTU *y,
	OSlwToolMatrixSTU *x1,
	OSlwToolMatrixSTU *x2,
	lw_u8 dim
	),
	OSlwToolMatrixDotSum)
{

	LwMatRowType i, _r;
	LwMatColType j, _c;
	LwMatLenType l, _L;
	ParaType _sum;
	ParaType *ya, *ya_b;
	ParaType *x1a, *x1a_b;
	ParaType *x2a, *x2a_b;

	OSLW_assert(!(y));
	OSLW_assert(!(x1));
	OSLW_assert(!(x2));


	switch (dim)
	{

	case 0x00:
		x1a = x1->a;
		x2a = x2->a;
		_sum = _ParaFint(0);
		for ( l = 0,_L=x1->length; l < _L; ++l,++x1a,++x2a)
		{
			_sum = _ParaAdd(_sum, (_ParaMpy(*x1a, *x2a)));
		}
		y->a[0] = _sum;
		break;

	case 0x10:
		x1a = x1->a;
		x2a = x2->a;
		_sum = _ParaFint(0);
		for (l = 0, _L = x1->length; l < _L; ++l, ++x1a, ++x2a)
		{
			_sum = _ParaAdd(_sum, (_ParaMpy(*x1a, *x2a)));
		}
		y->a[0] = _ParaAdd(y->a[0], _sum);
		break;

	case 0x01:
		x1a = x1->a;
		x2a = x2->a;
		ya = y->a;
		for (i = 0, _r = x1->row, _c = x1->col; i < _r; ++i, ++ya)
		{
			for (j = 0, _sum = _ParaFint(0); j < _c; ++j, ++x1a, ++x2a)
			{
				_sum = _ParaAdd(_sum, (_ParaMpy(*x1a, *x2a)));
			}
			*ya = _sum;
		}

		break;

	case 0x11:
		x1a = x1->a;
		x2a = x2->a;
		ya = y->a;
		for (i = 0, _r = x1->row, _c = x1->col; i < _r; ++i, ++ya)
		{
			for (j = 0, _sum = _ParaFint(0); j < _c; ++j, ++x1a, ++x2a)
			{
				_sum = _ParaAdd(_sum, (_ParaMpy(*x1a, *x2a)));
			}
			*ya = _ParaAdd(*ya, _sum);
		}
		break;

	case 0x02:
		LW_MAT_CLR(y);
	case 0x12:
		x1a = x1->a;
		x2a = x2->a;
		ya_b = y->a;
		for (i = 0, _r = x1->row, _c = x1->col; i < _r; ++i, ++ya)
		{
			for (j = 0, ya = ya_b; j < _c; ++j, ++x1a, ++x2a, ++ya)
			{
				*ya = _ParaAdd(*ya, (_ParaMpy(*x1a, *x2a)));
			}
		}
		break;

	default:
		OSLW_assert(1);
		break;
	}


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

#endif //OSLW_TOOL_IMPORT_MATH || OSLW_TOOL_IMPORT_ALL

#endif // !(OSLW_SIMPLE_LEVEL >= 4)

