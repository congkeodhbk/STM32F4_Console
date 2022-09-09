#ifndef __CONSOLE_H_
#define __CONSOLE_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "log_sys.h"
#include <stdint.h>
#include <stdbool.h>

#ifndef CONSOLE_BUFF_LEN
#define CONSOLE_BUFF_LEN 128
#endif

#define __check_cmd_at_pos(x,p) (strncmp(&message[p], x, strlen(x)) == 0)
#define __check_cmd(x) __check_cmd_at_pos(x,0)
#define __param_pos(x)	((char *)(&message[strlen(x)]))

// return -1 if no char available
typedef int (* console_get_char_t)(void);
typedef void (* console_callback_t)(char *result);
typedef void (* console_put_t)(char *data, uint16_t len);

void console_init(console_callback_t callback,
					console_get_char_t get_char,
					console_put_t put,
					bool rx_echo);
void console_process();
void console_handle_char(char c);

#ifdef __cplusplus
}
#endif
#endif
