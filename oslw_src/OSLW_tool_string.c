/*(Ver.=0.9~)(Beg.=0.9)
* OSLW_tool_string.c
*
*  Created on: 2018-08-22
*      Author: slw
*/

#include "OSLW_include.h"
#if !(OSLW_SIMPLE_LEVEL >= 4)



ParaType OSlwToolStringToParaType(const lw_u8 *str)
{
	ParaType s = _ParaFrom(0.0);
	ParaType d = _ParaFrom(10.0);
	lw_u32 jishu = 0;

	lw_u8 falg = 0;

	OSLW_assert(!(str));

	while (*str == ' ')
	{
		str++;
	}

	if (*str == '-')//记录数字正负
	{
		falg = 1;
		str++;
	}

	if (!(*str >= '0'&&*str <= '9'))//如果一开始非数字则退出，返回0.0
		return s;

	while (*str >= '0'&&*str <= '9'&&*str != '.')//计算小数点前整数部分
	{
		s = s*10.0 + *str - '0';
		str++;
	}

	if (*str == '.')//以后为小数部分
		str++;

	while (*str >= '0'&&*str <= '9')//计算小数部分
	{
		s = s + (*str - '0') / d;
		d *= 10.0;
		str++;
	}

	if (*str == 'e' || *str == 'E')//考虑科学计数法
	{
		str++;
		if (*str == '+')
		{
			str++;
			while (*str >= '0'&&*str <= '9')
			{
				jishu = jishu * 10 + *str - '0';
				str++;
			}
			while (jishu > 0)
			{
				s *= 10;
				jishu--;
			}
		}
		if (*str == '-')
		{
			str++;
			while (*str >= '0'&&*str <= '9')
			{
				jishu = jishu * 10 + *str - '0';
				str++;
			}
			while (jishu > 0)
			{
				s /= 10;
				jishu--;
			}
		}
	}

	return s*(falg ? -1.0 : 1.0);
}


lw_32 OSlwToolStringToInt(const lw_u8 *str)
{
	lw_32 Ret_Integer = 0;
	lw_u8 Integer_sign = 0;

	OSLW_assert(!(str));

	while (*str == ' ')
	{
		str++;
	}


	if (*str == '-')
	{
		Integer_sign = 1;
	}

	if (*str == '-' || *str == '+')
	{
		str++;
	}

	while (*str >= '0' && *str <= '9')
	{
		Ret_Integer = Ret_Integer * 10 + *str - '0';
		str++;
	}
	

	return Integer_sign ?  -Ret_Integer : Ret_Integer;
}


static const lw_u8 _str_list[] = "0123456789abcdef";

lw_u8* OSlwToolString_itoa(lw_32 value, lw_u8 *str, lw_32 radix) 
{
	lw_8 reverse[20];
	lw_8 *p = reverse;
	lw_32 sign = (value >= 0) ? 1 : 0;

	value = (value >= 0) ? value : -value;
	*p++ = '\0';
	while (value >= 0) {
		*p++ = _str_list[value%radix];
		value /= radix;
		if (value == 0) break;
	}

	if (!sign) {
		*p = '-';
	}
	else {
		p--;
	}

	while (p >= reverse) {
		*str++ = *p--;
	}

	return str;
}


lw_u8* OSlwToolString_utoa(lw_u32 value, lw_u8 *str, lw_32 radix) 
{
	lw_8 reverse[20];
	lw_8 *p = reverse;

	*p++ = '\0';
	while (value != 0) {
		*p++ = _str_list[value%radix];
		value /= radix;
		if (value == 0) break;
	}
	p--;

	while (p >= reverse) {
		*str++ = *p--;
	}

	return str;
}


lw_u8* OSlwToolString_gcvt(ParaType value, lw_32 ndigit, lw_u8 *buf) 
{
	lw_8 tmpbuf[40];
	lw_32 int_part = (int)value;

	ParaType folat_part = value - int_part;
	lw_8 *p;

	if (folat_part < 0) folat_part = -folat_part;
	OSlwToolString_itoa(int_part, tmpbuf, 10);

	p = tmpbuf;

	while (*p != '\0') p++;

	*p++ = '.';

	while (ndigit > 0 && folat_part > 0.0000001) {
		*p++ = (int)(folat_part * 10) + '0';
		folat_part = (folat_part * 10) - (int)(folat_part * 10);
		ndigit--;
	}

	*p = '\0';
	strcpy(buf, tmpbuf);
	return buf;
}

void* OSlwToolStringVSprintf(lw_8 *buf, const lw_8 *fmt, va_list args)
{
	lw_8 *p;
	va_list p_next_arg = args;

	for (p = buf; *fmt; fmt++) {
		if (*fmt != '%') {
			*p++ = *fmt;
			continue;
		}
		fmt++;  // *fmt = '%'
		switch (*fmt) {
		case 'd':
			OSlwToolString_itoa(va_arg(p_next_arg, int), p, 10);
			p += strlen(p);
			break;
		case 'x':
			OSlwToolString_utoa(va_arg(p_next_arg, unsigned int), p, 16);
			p += strlen(p);
			break;
		case 'c':
			*p++ = va_arg(p_next_arg, char);
			break;
		case 's':
			*p = '\0';
			strcat(p, va_arg(p_next_arg, char *));
			p += strlen(p);
			break;
		case 'f':
			OSlwToolString_gcvt(va_arg(p_next_arg, double), 6, p);
			p += strlen(p);
			break;
		default:
			break;
		}
	}
	*p = '\0';
	return p;
}

void* OSlwToolStringSprintf(lw_8 *buf, const lw_8 *fmt, ...)
{
	lw_u8 *p;
	va_list args;
	va_start(args, fmt);

	p=OSlwToolStringVSprintf(buf, fmt, args);

	va_end(args);
	return p;
}


#endif // !(OSLW_SIMPLE_LEVEL >= 3)
