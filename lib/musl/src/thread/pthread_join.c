#include <pthread.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

int pthread_join(pthread_t t, void **res)
{
	TaskHandle_t xHandle = NULL;
	SemaphoreHandle_t xSemaphore = NULL;

	xHandle = (TaskHandle_t)t;
	xSemaphore = xSemaphoreCreateBinary();

	vTaskpThreadJoinSemaSet(xHandle, xSemaphore);

	if (xSemaphoreTake(xSemaphore, portMAX_DELAY) ) {
		/* Got Semaphore and then release semaphore */
		vSemaphoreDelete(xSemaphore);
	}

        return 0;
}

