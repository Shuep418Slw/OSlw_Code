#include "OSLW_include.h"
#if OSLW_SIMPLE_LEVEL<4 && OSLW_STR_DBG_EN



static const lw_u8 _kind_info[][4] = { {'c','c',sizeof(lw_u8),0},{ 'i','d',sizeof(lw_u16),0 },{ 'l','d',sizeof(lw_u32),0 },{ 'f','f',sizeof(ParaType),0 },{ 'F','s',0,0 } };


lw_u8 *_StringDebugSelectInfo(lw_8 c)
{
	lw_u16 i = 0;
	for (i = 0; i < (sizeof(_kind_info)/sizeof(_kind_info[0])); i++)
	{
		if (_kind_info[i][0] == c)
			return (lw_u8 *)_kind_info[i];
	}
	return NULL;
}

#if OSLW_STR_DBG_SIMPLE_MODE==0 || OSLW_STR_DBG_SIMPLE_MODE==1
lw_u8 * _StringDebugFindChar(lw_8 *name, lw_8 c)
{
	lw_u8 *p=NULL;
	do {
		if (*name == c)
		{
			p = name;
			break;
		}
	} while (*name++);
	return p;
}

lw_u8 _StringDebugFindCharNum(lw_8 *name, lw_8 c)
{
	lw_u8 _count = 0;
	do {
		if (*name == c)
			_count++;
	} while (*name++);
	
	return _count;
}	


lw_u8 *_SD_Data2Str(lw_u8 *str, void *data, lw_u8 *form,lw_u8 *buf,lw_u8 *info)
{

	switch (info[0])
	{
	case 'c':
		str = SD_Sprintf(str, form, (lw_8)(*((lw_8 *)data)));
		break;
	case 'i':
		str = SD_Sprintf(str, form, (lw_16)(*((lw_16 *)data)));
		break;
	case 'l':
		str = SD_Sprintf(str, form, (lw_32)(*((lw_32*)data)));
		break;
	case 'f':
		str = SD_Sprintf(str, form, (ParaType)(*((ParaType *)data)));
		break;

	default:
		break;
	}


	if (form[1]!='c')
	{
		*str++ = '\n';
	}
	
	return str;
}



lw_u8 * _SD_HashNode2Str(lw_u8 *str, OSlwToolDListNodeSTU *pdln)
{

	str = SD_Sprintf(str, "%s %c[%d]", pdln->Key.pData, (lw_u8)(pdln->Data.uData & 0xff), (lw_u8)((pdln->Data.uData >> 8) & 0xff));
	*str = '\n';
	return str + 1;

}

void * _StringDebugFind(OSlwToolHashSTU *ph, lw_8 *name, lw_8 *pkind,lw_u8 *pindex, OSlwToolDListNodeSTU **ppdln,lw_u8 *pdata_sel_kind)
{
	OSlwToolDListNodeSTU *pdln = NULL;
	lw_u8 *pend;
	lw_u8 _index = 0, _mix_k = 1;
	lw_u8 kind;
	lw_u8 *info;
	void *pre;
	pend = _StringDebugFindChar(name, ']');
	if (pend)//如果是查找子数据
	{
		lw_u16 i = 0;
		pend--;
		for (;i < 3 && (*pend >= '0' && *pend <= '9');pend--, i++)
		{
			_index += (*pend - '0')*_mix_k;
			_mix_k *= 10;
		}
		
		*pend = 0;
		*pdata_sel_kind = '[';
	}
	else
	{
		_index = 0;
		*pdata_sel_kind = ' ';
	}

	if (ph->SelectFun(ph, name, NULL, &pdln))//查找数据
	{
		
		if((pdln->Data.uData & 0xff)=='F')
		{
			pre=pdln->Data.pData;
			kind='F';
			info = _StringDebugSelectInfo(kind);//查找对应信息
		}		
		else
		{
			if (((pdln->Data.uData>>8)&0x000000ff) <= _index)
			{
				pre = NULL;
			}
			else
			{
				kind = (pdln->Data.uData ) & 0xff;//16~8位存储总大小
				info = _StringDebugSelectInfo(kind);//查找对应信息
				pre= ((lw_u8 *)pdln->Data.pData) + info[2] / (sizeof(lw_u8))*_index;//返回指针
			}
		}

		*pkind = kind;
		*pindex = _index;
		*ppdln = pdln;
		

	}
	else//如果没有
	{
		pre= NULL;
	}

	return pre;

}

lw_u8 _StringDebugSliceSELECT(lw_u8 *instr, lw_u8 *name, lw_u8 *pkind)
{
	if (instr[0] != '?')
	{
		return 0;
	}
	instr++;
	_LW_STR_SKIP_COPY(name, instr, ' ');
	if (instr[0]==0)
	{
		return 1;
	}
	while (*instr++ == ' ');
	*pkind = instr[-1];

	return 2;
}

lw_u8 _StringDebugSliceFUN(lw_u8 *instr, lw_u8 *name, lw_u8 *data)
{
	if (instr[0] != '@')
	{
		return 0;
	}
	instr++;
	_LW_STR_SKIP_COPY(name, instr, ' ');
	if (*instr!=' ')
	{
		return 1;
	}

	while (*instr++ == ' ');
	instr--;
	_LW_STR_SKIP_COPY(data, instr, 0);

	return 2;
}


#endif


void *_SD_MemCpy(void *dst, void *src, lw_u32 len) 
{
	lw_u8 *p = dst;
	lw_u8 *q = src; 
	while (len--)
	{
		*p++ = *q++;
	}
	return (void *)p;
}


lw_u8 _StringDebugSliceUPDATE(lw_u8 *instr, lw_u8 *name, lw_u8 *data)
{
	if (instr[0] != '=')
	{
		return 0;
	}
	instr++;
	_LW_STR_SKIP_COPY(name, instr, ' ');
	while (*instr++ == ' ');
	instr--;
	_LW_STR_SKIP_COPY(data, instr, 0);

	return 2;
}



void OSlwStrDbgInitial(struct OSLW_STRING_DEBUG_STRUCT *pSD)
{
	memset(pSD,0,sizeof(struct OSLW_STRING_DEBUG_STRUCT));
	
#if OSLW_STR_DBG_SIMPLE_MODE==0
	OSlwMemoryMapInital(&(pSD->DebugMem), pSD->DebugHeap, OSLW_STR_DBG_MEM_LEN, OSLW_STR_DBG_MEM_SIZE, sizeof(pSD->DebugHeap));
#endif
	
#if OSLW_STR_DBG_SIMPLE_MODE==0 || OSLW_STR_DBG_SIMPLE_MODE==1 	
	OSlwToolHashInitial(&(pSD->DebugHash), OSLW_STR_DBG_HASH_LEN, HashCalStringBKDR, HashCmpString);
#endif
	
}	

void OSlwStrDbgRun(struct OSLW_STRING_DEBUG_STRUCT *pSD,lw_u8 *instr)
{
	
	memset(&(pSD->StrDbgAllStr), 0, sizeof(pSD->StrDbgAllStr));
	
	
#if OSLW_STR_DBG_SIMPLE_MODE==0
	StringDebug(&(pSD->DebugHash), (void *)&(pSD->DebugMem), instr, pSD->StrDbgAllStr.OutStr, pSD->StrDbgAllStr.UsingStr.NameStr, pSD->StrDbgAllStr.UsingStr.DataStr);
#elif OSLW_STR_DBG_SIMPLE_MODE==1
	StringDebug(&(pSD->DebugHash), (void *)&(pSD->DebugHeap), instr, pSD->StrDbgAllStr.OutStr, pSD->StrDbgAllStr.UsingStr.NameStr, pSD->StrDbgAllStr.UsingStr.DataStr);
#elif OSLW_STR_DBG_SIMPLE_MODE==2
	StringDebug(pSD,instr,pSD->StrDbgAllStr.OutStr, pSD->StrDbgAllStr.UsingStr.NameStr, pSD->StrDbgAllStr.UsingStr.DataStr);
#endif
	
	
	return;	
}	


#if OSLW_STR_DBG_SIMPLE_MODE==0

lw_u32 _StringDebugMemCalSq(lw_u8 *data, lw_u8 *info, lw_u8 *pNum)
{
	lw_u8 _len = 0;
	lw_u32 i = 0;

	data++;
	for (;i < 3 && (*data >= '0' && *data <= '9');i++)
	{
		_len *= 10;
		_len += (*data - '0');
		data++;
	}
	
	*pNum = _len;
	return (lw_u32)_len * (lw_u32)info[2];
}

lw_u32 _StringDebugMemLengthPl(lw_u8 *data, lw_u8 *info,ParaType *pstart, ParaType *pdelt,lw_u8 *pNum)
{
	lw_u8 num = 0;
	ParaType _start, _delt, _end;
	lw_u8 *s_s, *s_e, *s_d;
	
	data++;
	if (info[0]=='f')//是浮点
	{
		lw_u8 t= _StringDebugFindCharNum(data, ':');
		if (t==1)
		{
			_delt = _ParaFint(1);
			_LW_STR_SKIP_FIND(s_s, data, ':');
			_LW_STR_SKIP_FIND(s_e, data, ')');
			_start = SD_Str2F(s_s);
			_end = SD_Str2F(s_e);
		}
		else if (t==2)
		{
			_LW_STR_SKIP_FIND(s_s, data, ':');
			_LW_STR_SKIP_FIND(s_d, data, ':');
			_LW_STR_SKIP_FIND(s_e, data, ')');
			_start = SD_Str2F(s_s);
			_delt = SD_Str2F(s_d);
			_end = SD_Str2F(s_e);
		}
		else
		{
			return 0;
		}
	}
	else//不是浮点
	{
		lw_u8 t = _StringDebugFindCharNum(data, ':');
		if (t == 1)
		{
			_delt = _ParaFint(1);
			_LW_STR_SKIP_FIND(s_s, data, ':');
			_LW_STR_SKIP_FIND(s_e, data, ')');
			_start = _ParaFint(SD_Str2I(s_s));
			_end = _ParaFint(SD_Str2I(s_e));
		}
		else if (t == 2)
		{
			_LW_STR_SKIP_FIND(s_s, data, ':');
			_LW_STR_SKIP_FIND(s_d, data, ':');
			_LW_STR_SKIP_FIND(s_e, data, ')');
			_start = _ParaFint(SD_Str2I(s_s));
			_delt = _ParaFint(SD_Str2I(s_d));
			_end = _ParaFint(SD_Str2I(s_e));
		}
		else
		{
			return 0;
		}
	}

	if (_delt <= 0)
	{
		_delt = _ParaFint(1);
	}

	num = _ParaInt((_end - _start) / _delt) & 0xff;

	*pstart = _start;
	*pdelt = _delt;
	*pNum = num+1;
	

	return (lw_u32)(num+1) * (lw_u32)info[2];
}

lw_u32 _StringDebugMemLengthBrace(lw_u8 *data, lw_u8 *info,lw_u8 *pNum)
{
	lw_u8 _num;
	_num = _StringDebugFindCharNum(data, ',');

	*pNum = _num+1;
	return (lw_u32)(_num+1) * (lw_u32)info[2];
}




//格式字符串转换为数组
void *_StringDebugString2Number(OSlwMemoryBasicSTU *pmem, lw_u8 *str, lw_u8 *info,lw_u8 *outstr,lw_u32 *pdsize,lw_u8 *pdlen,lw_u8 *pstatus)
{
	lw_u32 mem_size = 0, i = 0;
	lw_u8 data_len;
	lw_u8 data_len_save;
	void *pdata = NULL;
	lw_u8 *pstr;

	lw_u8 *d8;
	lw_u16 *d16;
	lw_u32 *d32;
	ParaType *df;

	ParaType startn, deltn;

	switch (str[0])
	{
	case '{':
		*pstatus = '{';
		mem_size = _StringDebugMemLengthBrace(str, info, &data_len);
		data_len_save = data_len;
		if (mem_size == 0)
		{
			strcpy(outstr, "Using {} is Error \n");
			return NULL;
		}

		pdata = pmem->Malloc(pmem, mem_size);
		if (pdata == NULL)
		{
			strcpy(outstr, "Memory is exhausted\n");
			return NULL;
		}

		//避开‘{’
		str++;

		//对应不同类型
		switch (info[0])
		{
		case 'c':
			d8 = pdata;
			while (--data_len)
			{
				_LW_STR_SKIP_FIND(pstr, str, ',');
				*d8++ = SD_Str2I(pstr) & 0XFF;
			}

			_LW_STR_SKIP_FIND(pstr, str, '}');
			*d8++ = SD_Str2I(pstr) & 0XFF;

			break;
		case 'i':
			d16 = pdata;
			while (--data_len)
			{
				_LW_STR_SKIP_FIND(pstr, str, ',');
				*d16++ = SD_Str2I(pstr) & 0XFFFF;
			}

			_LW_STR_SKIP_FIND(pstr, str, '}');
			*d16++ = SD_Str2I(pstr) & 0XFFFF;

			break;
		case 'l':
			d32 = pdata;
			while (--data_len)
			{
				_LW_STR_SKIP_FIND(pstr, str, ',');
				*d32++ = SD_Str2I(pstr);
			}

			_LW_STR_SKIP_FIND(pstr, str, '}');
			*d32++ = SD_Str2I(pstr);

			break;
		case 'f':
			df = pdata;
			while (--data_len)
			{
				_LW_STR_SKIP_FIND(pstr, str, ',');
				*df++ = SD_Str2F(pstr);
			}
			_LW_STR_SKIP_FIND(pstr, str, '}');
			*df++ = SD_Str2F(pstr);
			break;
		default:
			if (pmem->Free(pmem, pdata))
			{
				strcpy(outstr, "Type Unkown\n");
				return NULL;
			}
			else
			{
				strcpy(outstr, "Memory Free Error\n");
				return NULL;
			}
			break;
		}// end switch (info[0])
		
		break;//end case '{':
	
	case '[':
		*pstatus = '[';
		mem_size = _StringDebugMemCalSq(str, info, &data_len);
		data_len_save = data_len;
		if (mem_size == 0)
		{
			strcpy(outstr, "Using [] is Error \n");
			return NULL;
		}

		pdata = pmem->Malloc(pmem, mem_size);
		if (pdata == NULL)
		{
			strcpy(outstr, "Memory is exhausted\n");
			return NULL;
		}

		memset(pdata, 0, (mem_size));

		break;//end case '[':

	case '(':
		*pstatus = '(';
		mem_size = _StringDebugMemLengthPl(str, info,&startn,&deltn,&data_len);
		data_len_save = data_len;
		if (mem_size == 0)
		{
			strcpy(outstr, "Using () is Error \n");
			return NULL;
		}

		pdata = pmem->Malloc(pmem, mem_size);
		if (pdata == NULL)
		{
			strcpy(outstr, "Memory is exhausted\n");
			return NULL;
		}

		//对应不同类型
		switch (info[0])
		{
		case 'c':
			d8 = pdata;
			while (data_len--)
			{
				*d8++ = _ParaInt(startn) & 0xff;
				startn += deltn;
			}
			break;
		case 'i':
			d16 = pdata;
			while (data_len--)
			{
				*d16++ = _ParaInt(startn) & 0xffff;
				startn += deltn;
			}

			break;
		case 'l':
			d32 = pdata;
			while (data_len--)
			{
				*d32++ = _ParaInt(startn) & 0xffffffff;
				startn += deltn;
			}
			break;
		case 'f':
			df = pdata;
			while (data_len--)
			{
				*df++ = startn;
				startn += deltn;
			}
			break;
		default:
			if (pmem->Free(pmem, pdata))
			{
				strcpy(outstr, "Type Unkown\n");
				return NULL;
			}
			else
			{
				strcpy(outstr, "Memory Free Error\n");
				return NULL;
			}
			break;
		}// end switch (info[0])

		break;//end case '(':

	case '\'':
		*pstatus = '\'';
		mem_size = strlen(str) - 1;
		data_len_save = mem_size;
		if (mem_size == 0)
		{
			strcpy(outstr, "Using \' \' is Error \n");
			return NULL;
		}

		pdata = pmem->Malloc(pmem, mem_size);
		if (pdata == NULL)
		{
			strcpy(outstr, "Memory is exhausted\n");
			return NULL;
		}
		str++;
		d8 = pdata;
		i = mem_size;
		while (--i)
		{
			*d8++ = *str++;
		}
		str[0] = 0;

		break;//end case '\'':
	default:
		*pstatus = ' ';
		pdata = pmem->Malloc(pmem, info[2]);
		if (pdata == NULL)
		{
			strcpy(outstr, "Memory is exhausted\n");
			return NULL;
		}
		data_len_save = 1;

		//对应不同类型
		switch (info[0])
		{
		case 'c':
			d8 = pdata;
			*d8 = SD_Str2I(str) & 0XFF;

			break;
		case 'i':
			d16 = pdata;
			*d16 = SD_Str2I(str) & 0XFFFF;
			break;
		case 'l':
			d32 = pdata;
			*d32 = SD_Str2I(str);
			break;
		case 'f':
			df = pdata;
			*df = SD_Str2F(str);
			break;
		default:
			if (pmem->Free(pmem, pdata))
			{
				strcpy(outstr, "Type Unkown\n");
				return NULL;
			}
			else
			{
				strcpy(outstr, "Memory Free Error\n");
				return NULL;
			}
			break;
		}// end switch (info[0])

		break;
	}//end switch (str[0])

	*pdsize = mem_size;
	*pdlen = data_len_save;

	return pdata;
}

lw_u8 _StringDebugSliceDEF(lw_u8 *instr, lw_u8 *name, lw_u8 *pkind, lw_u8 *data)
{

	if (instr[0]!='#')
	{
		return 0;
	}
	instr++;
	_LW_STR_SKIP_COPY(name, instr, ' ');


	while (*instr++ == ' ');
	*pkind = instr[-1];
	if (instr[0]!='_')
	{
		return 2;
	}
	instr++;
	_LW_STR_SKIP_COPY(data,instr, 0);

	
	return 3;
}

lw_8 * StringDebug(OSlwToolHashSTU *ph, OSlwMemoryBasicSTU *pmem, lw_8 *instr, lw_8 *outstr, lw_8 *data1, lw_8 *data2)
{
	void *pdata;
	OSlwToolDListNodeSTU *pdln;
	void *pseldata;
	lw_u8 *info;
	lw_8 *pstr;
	lw_u8 *pstr2;
	lw_u32 mem_size = 0;
	lw_u32 dsize;
	void *pArgList[9];
	lw_u8 return_flag = 1;
	lw_8 i;
	lw_u8 len = 0;
	lw_8 kind;
	lw_8 format_kind[3]="%d";
	lw_u8 data_len;
	lw_u8 dlen;
	lw_u8 status;
	lw_u8 val_kind, val_index;
	lw_u8 data_index_kind;
	lw_u8 val_len;
	lw_u8 redata;
	lw_u8 arg_num;



	switch (instr[0])
	{
	//定义语句
	case '#':
		if (_StringDebugSliceDEF(instr, data1, &kind, data2) != 3)
		{
			strcpy(outstr, "cmd # is Illegal\n");
			break;
		}

		pseldata = _StringDebugFind(ph, data1, &val_kind, &val_index, &pdln,&data_index_kind);

		if (data_index_kind=='[')
		{
			strcpy(outstr, "Illegal [] In Name\n");
			return NULL;
		}

		info = _StringDebugSelectInfo(kind);
		
		if (info == NULL)
		{
			strcpy(outstr, "Unkown Type\n");
			return NULL;
		}

		pdata = _StringDebugString2Number(pmem, data2, info, outstr, &dsize, &dlen, &status);

		if (pdata==NULL)
		{
			return NULL;
		}

		if (pseldata==NULL)//原来没有该变量
		{
			pdln = pmem->Malloc(pmem, sizeof(OSlwToolDListNodeSTU));
			pstr = pmem->Malloc(pmem, strlen(data1) + 1);
			strcpy(pstr, data1);
			if (pdln && pstr)//
			{
				pdln->Data.pData = pdata;
				pdln->Data.uData = kind +((lw_u16)dlen<<8);
				ph->AppendFun(ph, pstr, pdln);//追加
				strcpy(outstr, "DEF data SUCCESS\n");
			}
			else
			{
				pmem->Free(pmem, pdata);
				strcpy(outstr, "Memory is exhausted\n");
			}


		}
		else//原来就有
		{
			if (pmem->Free(pmem, pseldata) == NULL)//无法释放 表示静态数据
			{
				pmem->Free(pmem, pdata);
				strcpy(outstr, "REDEF STC ERROR\n");
			}
			else
			{
				pdln->Data.pData = pdata;
				pdln->Data.uData = kind + ((lw_u16)dlen << 8);
				strcpy(outstr, "REDEF data SUCCESS\n");
			}
		}

		break;

	case '!':
		instr++;

		pseldata = _StringDebugFind(ph, instr, &val_kind, &val_index, &pdln, &data_index_kind);

		if (pseldata == NULL)
		{
			strcpy(outstr, "UNDEF value\n");
			break;
		}

		if (pmem->Free(pmem,pseldata))
		{
			ph->DeleteFun(ph, pdln->Key.pData);
			pmem->Free(pmem, pdln->Key.pData);
			pmem->Free(pmem, pdln);	
			strcpy(outstr, "DELETE value SUCCESS\n");
		}
		else
		{
			strcpy(outstr, "CANNOT DELETE value\n");
		}

		break;
	//更新语句
	case '=':
		if (_StringDebugSliceUPDATE(instr, data1, data2) != 2)
		{
			strcpy(outstr, "cmd = is Illegal\n");
			break;
		}

		pseldata = _StringDebugFind(ph, data1, &val_kind, &val_index, &pdln, &data_index_kind);

		if (pseldata==NULL)
		{
			strcpy(outstr, "UNDEF value\n");
			break;
		}

		val_len = (pdln->Data.uData >> 8) & 0xff;//得到变量实际长度

		info = _StringDebugSelectInfo(val_kind);

		pdata = _StringDebugString2Number(pmem, data2, info, outstr, &dsize, &dlen, &status);

		if (data_index_kind!='[' && status == ' ')//单变量 多赋值 既对变量里面每一个元素赋值
		{
			while (val_len--)
			{
				pseldata = _SD_MemCpy(pseldata, pdata, info[2]);
			}

		}
		else//普通赋值
		{
			lw_u32 copy_len;
			if (dlen + val_index >= val_len)
			{
				copy_len = (val_len - val_index)*info[2];
			}
			else
			{
				copy_len = (dlen)*info[2];
			}

			_SD_MemCpy(pseldata, pdata, copy_len);
		}

		strcpy(outstr, "UPDATE value SUCCESS\n");

		pmem->Free(pmem, pdata);

		break;

	//查询语句
	case '?':
		redata = _StringDebugSliceSELECT(instr, data1, &kind);
		if (redata == 0)
		{
			strcpy(outstr, "cmd ? is Illegal\n");
			break;
		}
		
		pseldata = _StringDebugFind(ph, data1, &val_kind, &val_index, &pdln, &data_index_kind);

		info = _StringDebugSelectInfo(val_kind);

		if (pseldata==NULL)
		{
			strcpy(outstr, "UNDEF value\n");
			break;
		}

		val_len = (pdln->Data.uData >> 8) & 0xff;//得到变量实际长度

		if (val_kind=='F')
		{
			SD_Sprintf(outstr, "FUN Arg %d", val_len);
			break;
		}

		if (redata==2)
		{
			format_kind[1] = kind;
		}
		else
		{
			format_kind[1] = info[1];
		}

		pstr = outstr;

		if (data_index_kind=='[')//单个查询
		{
			_SD_Data2Str(outstr, pseldata, format_kind, data2, info);
		}
		else
		{
			while (val_len--)
			{
				pstr = _SD_Data2Str(pstr, pseldata, format_kind, data2, info);
				pseldata = ((lw_u8 *)pseldata) + info[2];
			}
		}
		if (format_kind[1] == 'c')
		{
			pstr[-1] = '\n';
		}

		break;

	case '@':

		redata = _StringDebugSliceFUN(instr, data1, data2);

		if (redata == 0)
		{
			strcpy(outstr, "cmd @ is Illegal\n");
			break;
		}

		pseldata = _StringDebugFind(ph, data1, &val_kind, &val_index, &pdln, &data_index_kind);

		if (pseldata == NULL || val_kind != 'F')
		{
			strcpy(outstr, "UNDEF FUN\n");
			break;
		}

		val_len = (pdln->Data.uData >> 8) & 0xff;//得到函数实际长度

		if (redata==1)
		{
			arg_num = 0;
		}
		else
		{
			arg_num = _StringDebugFindCharNum(data2, ',') + 1;
		}

		if (arg_num!=val_len)
		{
			strcpy(outstr, "ARG NUM is ERROR\n");
			break;
		}
		
		if (arg_num!=0)
		{
			pstr2 = data2 + 1;
			for (i = 0; i < val_len - 1; i++)
			{
				_LW_STR_SKIP_FIND(pstr, pstr2, ',');
				pArgList[i] = _StringDebugFind(ph, pstr, &val_kind, &val_index, &pdln, &data_index_kind);

				if (pArgList[i]==NULL)
				{
					strcpy(outstr, "Illegal ARG\n");
					return NULL;
				}

			}

			_LW_STR_SKIP_FIND(pstr, pstr2, ')');
			pArgList[i] = _StringDebugFind(ph, pstr, &val_kind, &val_index, &pdln, &data_index_kind);

			if (pArgList[i] == NULL)
			{
				strcpy(outstr, "Illegal ARG\n");
				return NULL;
			}

		}



		switch (val_len)
		{
		case 0:
			((FunType_0)(pseldata))();
			break;

		case 1:
			((FunType_1)(pseldata))(pArgList[0]);
			break;

		case 2:
			((FunType_2)(pseldata))(pArgList[0], pArgList[1]);
			break;

		case 3:
			((FunType_3)(pseldata))(pArgList[0], pArgList[1], pArgList[2]);
			break;

		case 4:
			((FunType_4)(pseldata))(pArgList[0], pArgList[1], pArgList[2], pArgList[3]);
			break;

		case 5:
			((FunType_5)(pseldata))(pArgList[0], pArgList[1], pArgList[2], pArgList[3], pArgList[4]);
			break;

		case 6:
			((FunType_6)(pseldata))(pArgList[0], pArgList[1], pArgList[2], pArgList[3], pArgList[4], pArgList[5]);
			break;

		case 7:
			((FunType_7)(pseldata))(pArgList[0], pArgList[1], pArgList[2], pArgList[3], pArgList[4], pArgList[5], pArgList[6]);
			break;

		case 8:
			((FunType_8)(pseldata))(pArgList[0], pArgList[1], pArgList[2], pArgList[3], pArgList[4], pArgList[5], pArgList[6], pArgList[7]);
			break;

		case 9:
			((FunType_9)(pseldata))(pArgList[0], pArgList[1], pArgList[2], pArgList[3], pArgList[4], pArgList[5], pArgList[6], pArgList[7], pArgList[8]);
			break;
		default:
			strcpy(outstr, "ARG MORE\n");
			break;
		}
	
		strcpy(outstr, "EXE FUN SUCCESS\n");

		break;

	case '.':
		switch (instr[1])
		{
		case 'I':
			if (instr[2]=='(')//具体查找
			{
				instr += 3;
				_LW_STR_SKIP_COPY(data1, instr, ')');
				pseldata = _StringDebugFind(ph, data1, &val_kind, &val_index, &pdln, &data_index_kind);

				if (pseldata == NULL)
				{
					strcpy(outstr, "UNDEF value\n");
					break;
				}

				outstr = _SD_HashNode2Str(outstr, pdln);

			}
			else
			{
				for ( i = 0; i < ph->HashListLength; i++)
				{
					pdln = ph->pHashList[i];

					while (pdln)
					{
						outstr = _SD_HashNode2Str(outstr, pdln);
						pdln = (void *)pdln->con.pNext;
					}
				}


			}
			break;


		case 'M':
			SD_Sprintf(outstr, "MEM surplus %f %%\n", _ParaFrom(pmem->memSurplus) / _ParaFrom(pmem->memMax));
			break;

		case 'V':
			SD_Sprintf(outstr, "Ver = %s\n", STRING_DEBUG_VER);
			break;

		default:
			strcpy(outstr, "cmd . is Illegal\n");
			break;
		}

		break;


	default:
		strcpy(outstr, "Cmd is unknow\n");
		break;
	}

	if (return_flag)
	{
		return outstr;
	}
	else
	{
		return NULL;
	}
}

#elif OSLW_STR_DBG_SIMPLE_MODE==1

//格式字符串转换为数组
void *_StringDebugString2NumberSimple(void *pmem, lw_u8 *str, lw_u8 *info,lw_u8 *outstr)
{
	
	lw_u8 *d8;
	lw_u16 *d16;
	lw_u32 *d32;
	ParaType *df;
	
	//对应不同类型
	switch (info[0])
	{
	case 'c':
	d8 = pmem;
	*d8 = SD_Str2I(str) & 0XFF;

	break;
	case 'i':
	d16 = pmem;
	*d16 = SD_Str2I(str) & 0XFFFF;
	break;
	case 'l':
	d32 = pmem;
	*d32 = SD_Str2I(str);
	break;
	case 'f':
	df = pmem;
	*df = SD_Str2F(str);
	break;
	default:

	strcpy(outstr, "Type Unkown\n");
	return NULL;

	break;
	}// end switch (info[0])
	
	return pmem;
	
}

lw_8 * StringDebug(OSlwToolHashSTU *ph, void *pm, lw_8 *instr, lw_8 *outstr, lw_8 *data1, lw_8 *data2)
{
	void *pdata;
	OSlwToolDListNodeSTU *pdln;
	void *pseldata;
	lw_u8 *info;
	lw_8 *pstr;
	lw_u8 *pstr2;
	lw_u32 mem_size = 0;
	lw_u32 dsize;
	void *pArgList[3];
	lw_u8 return_flag = 1;
	lw_8 i;
	lw_u8 len = 0;
	lw_8 kind;
	lw_8 format_kind[3]="%d";
	lw_u8 data_len;
	lw_u8 dlen;
	lw_u8 status;
	lw_u8 val_kind, val_index;
	lw_u8 data_index_kind;
	lw_u8 val_len;
	lw_u8 redata;
	lw_u8 arg_num;



	switch (instr[0])
	{
		
	//更新语句
	case '=':
		if (_StringDebugSliceUPDATE(instr, data1, data2) != 2)
		{
			strcpy(outstr, "cmd = is Illegal\n");
			break;
		}

		pseldata = _StringDebugFind(ph, data1, &val_kind, &val_index, &pdln, &data_index_kind);

		if (pseldata==NULL)
		{
			strcpy(outstr, "UNDEF value\n");
			break;
		}

		val_len = (pdln->Data.uData >> 8) & 0xff;//得到变量实际长度

		info = _StringDebugSelectInfo(val_kind);

		pdata = _StringDebugString2NumberSimple(pm, data2, info, outstr);

		if (data_index_kind!='[')//单变量 多赋值 既对变量里面每一个元素赋值
		{
			while (val_len--)
			{
				pseldata = _SD_MemCpy(pseldata, pdata, info[2]);
			}

		}
		else//普通赋值
		{
			_SD_MemCpy(pseldata, pdata, info[2]);
		}

		strcpy(outstr, "UPDATE value SUCCESS\n");

		
		break;

	//查询语句
	case '?':
		redata = _StringDebugSliceSELECT(instr, data1, &kind);
		if (redata == 0)
		{
			strcpy(outstr, "cmd ? is Illegal\n");
			break;
		}
		
		pseldata = _StringDebugFind(ph, data1, &val_kind, &val_index, &pdln, &data_index_kind);

		info = _StringDebugSelectInfo(val_kind);

		if (pseldata==NULL)
		{
			strcpy(outstr, "UNDEF value\n");
			break;
		}

		val_len = (pdln->Data.uData >> 8) & 0xff;//得到变量实际长度

		if (val_kind=='F')
		{
			SD_Sprintf(outstr, "FUN Arg %d", val_len);
			break;
		}

		if (redata==2)
		{
			format_kind[1] = kind;
		}
		else
		{
			format_kind[1] = info[1];
		}

		pstr = outstr;

		if (data_index_kind=='[')//单个查询
		{
			_SD_Data2Str(outstr, pseldata, format_kind, data2, info);
		}
		else
		{
			while (val_len--)
			{
				pstr = _SD_Data2Str(pstr, pseldata, format_kind, data2, info);
				pseldata = ((lw_u8 *)pseldata) + info[2];
			}
		}
		if (format_kind[1] == 'c')
		{
			pstr[-1] = '\n';
		}

		break;

	case '@':

		redata = _StringDebugSliceFUN(instr, data1, data2);

		if (redata == 0)
		{
			strcpy(outstr, "cmd @ is Illegal\n");
			break;
		}

		pseldata = _StringDebugFind(ph, data1, &val_kind, &val_index, &pdln, &data_index_kind);

		if (pseldata == NULL || val_kind != 'F')
		{
			strcpy(outstr, "UNDEF FUN\n");
			break;
		}

		val_len = (pdln->Data.uData >> 8) & 0xff;//得到函数实际长度

		if (redata==1)
		{
			arg_num = 0;
		}
		else
		{
			arg_num = _StringDebugFindCharNum(data2, ',') + 1;
		}

		if (arg_num!=val_len)
		{
			strcpy(outstr, "ARG NUM is ERROR\n");
			break;
		}
		
		if (arg_num!=0)
		{
			pstr2 = data2 + 1;
			for (i = 0; i < val_len - 1; i++)
			{
				_LW_STR_SKIP_FIND(pstr, pstr2, ',');
				pArgList[i] = _StringDebugFind(ph, pstr, &val_kind, &val_index, &pdln, &data_index_kind);

				if (pArgList[i]==NULL)
				{
					strcpy(outstr, "Illegal ARG\n");
					return NULL;
				}

			}

			_LW_STR_SKIP_FIND(pstr, pstr2, ')');
			pArgList[i] = _StringDebugFind(ph, pstr, &val_kind, &val_index, &pdln, &data_index_kind);

			if (pArgList[i] == NULL)
			{
				strcpy(outstr, "Illegal ARG\n");
				return NULL;
			}

		}



		switch (val_len)
		{
		case 0:
			((FunType_0)(pseldata))();
			break;

		case 1:
			((FunType_1)(pseldata))(pArgList[0]);
			break;

		case 2:
			((FunType_2)(pseldata))(pArgList[0], pArgList[1]);
			break;

		case 3:
			((FunType_3)(pseldata))(pArgList[0], pArgList[1], pArgList[2]);
			break;


		default:
			strcpy(outstr, "ARG MORE\n");
			break;
		}
	
		strcpy(outstr, "EXE FUN SUCCESS\n");

		break;

	case '.':
		switch (instr[1])
		{
		case 'I':
			if (instr[2]=='(')//具体查找
			{
				instr += 3;
				_LW_STR_SKIP_COPY(data1, instr, ')');
				pseldata = _StringDebugFind(ph, data1, &val_kind, &val_index, &pdln, &data_index_kind);

				if (pseldata == NULL)
				{
					strcpy(outstr, "UNDEF value\n");
					break;
				}

				outstr = _SD_HashNode2Str(outstr, pdln);

			}
			else
			{
				for ( i = 0; i < ph->HashListLength; i++)
				{
					pdln = ph->pHashList[i];

					while (pdln)
					{
						outstr = _SD_HashNode2Str(outstr, pdln);
						pdln = (void *)pdln->con.pNext;
					}
				}


			}
			break;



		case 'V':
			SD_Sprintf(outstr, "Ver = %s\n", STRING_DEBUG_VER);
			break;

		default:
			strcpy(outstr, "cmd . is Illegal\n");
			break;
		}

		break;


	default:
		strcpy(outstr, "Cmd is unknow\n");
		break;
	}

	if (return_flag)
	{
		return outstr;
	}
	else
	{
		return NULL;
	}
}

#elif OSLW_STR_DBG_SIMPLE_MODE == 2


void * _StringDebugFind_S2(OSlwStrDbgSTU *pSD,lw_8 *name, lw_8 *pkind)
{
	lw_u16 i=0;
	
	for(i=0;i<pSD->count;i++)
	{
		if(strcmp(name,pSD->Data[i].name)==0)
			break;
	}
	
	if(i>=pSD->count)
	{
		return NULL;
	}
	
	*pkind=pSD->Data[i].kind;
	
	return pSD->Data[i].pdata;
	
}


lw_8 * StringDebug(OSlwStrDbgSTU *pSD, lw_8 *instr, lw_8 *outstr, lw_8 *data1, lw_8 *data2)
{
	void *pdata;
	void *pseldata;
	lw_u8 *info;
	lw_8 *pstr;
	lw_u8 *pstr2;
	lw_8 kind;
	lw_8 format_kind[3]="%d";
	lw_u8 val_kind;
	lw_u8 redata;
	lw_32 trans_data;

	switch (instr[0])
	{
		
	//更新语句
	case '=':
		if (_StringDebugSliceUPDATE(instr, data1, data2) != 2)
		{
			strcpy(outstr, "cmd = is Illegal\n");
			break;
		}

		pseldata = _StringDebugFind_S2(pSD, data1, &val_kind);

		if (pseldata==NULL)
		{
			strcpy(outstr, "UNDEF value\n");
			break;
		}


		info = _StringDebugSelectInfo(val_kind);

		trans_data = SD_Str2I(data2);

		_SD_MemCpy(pseldata, &trans_data, info[2]);
		
		strcpy(outstr, "UPDATE value SUCCESS\n");

		
		break;

	//查询语句
	case '?':
		strcpy(data1,instr+1);
		
		pseldata = _StringDebugFind_S2(pSD, data1, &val_kind);

		if (pseldata==NULL)
		{
			strcpy(outstr, "UNDEF value\n");
			break;
		}
		
		info = _StringDebugSelectInfo(val_kind);
		

		if (val_kind=='F')
		{
			strcpy(outstr, "FUN");
			break;
		}


		pstr = outstr;
		switch (info[0])
		{
			case 'c':
				OSlwToolString_itoa(*((lw_8 *)pseldata),outstr,10);
				break;
			
			case 'i':
				OSlwToolString_itoa(*((lw_16 *)pseldata),outstr,10);
				break;
			case 'l':
				OSlwToolString_itoa(*((lw_32 *)pseldata),outstr,10);
				break;

			default:
				break;
		}
	
		break;

	case '@':

		strcpy(data1,instr+1);
		
		pseldata = _StringDebugFind_S2(pSD, data1, &val_kind);

		if (pseldata == NULL || val_kind != 'F')
		{
			strcpy(outstr, "UNDEF FUN\n");
			break;
		}


		((FunType_0)(pseldata))();
		strcpy(outstr, "EXE FUN SUCCESS\n");

		break;

	case '.':
		switch (instr[1])
		{

		case 'V':
			strcpy(outstr,STRING_DEBUG_VER);
			break;

		default:
			strcpy(outstr, "cmd . is Illegal\n");
			break;
		}

		break;


	default:
		strcpy(outstr, "Cmd is unknow\n");
		break;
	}


		return outstr;
	
}


#else



#endif //OSLW_STR_DBG_SIMPLE_MODE==0




#if OSLW_OS_ENABLE
lw_u8 * OSlwStrDbgFun(struct OSLW_CORE_STRUCT *pOS, lw_u8 *instr)
{
	
	OSlwStrDbgRun(&(pOS->StrDbg),instr);
	return pOS->StrDbg.StrDbgAllStr.OutStr;
	
}
#endif //OSLW_OS_ENABLE


#endif