/*
 * osal.c
 *
 * Created: 2019/11/30 15:55:02
 *  Author: WJY
 * Version: 1.0
 */ 

#include "osal.h"

#define TASK_MAX	16


//*****************function select********************//
//#define SYS_MONITOR		1			//enable the system monitor to estimate the cpu usage.
#define RATIO_BUSY_IDLE	40

//#define COMPLETE_MODE		1			//enable the idle task and always task

//*****************global variable********************//
#ifdef SYS_MONITOR
unsigned int idleCnt, alwaysCnt;
float utility;
#endif

unsigned char tasksCnt;
unsigned char *pTasksEvents[TASK_MAX];
tasksArrHandle tasksArr[TASK_MAX];
pTaskHandle idleTask, alwaysTask;

void idleSys(void)
{
	
};

void os_init(void)
{
	tasksCnt = 0;
	os_registerIdleTask(idleSys);
	os_registerAlwaysTask(idleSys);
}

unsigned char os_registerTask(tasksArrHandle pFunc, unsigned char *events)
{
	tasksArr[tasksCnt] = pFunc;
	pTasksEvents[tasksCnt] = events;
	tasksCnt++;
	return (tasksCnt-1);
}
void os_registerIdleTask(tasksArrHandle pFunc)
{
	idleTask = pFunc;
}
void os_registerAlwaysTask(tasksArrHandle pFunc)
{
	alwaysTask = pFunc;
}


void os_start(void)
{
	unsigned char idx;
	unsigned char events;
	
	while(1)
	{
		idx = 0;
//********check events*********//
		do{
			if (*(pTasksEvents[idx]))  // Task is highest priority that is ready.
				break;
		}while (++idx < tasksCnt);
		
//********run task*********//
		if(idx < tasksCnt)
		{
			events = *(pTasksEvents[idx]);	//read the event
			*(pTasksEvents[idx]) = 0;		//clear the event
			events = (tasksArr[idx])(idx, events);		//process the task
			*(pTasksEvents[idx]) |= events;  // Add back unprocessed events to the current task.
		}
		else	//Complete pass through all task events with no activity?
		{
		//********idle task*********//
#ifdef COMPLETE_MODE
			(idleTask)();
#endif
#ifdef SYS_MONITOR
			idleCnt++;
#endif
		}
		
//********always running task*********//
#ifdef COMPLETE_MODE
		(alwaysTask)();
#endif
#ifdef SYS_MONITOR
		alwaysCnt++;
		if(alwaysCnt==65535)
		{			
			utility = RATIO_BUSY_IDLE/(RATIO_BUSY_IDLE + 1 +(float)alwaysCnt/(alwaysCnt-idleCnt));
			alwaysCnt = 0;
			idleCnt = 0;
		}
#endif
	}
	
}

