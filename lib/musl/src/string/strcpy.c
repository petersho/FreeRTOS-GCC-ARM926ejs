#include <string.h>

//char *__stpcpy(char *, const char *);
char *stpcpy(char *dest, const char *src)
{
	const unsigned char *s = (unsigned char *)src;
	unsigned char *d = (unsigned char *)dest;

	while ((*d++ = *s++));
	return dest;
}

char *strcpy(char *dest, const char *src)
{
	const unsigned char *s = (unsigned char *)src;
	unsigned char *d = (unsigned char *)dest;

	while ((*d++ = *s++));
	return dest;

}
