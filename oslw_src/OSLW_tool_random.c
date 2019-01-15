/*(Ver.=0.9~)(Beg.=0.9)
 * OSLW_tool.c
 *
 *  Created on: 2017-11-13
 *      Author: slw
 */

#include "OSLW_include.h"
#if !(OSLW_SIMPLE_LEVEL >= 3)


ParaType OSlwToolRandomRangeMapping(OSlwToolRandomBasicSTU *pRB,ParaType r,ParaType Rmin,ParaType Rmax,ParaType delt)
{
    ParaType center1=_ParaDiv(_ParaAdd(Rmax,Rmin),_ParaFint(2)),center2=_ParaDiv(_ParaAdd(pRB->Rmax,pRB->Rmin),_ParaFint(2));//º∆À„÷––ƒµ„
    ParaType range1=_ParaSub(Rmax,Rmin),range2=_ParaSub(pRB->Rmax,pRB->Rmin);//º∆À„∑∂Œß
    //ParaType data=_ParaAdd(center1,_ParaMpy(_ParaSub(r,center2),_ParaDiv(range1,range2)));//º∆À„∆´“∆  //º∆À„¥Ì¡À!!!!!!!!
	ParaType data = _ParaAdd(center1, _ParaMpy(_ParaSub(r, center2), _ParaDiv(range1, range2)));//º∆À„∆´“∆

    if(delt <= _ParaFint(0))
    {
        return data;
    }
    else
    {
        lw_32 d=_ParaInt(_ParaDiv(data,delt));//º∆À„”≥…‰ ‰≥ˆ
        return _ParaMpy(_ParaFint(d),delt);
    }

}


ParaType OSlwToolRandomNormal(OSlwToolRandomBasicSTU *pRB, ParaType Cent, ParaType Var)
{
#if OSLW_TOOL_RAND_NORM_METHOD == OSLW_TOOL_RAND_NORM_BM
	ParaType r1, r2;
	ParaType z;

	pRB->_normal_gen_flag = !pRB->_normal_gen_flag;
	if (!pRB->_normal_gen_flag)
	{
		return _ParaAdd(Cent, _ParaMpy(pRB->_normal_last, Var));
	}

	
	do
	{
		r1 = pRB->rand(pRB, _ParaFrom(1), _ParaFrom(0));
	} while (r1 <= _ParaFrom(0));

	r2= pRB->rand(pRB, _ParaFrom(1), _ParaFrom(0));
	z = _ParaMpy(
		_ParaSqrt(_ParaMpy(_ParaFrom(-2), _ParaLn(r1))),
		_ParaCos(_ParaMpy(_ParaFrom(2 * PI), r2))
	);

	pRB->_normal_last = _ParaMpy(
		_ParaSqrt(_ParaMpy(_ParaFrom(-2), _ParaLn(r1))),
		_ParaSin(_ParaMpy(_ParaFrom(2 * PI), r2))
	);

	return _ParaAdd(Cent, _ParaMpy(z, Var));

#elif OSLW_TOOL_RAND_NORM_METHOD == OSLW_TOOL_RAND_NORM_LLN
	lw_u16 i;
	ParaType sum = 0;

	for (i = 0; i < OSLW_TOOL_RAND_NORM_LLN_NUM; i++)
	{
		sum = _ParaAdd(sum, pRB->rand(pRB, _ParaFint(1), _ParaFint(0)));
	}

	sum = _ParaSub(sum, _ParaFrom(OSLW_TOOL_RAND_NORM_LLN_NUM / 2.0));
	sum = _ParaDiv(sum, _ParaFrom(OSLW_TOOL_RAND_NORM_LLN_DIV));

	return _ParaAdd(Cent, _ParaMpy(sum, Var));

#elif OSLW_TOOL_RAND_NORM_METHOD == OSLW_TOOL_RAND_NORM_MP
	ParaType r1, r2;
	ParaType s;
	
	pRB->_normal_gen_flag = !pRB->_normal_gen_flag;


	if (!pRB->_normal_gen_flag)
	{
		return _ParaAdd(Cent, _ParaMpy(pRB->_normal_last, Var));
	}

	
	do
	{
		r1 = pRB->rand(pRB, _ParaFrom(1), _ParaFrom(-1));
		r2 = pRB->rand(pRB, _ParaFrom(1), _ParaFrom(-1));

		s = _ParaAdd(
			_ParaMpy(r1, r1),
			_ParaMpy(r2, r2)
		);

	} while ((s >= _ParaFrom(1)) || (s == _ParaFrom(1)));

	s = _ParaSqrt(
		_ParaMpy(_ParaFrom(-2),
			_ParaDiv(_ParaLn(s), s)
		)
	);

	pRB->_normal_last = _ParaMpy(r2, s);
	s = _ParaMpy(s, r1);

	return _ParaAdd(Cent, _ParaMpy(s, Var));
#else
	return _ParaAdd(pRB->rand(pRB, Var, -Var) , Cent);
#endif // OSLW_TOOL_RAND_NORM_METHOD == OSLW_TOOL_RAND_NORM_BM


}

#if OSLW_TOOL_IMPORT_MATH || OSLW_TOOL_IMPORT_ALL

void OSlwToolMatrixRandomInitial(OSlwToolMatrixSTU *m, void *pRand, ParaType Rmin, ParaType Rmax)
{

	lw_u32 d = m->length, n = 0;
	ParaType *a;
	OSlwToolRandomBasicSTU *p = (OSlwToolRandomBasicSTU *)pRand;
	OSLW_assert(!(m));
	OSLW_assert(!(pRand));
	a = m->a;
	while (d--)
	{
		*a++ = p->rand((void *)p, Rmax, Rmin);
	}

}

void *OSlwToolMatrixToRandnChip(OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *m, OSlwToolRandomBasicSTU *pRand, ParaType var, ParaType Rmin, ParaType Rmax)
{
	register lw_u32 i;
	register ParaType *sa, *sm;
	ParaType d;
	
	OSLW_assert(!(m));
	OSLW_assert(!(s));
	OSLW_assert(!(pRand));

	if (s->length == m->length)
	{
		i = m->length;
		sa = s->a;
		sm = m->a;
		while (i--)
		{
			d = pRand->randn(pRand, *sm, var);
			d = d > Rmax ? Rmax : d;
			d = d < Rmin ? Rmin : d;
			*sa++ = d;
			sm++;
		}
	}


	return m;

}
#endif //OSLW_TOOL_IMPORT_MATH || OSLW_TOOL_IMPORT_ALL


//-----------------------
//Logistic ”≥…‰

#if OSLW_TOOL_IMPORT_RAND_CHAOS || OSLW_TOOL_IMPORT_ALL

OSlwToolRandomChaosLogisticSTU OSlwTRCLog =
{ _ParaFrom(1.0),_ParaFrom(0.0),OSlwToolRandomChaosLogisticFun,OSlwToolRandomChaosLogisticFunInt,OSlwToolRandomNormal,NULL,
#if OSLW_TOOL_RAND_NORM_METHOD == OSLW_TOOL_RAND_NORM_BM || OSLW_TOOL_RAND_NORM_METHOD == OSLW_TOOL_RAND_NORM_MP
_ParaFrom(0), 0,
#endif // OSLW_TOOL_RAND_NORM_METHOD == OSLW_TOOL_RAND_NORM_BM || OSLW_TOOL_RAND_NORM_METHOD == OSLW_TOOL_RAND_NORM_MP
(3.9),(0.41) };
OSlwToolRandomBasicSTU *pRCLog = (OSlwToolRandomBasicSTU *)&OSlwTRCLog;

ParaType OSlwToolRandomChaosLogisticFun(void *pRand,ParaType Rmin,ParaType Rmax)
{

    OSlwToolRandomChaosLogisticSTU *p=(OSlwToolRandomChaosLogisticSTU *) pRand;
    OSLW_assert(!pRand);

    //p->x=_ParaMpy(_ParaMpy(p->u,_ParaSub(_ParaFint(1),p->x)),p->x);
    p->x *= p->u*(1.0 - p->x);
    return OSlwToolRandomRangeMapping((OSlwToolRandomBasicSTU *)p,p->x,Rmin,Rmax,_ParaFint(-1));
}


lw_32 OSlwToolRandomChaosLogisticFunInt(void *pRand,ParaType Rmin,ParaType Rmax)
{

    OSlwToolRandomChaosLogisticSTU *p=(OSlwToolRandomChaosLogisticSTU *) pRand;
    OSLW_assert(!pRand);

    OSlwToolRandomChaosLogisticFun(pRand,p->basic.Rmax,p->basic.Rmin);

    return _ParaInt(OSlwToolRandomRangeMapping((OSlwToolRandomBasicSTU *)p,p->x,Rmin,Rmax,_ParaFint(1)));

}

//-----------------------
//’ ≈Ò”≥…‰

OSlwToolRandomChaosTentSTU OSlwTRCTent=
{_ParaFrom(1.0),_ParaFrom(-1.0),OSlwToolRandomChaosTentFun,OSlwToolRandomChaosTentFunInt,OSlwToolRandomNormal,NULL,
#if OSLW_TOOL_RAND_NORM_METHOD == OSLW_TOOL_RAND_NORM_BM || OSLW_TOOL_RAND_NORM_METHOD == OSLW_TOOL_RAND_NORM_MP
_ParaFrom(0), 0,
#endif // OSLW_TOOL_RAND_NORM_METHOD == OSLW_TOOL_RAND_NORM_BM || OSLW_TOOL_RAND_NORM_METHOD == OSLW_TOOL_RAND_NORM_MP
(0.995),(0.01)};

OSlwToolRandomBasicSTU *pRCTent = (OSlwToolRandomBasicSTU *)&OSlwTRCTent;

ParaType OSlwToolRandomChaosTentFun(void *pRand,ParaType Rmin,ParaType Rmax)
{
    OSlwToolRandomChaosTentSTU *p=(OSlwToolRandomChaosTentSTU *) pRand;
    OSLW_assert(!pRand);

    //p->x=_ParaSub(p->a,_ParaMpy(_ParaAdd(_ParaFint(1),p->a),_ParaAbs(p->x)));
    p->x = p->a - (1 + p->a)*(p->x > 0.0 ? p->x : -(p->x));
    return OSlwToolRandomRangeMapping((OSlwToolRandomBasicSTU *)p,p->x,Rmin,Rmax,_ParaFint(-1));
}

lw_32 OSlwToolRandomChaosTentFunInt(void *pRand,ParaType Rmin,ParaType Rmax)
{
    OSlwToolRandomChaosTentSTU *p=(OSlwToolRandomChaosTentSTU *) pRand;
    OSLW_assert(!pRand);

    OSlwToolRandomChaosTentFun(pRand,p->basic.Rmax,p->basic.Rmin);

    return _ParaInt(OSlwToolRandomRangeMapping((OSlwToolRandomBasicSTU *)p,p->x,Rmin,Rmax,_ParaFint(1)));

}

//-----------------------
//KENT”≥…‰
OSlwToolRandomChaosKentSTU OSlwTRCKent=
{_ParaFrom(1.0),_ParaFrom(0.0),OSlwToolRandomChaosKentFun,OSlwToolRandomChaosKentFunInt,OSlwToolRandomNormal,NULL,
#if OSLW_TOOL_RAND_NORM_METHOD == OSLW_TOOL_RAND_NORM_BM || OSLW_TOOL_RAND_NORM_METHOD == OSLW_TOOL_RAND_NORM_MP
_ParaFrom(0), 0,
#endif // OSLW_TOOL_RAND_NORM_METHOD == OSLW_TOOL_RAND_NORM_BM || OSLW_TOOL_RAND_NORM_METHOD == OSLW_TOOL_RAND_NORM_MP
(0.3),(0.01)};

OSlwToolRandomBasicSTU *pRCKent = (OSlwToolRandomBasicSTU *)&OSlwTRCKent;

ParaType OSlwToolRandomChaosKentFun(void *pRand,ParaType Rmin,ParaType Rmax)
{
    OSlwToolRandomChaosKentSTU *p=(OSlwToolRandomChaosKentSTU *) pRand;
    OSLW_assert(!pRand);

    /*(Ver.=0.9~)(Beg.=0.9)
    if(p->x <= p->a)
    {
        p->x=_ParaDiv(p->x,p->a);
    }
    else
    {
        p->x=_ParaDiv(_ParaSub(_ParaFint(1),p->x),_ParaSub(_ParaFint(1),p->a));
    }
    */
    if (p->x <= p->a)
    {
        p->x /=  p->a;
    }
    else
    {
        p->x = (1.0 - p->x) / (1.0 - p->a);
    }


    return OSlwToolRandomRangeMapping((OSlwToolRandomBasicSTU *)p,p->x,Rmin,Rmax,_ParaFint(-1));

}

lw_32 OSlwToolRandomChaosKentFunInt(void *pRand,ParaType Rmin,ParaType Rmax)
{
    OSlwToolRandomChaosKentSTU *p=(OSlwToolRandomChaosKentSTU *) pRand;
    OSLW_assert(!pRand);

    OSlwToolRandomChaosKentFun(pRand,p->basic.Rmax,p->basic.Rmin);

    return _ParaInt(OSlwToolRandomRangeMapping((OSlwToolRandomBasicSTU *)p,p->x,Rmin,Rmax,_ParaFint(1)));
}
#endif // OSLW_TOOL_IMPORT_RAND_CHAOS || OSLW_TOOL_IMPORT_ALL

//-----------------------
//œﬂ–‘Õ¨”‡

OSlwToolRandomLCGSTU OSlwTRLcg =
{ _ParaFrom(1.0),_ParaFrom(0.0),OSlwToolRandomLCGFun,OSlwToolRandomLCGFunInt,OSlwToolRandomNormal,NULL,
#if OSLW_TOOL_RAND_NORM_METHOD == OSLW_TOOL_RAND_NORM_BM || OSLW_TOOL_RAND_NORM_METHOD == OSLW_TOOL_RAND_NORM_MP
_ParaFrom(0), 0,
#endif // OSLW_TOOL_RAND_NORM_METHOD == OSLW_TOOL_RAND_NORM_BM || OSLW_TOOL_RAND_NORM_METHOD == OSLW_TOOL_RAND_NORM_MP
0l,1103515245UL,12345,0xffffffffUL };

OSlwToolRandomBasicSTU *pRLcg = (OSlwToolRandomBasicSTU *)&OSlwTRLcg;

ParaType OSlwToolRandomLCGFun(void *pRand, ParaType Rmin, ParaType Rmax)
{
    OSlwToolRandomLCGSTU *p = (OSlwToolRandomLCGSTU *)pRand;
    ParaType res;
	lw_u32 _r1;
    OSLW_assert(!pRand);
    p->rand_x = ((lw_u32)(p->rand_x) *(lw_u32)(p->rand_a) + (lw_u32)(p->rand_c)) & p->rand_mod;
	_r1 = ((p->rand_x) >> 16) & 0x7fff;
	p->rand_x = ((lw_u32)(p->rand_x) *(lw_u32)(p->rand_a) + (lw_u32)(p->rand_c)) & p->rand_mod;
	_r1 = _r1 | (((p->rand_x) & 0x7fff0000) >> 1);

    res = _ParaMpy(_ParaFint(_r1), _ParaFrom(1.0/0x3fffffff));

    return OSlwToolRandomRangeMapping((OSlwToolRandomBasicSTU *)p, res, Rmin, Rmax, _ParaFint(-1));


}
lw_32 OSlwToolRandomLCGFunInt(void *pRand, ParaType Rmin, ParaType Rmax)
{
    OSlwToolRandomLCGSTU *p = (OSlwToolRandomLCGSTU *)pRand;
	lw_u32 _r1;
    OSLW_assert(!pRand);
	p->rand_x = ((lw_u32)(p->rand_x) *(lw_u32)(p->rand_a) + (lw_u32)(p->rand_c)) & p->rand_mod;
	_r1 = ((p->rand_x) >> 16) & 0x7fff;
	p->rand_x = ((lw_u32)(p->rand_x) *(lw_u32)(p->rand_a) + (lw_u32)(p->rand_c)) & p->rand_mod;
	_r1 = _r1 | (((p->rand_x) & 0x7fff0000) >> 1);

    return (_r1) % (_ParaInt(Rmax) - _ParaInt(Rmin)) + _ParaInt(Rmin);


}

#if OSLW_TOOL_IMPORT_RAND_WELL || OSLW_TOOL_IMPORT_ALL

static lw_u32 _OSlwToolRandWELL512Rand(OSlwToolRandomWELL512STU *_pr)
{
	register lw_u32 a, b, c, d;
	register lw_u32 *state;
	register lw_u32 index;

	index = _pr->_index;
	state = _pr->_state;

	a = state[index];
	c = state[(index + 13) & 15];
	b = a^c ^ (a << 16) ^ (c << 15);
	c = state[(index + 9) & 15];
	c ^= (c >> 11);
	a = state[index] = b^c;
	d = a ^ ((a << 5) & 0xDA442D20);
	index = (index + 15) & 15;
	a = state[index];
	state[index] = a^b^d ^ (a << 2) ^ (b << 18) ^ (c << 28);

	_pr->_index = index;

	return state[index];

}

void OSlwToolRandomWELL512Seed(OSlwToolRandomWELL512STU *pRand, lw_u32 seed)
{
	lw_u32 i = 16;
	lw_u32 *p;
	OSLW_assert(!(pRand));

	memset(pRand, 0, sizeof(OSlwToolRandomWELL512STU));

	seed = seed ? seed : 1;

	p = pRand->_state;
	while (i--)
	{
		*p++ = seed;
	}


	pRand->basic.Rmax = _ParaFint(1);
	pRand->basic.Rmin = _ParaFint(0);
	
	pRand->basic.rand = OSlwToolRandomWELL512Fun;
	pRand->basic.randint = OSlwToolRandomWELL512FunInt;
	pRand->basic.randn = OSlwToolRandomNormal;
	pRand->basic.Seed = OSlwToolRandomWELL512Seed;


}

ParaType OSlwToolRandomWELL512Fun(void *pRand, ParaType Rmin, ParaType Rmax)
{
	lw_u32 _r;

	OSLW_assert(!(pRand));

	_r = _OSlwToolRandWELL512Rand(pRand);

	return OSlwToolRandomRangeMapping((OSlwToolRandomBasicSTU *)pRand, _ParaMpy(_ParaFrom(_r), _ParaFrom(1.0/0xFFFFFFFF)), Rmin, Rmax, _ParaFint(-1));
}


lw_32 OSlwToolRandomWELL512FunInt(void *pRand, ParaType Rmin, ParaType Rmax)
{
	lw_u32 _r;

	OSLW_assert(!(pRand));

	_r = _OSlwToolRandWELL512Rand(pRand);

	return (_r) % (_ParaInt(Rmax) - _ParaInt(Rmin)) + _ParaInt(Rmin);
}
#endif // OSLW_TOOL_IMPORT_RAND_WELL || OSLW_TOOL_IMPORT_ALL



#if OSLW_TOOL_IMPORT_RAND_MT || OSLW_TOOL_IMPORT_ALL

#define N 624
#define M 397
#define R 31
#define A 0x9908B0DF
#define F 1812433253
#define U 11
#define S 7
#define B 0x9D2C5680
#define T 15
#define C 0xEFC60000
#define L 18
#define MASK_LOWER (1ull << R) - 1
#define MASK_UPPER (1ull << R)


static void _OSlwToolRandomMT_Twist(OSlwToolRandomMTSTU *pR)
{
	register lw_u32  i, x, xA;
	register lw_u32 *mt;

	mt = pR->_mt;

	for (i = 0; i < N; i++)
	{
		x = (mt[i] & MASK_UPPER) + (mt[(i + 1) % N] & MASK_LOWER);

		xA = x >> 1;

		if (x & 0x1)
			xA ^= A;

		mt[i] = mt[(i + M) % N] ^ xA;
	}

	pR->_index = 0;
}

static lw_u32 _OSlwToolRandomMT_Rand(OSlwToolRandomMTSTU *pR)
{
	lw_u32 y;
	lw_u32 i = pR->_index;

	if (i >= N)
	{
		_OSlwToolRandomMT_Twist(pR);
		i = pR->_index;
	}

	y = pR->_mt[i];
	pR->_index = i + 1;

	y ^= (pR->_mt[i] >> U);
	y ^= (y << S) & B;
	y ^= (y << T) & C;
	y ^= (y >> L);

	return y;

}


void OSlwToolRandomMTSeed(OSlwToolRandomMTSTU *pRand, lw_u32 seed)
{
	lw_u32  i;
	pRand->_mt[0] = seed;
	for (i = 1; i < N; i++)
	{
		pRand->_mt[i] = (F * ((pRand->_mt[i - 1]) ^ ((pRand->_mt[i - 1]) >> 30)) + i);
	}
	pRand->_index = N;
	pRand->basic.rand = OSlwToolRandomMTFun;
	pRand->basic.randint = OSlwToolRandomMTFunInt;
	pRand->basic.randn = OSlwToolRandomNormal;
	pRand->basic.Seed = OSlwToolRandomMTSeed;

	pRand->basic.Rmax = _ParaFint(1);
	pRand->basic.Rmin = _ParaFint(0);
#if OSLW_TOOL_RAND_NORM_METHOD == OSLW_TOOL_RAND_NORM_BM || OSLW_TOOL_RAND_NORM_METHOD == OSLW_TOOL_RAND_NORM_MP
	pRand->basic._normal_gen_flag = 0;
#endif // OSLW_TOOL_RAND_NORM_METHOD == OSLW_TOOL_RAND_NORM_BM || OSLW_TOOL_RAND_NORM_METHOD == OSLW_TOOL_RAND_NORM_MP

}


ParaType OSlwToolRandomMTFun(void *pRand, ParaType Rmin, ParaType Rmax)
{
	lw_u32 _r;

	OSLW_assert(!(pRand));

	_r = _OSlwToolRandomMT_Rand(pRand);

	return OSlwToolRandomRangeMapping((OSlwToolRandomBasicSTU *)pRand, _ParaMpy(_ParaFrom(_r), _ParaFrom(1.0/0xFFFFFFFF)), Rmin, Rmax, _ParaFint(-1));
}

lw_32 OSlwToolRandomMTFunInt(void *pRand, ParaType Rmin, ParaType Rmax)
{
	lw_u32 _r;

	OSLW_assert(!(pRand));
	_r = _OSlwToolRandomMT_Rand(pRand);
	return (_r) % (_ParaInt(Rmax) - _ParaInt(Rmin)) + _ParaInt(Rmin);
}



#undef N 
#undef M
#undef R
#undef A
#undef F 
#undef U 
#undef S 
#undef B
#undef T
#undef C
#undef L
#undef MASK_LOWER
#undef MASK_UPPER

#endif // OSLW_TOOL_IMPORT_RAND_MT || OSLW_TOOL_IMPORT_ALL



#endif // !(OSLW_SIMPLE_LEVEL >= 3)
