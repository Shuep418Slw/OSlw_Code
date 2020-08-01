/*(Ver.=0.97)
* OSLW_tool_string.c
*
*  Created on: 2018-08-22
*      Author: slw
*/

#include "OSLW_include.h"
#if !(OSLW_SIMPLE_LEVEL >= 4)



lw_sf OSlwToolStringTolw_sf(const lw_u8 *str)
{
	lw_sf s = _ParaFrom(0.0);
	lw_sf d = _ParaFrom(10.0);
	lw_u32 jishu = 0;

	lw_u8 falg = 0;

	OSLW_assert(!(str));

	while (*str == ' ')
	{
		str++;
	}

	if (*str == '-')//��¼��������
	{
		falg = 1;
		str++;
	}

	if (!(*str >= '0'&&*str <= '9'))//���һ��ʼ���������˳�������0.0
		return s;

	while (*str >= '0'&&*str <= '9'&&*str != '.')//����С����ǰ��������
	{
		s = s*10.0f + *str - '0';
		str++;
	}

	if (*str == '.')//�Ժ�ΪС������
		str++;

	while (*str >= '0'&&*str <= '9')//����С������
	{
		s = s + (*str - '0') / d;
		d *= 10.0f;
		str++;
	}

	if (*str == 'e' || *str == 'E')//���ǿ�ѧ������
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

	return s*(falg ? -1.0f : 1.0f);
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


lw_u8* OSlwToolString_gcvt(lw_sf value, lw_32 ndigit, lw_u8 *buf) 
{
	lw_8 tmpbuf[40];
	lw_32 int_part = (int)value;

	lw_sf folat_part = value - int_part;
	lw_8 *p=tmpbuf;

	if(value<0)
	{
	    *p++='-';
	    int_part=-int_part;
	}

	if (folat_part < 0)
	{
	    folat_part = -folat_part;
	}
	OSlwToolString_itoa(int_part, (lw_u8*)p, 10);

	while (*p != '\0') p++;

	*p++ = '.';

	while (ndigit > 0 && folat_part > 0.0000001f) {
		*p++ = (int)(folat_part * 10) + '0';
		folat_part = (folat_part * 10) - (int)(folat_part * 10);
		ndigit--;
	}

	*p = '\0';
	strcpy((void *)buf, (void *)tmpbuf);
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
			OSlwToolString_itoa(va_arg(p_next_arg, int), (lw_u8*)p, 10);
			p += strlen(p);
			break;
		case 'x':
			OSlwToolString_utoa(va_arg(p_next_arg, unsigned int), (lw_u8*)p, 16);
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
			OSlwToolString_gcvt(va_arg(p_next_arg, lw_sf), 6, (lw_u8*)p);
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
