# OSlw_Code
## OS移植步骤

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


##搭建神经网络

1)新建神经网络结构体
~~~
OSlwToolBPnnSTU mynn;
~~~

2)定义内存堆
~~~
OSLW_MEM_SIMPLE_DEF(mem, 32, 1500)
~~~

3)定义随机数用以初始化
~~~
OSlwToolRandomWELL512STU myrand;
OSlwToolRandomBasicSTU *pr = &myrand;
~~~

4)初始化内存堆与随机数
~~~
OSLW_MEM_INIT(mem);
OSlwToolRandomWELL512Seed(&myrand, 1);
~~~

5)初始化神经网络 并且定义神经网络最大mini-batch
~~~
OSlwToolBPnnInit(&mynn, 1);
~~~

6)追加神经网络层
~~~
OSlwToolBPnnFullConAppend(
	&mynn,
	784, 10,
	NULL, NULL,
	NULL, NULL,
	pr->randn, pr, _ParaFrom(0), _ParaFrom(0.1),
	LwSoftMax, pmem
);
~~~

7)追加损失层
~~~
OSlwToolBPnnTrainInit(&mynn, NULL, pmem, pOSlwToolMatrixLossCrossEntropyForSoftMax, _ParaFrom(0.1));
~~~

8)初始化所有数据
~~~
OSlwToolBPnnAllDataInit(&mynn, pmem);
~~~
