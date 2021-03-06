/*
Copyright 2013, Jernej Kovacic

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/**
 * @file
 * Implementation of functions that perform printing messages to a UART
 *
 * @author Jernej Kovacic
 */

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <stdarg.h>

#include "app_config.h"
#include "bsp.h"
#include "uart.h"



/*
 * A string buffer is necessary for printing individual characters:
 * (1) The gate keeper tasks accepts pointers to strings only. Hence a character will
 *     be placed into a short string, its first character will be the actual character,
 *     followed by '\0'.
 * (2) Corruptions must be prevented when several tasks call vPrintChar simultaneously.
 *     To accomplish this, the buffer will consist of several strings, the optimal number
 *     depends on the application.
 */

/* The number of actual strings for the buffer has been defined in "app_config.h" */

/* Length of one buffer string, one byte for the character, the other one for '\0' */
#define CHR_BUF_STRING_LEN      ( 2 )

/* Allocate the buffer for printing individual characters */
static portCHAR printChBuf[ PRINT_CHR_BUF_SIZE ][ CHR_BUF_STRING_LEN ];

/* Position of the currently available "slot" in the buffer */
static uint16_t chBufCntr = 0;



/* UART number: */
static uint8_t printUartNr = (uint8_t) -1;

/* Messages to be printed will be pushed to this queue */
static QueueHandle_t printQueue;



/**
 * Initializes all print related tasks and synchronization primitives.
 * This function must be called before anything is attempted to be printed
 * via vPrintMsg or vPrintChar!
 *
 * @param uart_nr - number of the UART
 *
 * @return pdPASS if initialization is successful, pdFAIL otherwise
 */
int16_t printInit(uint8_t uart_nr)
{
    uint16_t i;

    /*
     * Initialize the character print buffer.
     * It is sufficient to set each string's second character to '\0'.
     */
    for ( i=0; i<PRINT_CHR_BUF_SIZE; ++i )
    {
        printChBuf[i][1] = '\0';
    }

    chBufCntr = 0;

    /* Check if UART number is valid */
    if ( uart_nr >= BSP_NR_UARTS )
    {
        return pdFAIL;
    }

    printUartNr = uart_nr;

    /* Create and assert a queue for the gate keeper task */
    printQueue = xQueueCreate(PRINT_QUEUE_SIZE, sizeof(portCHAR*));
    if ( 0 == printQueue )
    {
        return pdFAIL;
    }

    /* Enable the UART for transmission */
    uart_enableTx(printUartNr);

    return pdPASS;
}


/**
 * A gate keeper task that waits for messages to appear in the print queue and
 * prints them. This prevents corruption of printed messages if a task that
 * actually attempts to print, is preempted.
 *
 + @param params - ignored
 */
void printGateKeeperTask(void* params)
{
    portCHAR* message;

    for ( ; ; )
    {
        /* The task is blocked until something appears in the queue */
        xQueueReceive(printQueue, (void*) &message, portMAX_DELAY);
        /* Print the message in the queue */
        uart_print(printUartNr, message);
    }

    /* if it ever breaks out of the infinite loop... */
    vTaskDelete(NULL);

    /* suppress a warning since 'params' is ignored */
    (void) params;
}


/**
 * Prints a message in a thread safe manner - even if the calling task is preempted,
 * the entire message will be printed.
 *
 * Nothing is printed if 'msg' equals NULL.
 *
 * @note This function may only be called when the FreeRTOS scheduler is running!
 *
 * @param msg - a message to be printed
 */
void vPrintMsg(const portCHAR* msg)
{
    if ( NULL != msg )
    {
        xQueueSendToBack(printQueue, (void*) &msg, 0);
    }
}


/**
 * Prints a character in a thread safe manner - even if the calling task preempts
 * another printing task, its message will not be corrupted. Additionally, if another
 * task attempts to print a character, the buffer will not be corrupted.
 *
 * @note This function may only be called when the FreeRTOS scheduler is running!
 *
 * @param ch - a character to be printed
 */
void vPrintChar(portCHAR ch)
{
    /*
     * If several tasks call this function "simultaneously", the buffer may get
     * corrupted. To prevent this, the buffer contains several strings
     */

    /*
     * Put 'ch' to the first character of the current buffer string,
     * note that the second character has been initialized to '\0'.
     */
    printChBuf[chBufCntr][0] = ch;

    /* Now the current buffer string may be sent to the printing queue */
    vPrintMsg(printChBuf[chBufCntr]);

    /*
     * Update chBufCntr and make sure it always
     * remains between 0 and CHR_PRINT_BUF_SIZE-1
     */
    ++chBufCntr;
    chBufCntr %= PRINT_CHR_BUF_SIZE;
}


/**
 * Prints a message directly to the UART. The function is not thread safe
 * and corruptions are possible when multiple tasks attempt to print "simultaneously"
 *
 * Nothing is printed if 'msg' equals NULL.
 *
 * @note This function should only be called when the FreeRTOS scheduler is not running!
 *
 * @param msg - a message to be printed
 */
void vDirectPrintMsg(const portCHAR* msg)
{
	if ( NULL != msg ) {
		uart_print(printUartNr, msg);
	}
}


/**
 * Prints a character directly to the UART. The function is not thread safe and
 * corruptions are possible when multiple tasks attempt to print "simultaneously".
 *
 * @note his function should only be called when the FreeRTOS scheduler is not running!
 *
 * @param ch - a character to be printed
 */
void vDirectPrintCh(portCHAR ch)
{
	uart_printChar(printUartNr, ch);
}

#if 0
/*
 * For test only.....
 */
#define CFG_PBSIZE		64

size_t strnlen (const char *s, size_t count)
//int strnlen(const char * s, unsigned int count)
{
	const char *sc;

	for (sc = s; count-- && *sc != '\0'; ++sc)
        /* nothing */;
	return sc - s;
}

/* we use this so that we can do without the ctype library */
#define is_digit(c)     ((c) >= '0' && (c) <= '9')

static int skip_atoi(const char **s)
{
	int i=0;

	while (is_digit(**s))
		i = i*10 + *((*s)++) - '0';
	return i;
}

#define ZEROPAD	1               /* pad with zero */
#define SIGN	2               /* unsigned/signed long */
#define PLUS	4               /* show plus */
#define SPACE	8               /* space if plus */
#define LEFT	16              /* left justified */
#define SPECIAL	32              /* 0x */
#define LARGE	64              /* use 'ABCDEF' instead of 'abcdef' */

#define do_div(n,base) ({ \
int __res; \
__res = ((unsigned long) n) % (unsigned) base; \
n = ((unsigned long) n) / (unsigned) base; \
__res; })

static char * number(char * str, long num, int base, int size, int precision
        ,int type)
{
        char c,sign,tmp[66];
        const char *digits="0123456789abcdefghijklmnopqrstuvwxyz";
        int i;

        if (type & LARGE)
                digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        if (type & LEFT)
                type &= ~ZEROPAD;
        if (base < 2 || base > 36)
                return 0;
        c = (type & ZEROPAD) ? '0' : ' ';
        sign = 0;
        if (type & SIGN) {
                if (num < 0) {
                        sign = '-';
                        num = -num;
                        size--;
                } else if (type & PLUS) {
                        sign = '+';
                        size--;
                } else if (type & SPACE) {
                        sign = ' ';
                        size--;
                }
        }
        if (type & SPECIAL) {
                if (base == 16)
                        size -= 2;
                else if (base == 8)
                        size--;
        }
        i = 0;
        if (num == 0)
                tmp[i++]='0';
        else while (num != 0)
                tmp[i++] = digits[do_div(num,base)];
        if (i > precision)
                precision = i;
        size -= precision;
        if (!(type&(ZEROPAD+LEFT)))
                while(size-->0)
                        *str++ = ' ';
        if (sign)
                *str++ = sign;
        if (type & SPECIAL) {
                if (base==8)
                        *str++ = '0';
                else if (base==16) {
                        *str++ = '0';
                        *str++ = digits[33];
                }
        }
        if (!(type & LEFT))
                while (size-- > 0)
                        *str++ = c;
        while (i < precision--)
                *str++ = '0';
        while (i-- > 0)
                *str++ = tmp[i];
        while (size-- > 0)
                *str++ = ' ';
        return str;
}


int vsprintf(char *buf, const char *fmt, va_list args)
{
        int len;
        unsigned long num;
        int i, base;
        char * str;
        const char *s;

        int flags;              /* flags to number() */

        int field_width;        /* width of output field */
        int precision;          /* min. # of digits for integers; max
                                   number of chars for from string */
        int qualifier;          /* 'h', 'l', or 'L' for integer fields */

        for (str=buf ; *fmt ; ++fmt) {
                if (*fmt != '%') {
                        *str++ = *fmt;
                        continue;
                }

                /* process flags */
                flags = 0;
                repeat:
                        ++fmt;          /* this also skips first '%' */
                        switch (*fmt) {
                                case '-': flags |= LEFT; goto repeat;
                                case '+': flags |= PLUS; goto repeat;
                                case ' ': flags |= SPACE; goto repeat;
                                case '#': flags |= SPECIAL; goto repeat;
                                case '0': flags |= ZEROPAD; goto repeat;
                                }

                /* get field width */
                field_width = -1;
                if (is_digit(*fmt))
                        field_width = skip_atoi(&fmt);
                else if (*fmt == '*') {
                        ++fmt;
                        /* it's the next argument */
                        field_width = va_arg(args, int);
                        if (field_width < 0) {
                                field_width = -field_width;
                                flags |= LEFT;
                        }
                }

                /* get the precision */
                precision = -1;
                if (*fmt == '.') {
                        ++fmt;
                        if (is_digit(*fmt))
                                precision = skip_atoi(&fmt);
                        else if (*fmt == '*') {
                                ++fmt;
                                /* it's the next argument */
                                precision = va_arg(args, int);
                        }
                        if (precision < 0)
                                precision = 0;
                }

                /* get the conversion qualifier */
                qualifier = -1;
                if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L') {
                        qualifier = *fmt;
                        ++fmt;
                }

                /* default base */
                base = 10;

                switch (*fmt) {
                case 'c':
                        if (!(flags & LEFT))
                                while (--field_width > 0)
                                        *str++ = ' ';
                        *str++ = (unsigned char) va_arg(args, int);
                        while (--field_width > 0)
                                *str++ = ' ';
                        continue;

                case 's':
                        s = va_arg(args, char *);
                        if (!s)
                                s = "<NULL>";

                        len = strnlen(s, precision);

                        if (!(flags & LEFT))
                                while (len < field_width--)
                                        *str++ = ' ';
                        for (i = 0; i < len; ++i)
                                *str++ = *s++;
                        while (len < field_width--)
                                *str++ = ' ';
                        continue;

                case 'p':
                        if (field_width == -1) {
                                field_width = 2*sizeof(void *);
                                flags |= ZEROPAD;
                        }
                        str = number(str,
                                (unsigned long) va_arg(args, void *), 16,
                                field_width, precision, flags);
                        continue;


                case 'n':
                        if (qualifier == 'l') {
                                long * ip = va_arg(args, long *);
                                *ip = (str - buf);
                        } else {
                                int * ip = va_arg(args, int *);
                                *ip = (str - buf);
                        }
                        continue;

                case '%':
                        *str++ = '%';
                        continue;
                /* integer number formats - set up the flags and "break" */
                case 'o':
                        base = 8;
                        break;

                case 'X':
                        flags |= LARGE;
                case 'x':
                        base = 16;
                        break;

                case 'd':
                case 'i':
                        flags |= SIGN;
                case 'u':
                        break;

                default:
                        *str++ = '%';
                        if (*fmt)
                                *str++ = *fmt;
                        else
                                --fmt;
                        continue;
                }
                if (qualifier == 'l')
                        num = va_arg(args, unsigned long);
                else if (qualifier == 'h') {
                        num = (unsigned short) va_arg(args, int);
                        if (flags & SIGN)
                                num = (short) num;
                } else if (flags & SIGN)
                        num = va_arg(args, int);
                else
                        num = va_arg(args, unsigned int);
                str = number(str, num, base, field_width, precision, flags);
        }
        *str = '\0';
        return str-buf;
}

void print_msg(const char *fmt, ...)
{
	va_list args;
	char    printbuffer[CFG_PBSIZE];

	va_start(args, fmt);

	/* For this to work, printbuffer must be larger than
	 * anything we ever want to print.
	 */
	vsprintf(printbuffer, fmt, args);
	va_end(args);

	/* Print the string */
	uart_print(printUartNr, printbuffer);
}

#endif

