#include <pthread.h>
#include <FreeRTOS.h>
#include <task.h>

int pthread_create(pthread_t *res, const pthread_attr_t *attrp, void *(*entry)(void *), void *arg)
{
	TaskHandle_t xHandle = NULL;
	BaseType_t xReturned = 0;
	int ret = 0;

	xReturned = xTaskCreate((TaskFunction_t)entry, "pthread", 128, NULL, 10, &xHandle);

	if (xReturned == pdPASS)
		ret = 0;

	*res = xHandle;

	return ret;
}

