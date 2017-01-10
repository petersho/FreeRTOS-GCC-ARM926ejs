#include <pthread.h>
#include <FreeRTOS.h>
#include <task.h>

int pthread_create(pthread_t *res, const pthread_attr_t *attrp, void *(*entry)(void *), void *arg)
{
	TaskHandle_t xHandle = NULL;
	BaseType_t xReturned = 0;
	UBaseType_t priority = 0;
	int ret = 0;


	priority = uxTaskPriorityGet(NULL);

	xReturned = xTaskCreate((TaskFunction_t)entry, "pthread", 128, NULL, priority, &xHandle);

	if (xReturned == pdPASS)
		ret = 0;

	*res = xHandle;

	return ret;
}

