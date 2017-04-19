#ifndef _PTHREAD_H
#define _PTHREAD_H

#include <bits/alltypes.h>

int pthread_create(pthread_t *res, const pthread_attr_t *attrp, void *(*entry)(void *), void *arg);
int pthread_join(pthread_t t, void **res);

#endif
