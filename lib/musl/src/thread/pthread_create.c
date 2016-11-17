#include <pthread.h>
#include <FreeRTOS.h>
#include <task.h>

int pthread_create(pthread_t *res, const pthread_attr_t *attrp, void *(*entry)(void *), void *arg)
{
	struct pthread *pd = NULL;

	xTaskCreate((TaskFunction_t)entry, "pthread", 128, NULL, 10, NULL);

	
	*res = (pthread_t)pd;

	return 0;
}

