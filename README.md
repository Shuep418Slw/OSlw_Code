# OSlw_Code
移植步骤

0)添加文件与路径

1)修改宏定义

2)实例化内核
~~~
 OSlwCoreSTU myos;
~~~
3)定时器回调函数
~~~
OSlwClockCallBack(&myos);
~~~
4)实例任务
~~~
OSlwTaskSTU mytask;
~~~
5)内核任务函数
~~~
void Mytask(OSlwTaskSTU *_pta)
{
	static OSlwTaskSTU *pta; 
	static lw_u16 num=0;
	pta=_pta;
	OSlwJiJi(_pta);
	for(;;)	
	{
		OSlwSleepSec(pta,1,0);
	}
	OSlwWeiJi(_pta);
}
~~~
6)初始化内核 追加任务
~~~
OSlwCoreInitial(&myos);
OSlwTaskInit(&mytask,3,"T1",0,NULL,Mytask);
myos.TaskAppendFun(&myos,&mytask);
~~~

7)启动os
~~~
myos.StartFun(&myos);
~~~
