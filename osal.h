/*
 * osal.h
 *
 * Created: 2019/11/30 15:55:17
 *  Author: WJY
 * Version: 1.0
 */ 


#ifndef OSAL_H_
#define OSAL_H_


typedef unsigned char (*tasksArrHandle) (char idx, unsigned char events);
typedef void (*pTaskHandle) (void);


void os_init(void);
unsigned char os_registerTask(tasksArrHandle pFunc, unsigned char *events);
void os_registerIdleTask(tasksArrHandle pFunc);
void os_registerAlwaysTask(tasksArrHandle pFunc);
void os_start(void);

//task example
/*
#define MES_ERR		0x01
#define MES_DATA	0x02
unsigned char function0(char idx, unsigned char events)
{
	if(events & MES_ERR)
	{
		//process
		//...
		events &= (~MES_ERR);
	}
	else if(events & MES_DATA)
	{
		//process
		//...
		events &= (~MES_DATA);
	}
	return events;
}
*/


#endif /* OSAL_H_ */