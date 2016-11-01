#include <string.h>

char *__stpcpy(char *, const char *);

char *strcpy(char *dest, const char *src)
{
#if 0
	__stpcpy(dest, src);
	return dest;
#else
	const unsigned char *s = (unsigned char *)src;
	unsigned char *d = (unsigned char *)dest;
	while ((*d++ = *s++));
	return dest;
#endif
}
