#ifndef _CTYPE_H
#define _CTYPE_H

int isalnum(int);
int isalpha(int);
int isblank(int);
int iscntrl(int);
int isdigit(int);
int isgraph(int);
int islower(int);
int isprint(int);
int ispunct(int);
int isspace(int);
int isupper(int);
int isxdigit(int);
int tolower(int);
int toupper(int);

static __inline int __isspace(int _c)
{
	return _c == ' ' || (unsigned)_c-'\t' < 5;
}

#define isspace(a) __isspace(a)

#endif
