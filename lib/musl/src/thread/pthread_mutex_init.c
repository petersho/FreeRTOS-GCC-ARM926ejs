#include <pthread.h>
#include <FreeRTOS.h>
#include <semphr.h>

int pthread_mutex_init(pthread_mutex_t *restrict m, const pthread_mutexattr_t *restrict a)
{
	int ret = 0;

	*m = xSemaphoreCreateMutex();

	if( *m != NULL ) {
		ret = 0;
	} else {
		ret = -1;
	}

	return ret;
}

int pthread_mutex_lock(pthread_mutex_t *m)
{

	xSemaphoreTake(*m, portMAX_DELAY );

	return 0;
#if 0
        if ((m->_m_type&15) == PTHREAD_MUTEX_NORMAL
            && !a_cas(&m->_m_lock, 0, EBUSY))
                return 0;

        return __pthread_mutex_timedlock(m, 0);
#endif
}

int pthread_mutex_unlock(pthread_mutex_t *m)
{
	xSemaphoreGive(*m);

	return 0;
}

int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
	vSemaphoreDelete(*mutex);

        return 0;
}
