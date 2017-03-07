#ifndef _ALLTYPES_H
#define _ALLTYPES_H

#define _Addr int
#define _Int64 long long
#define _Reg int

typedef __builtin_va_list va_list;

typedef unsigned _Addr size_t;

typedef signed char	int8_t;
typedef short		int16_t;
typedef int		int32_t;
typedef _Int64		int64_t;
typedef int		int32_t;
typedef unsigned char	uint8_t;
typedef unsigned short	uint16_t;
typedef unsigned int	uint32_t;
typedef unsigned _Int64 uint64_t;

typedef unsigned _Addr uintptr_t;

typedef unsigned long pthread_t;
typedef struct { union { int __i[9]; volatile int __vi[9]; unsigned __s[9]; } __u; } pthread_attr_t;
typedef struct { union { int __i[6]; volatile int __vi[6]; volatile void *volatile __p[6]; } __u; } pthread_mutex_t;

#endif
