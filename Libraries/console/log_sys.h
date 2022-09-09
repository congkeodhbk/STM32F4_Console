#ifndef __log_sys_h__
#define __log_sys_h__

#ifdef __cplusplus9
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
	 
#define error(fmt, ...)  do {} while (0)
#define warning(fmt, ...)  do {} while (0)
#define info(fmt, ...)  do {} while (0)
#define debug(fmt, ...)  do {} while (0)
#define debugx(fmt, ...)  do {} while (0)

#ifdef DEBUG

#define __FILENAME_ONLY__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define _printf(...)\
	do{\
		printf(__VA_ARGS__);\
		printf("\r\n");\
	}while(0)

#define _printf_format(header, fmt, ...) _printf("[%s] %s - Line: %d: " fmt, header, __FILENAME_ONLY__, __LINE__,## __VA_ARGS__)
#if __DEBUG__>0
#undef error
#define error(fmt, ...) _printf_format("ERROR", fmt, ## __VA_ARGS__)
#endif

#if __DEBUG__>1
#undef warning
#define warning(fmt, ...) _printf_format("WARNG", fmt, ## __VA_ARGS__)
#endif

#if __DEBUG__>2
#undef info
#define info(fmt, ...) _printf_format("INFO", fmt, ## __VA_ARGS__)
#endif

#if __DEBUG__>3
#undef debug
#undef debugx
#define debug(fmt, ...) _printf_format("DEBUG", fmt, ## __VA_ARGS__)
#define debugx(...) _printf(__VA_ARGS__)
#endif

#endif /* DEBUG */

#ifdef __cplusplus
}
#endif

#endif
