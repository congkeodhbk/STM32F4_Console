#include "console.h"
#include <stdint.h>
#include <stdio.h>
#include <_ansi.h>
#include <_syslist.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/times.h>
#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

static char buffer[CONSOLE_BUFF_LEN];
static uint32_t idx;
static console_get_char_t console_get_char = NULL;
static console_callback_t console_callback = NULL;
static console_put_t console_put = NULL;
static bool console_echo =false;

void console_init(console_callback_t callback,
					console_get_char_t get_char,
					console_put_t put, bool rx_echo)
{
    console_callback = callback;
    console_get_char = get_char;
    console_put = put;
    console_echo = rx_echo;
    idx = 0;
	setvbuf(stdout, NULL, _IONBF, 0);
}

void console_handle_char(char c)
{
    buffer[idx++] = c;
    if (idx >= CONSOLE_BUFF_LEN)
    {
        idx = 0;
        return;
    }
    if (c == '\n' || c == '\r')
    {
        buffer[idx] = 0;
        console_callback((char *)buffer);
        idx = 0;
    }
    if(c == '\b' && idx > 0)
    {
    	idx--;
    }
}

void console_process()
{
    static int key;
    while ((key = console_get_char()) > 0)
    {
    	if(console_echo)
    	{
    		console_put((char *)&key, 1);
    	}
        console_handle_char((char)key);
    }
}

int _write(int fd, char* ptr, int len) {
	if(console_put ==NULL)
	{
		goto __exit;
	}
	if(fd == STDOUT_FILENO || fd == STDERR_FILENO) {
		console_put(ptr, len);
		return len;
	}
  __exit:
  errno = EBADF;
  return -1;
}
