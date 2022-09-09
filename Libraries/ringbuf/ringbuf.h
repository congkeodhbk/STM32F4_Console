/**
	Header file for ringbuffer implementation base on contiki-os core and many
	resources
	
	HieuNT
	17/7/2015
*/

#ifndef _RING_BUF_H_
#define _RING_BUF_H_

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include <stdint.h>
#include <stdbool.h>

#ifndef __IO
#define __IO volatile
#endif

typedef void (* ringbuf_interrupt_t)(bool active);

typedef struct
{
    __IO uint32_t head;                
    __IO uint32_t tail;                
    __IO uint32_t size;                
    __IO uint8_t *pt;
    ringbuf_interrupt_t interrupt;
} RINGBUF;

void RINGBUF_Init(RINGBUF *r, uint8_t* buf, uint32_t size, ringbuf_interrupt_t interrupt);
int32_t RINGBUF_Put(RINGBUF *r, uint8_t c);																										
int32_t RINGBUF_Put_Buff(RINGBUF *r, uint8_t *buff, uint16_t len);
int32_t RINGBUF_Get(RINGBUF *r, uint8_t* c);
int32_t RINGBUF_Count(RINGBUF *r);
int32_t RINGBUF_Free(RINGBUF *r);
int32_t RINGBUF_Available(RINGBUF *r);
void RINGBUF_Flush(RINGBUF *r);
int32_t RINGBUF_Size(RINGBUF *r);
int32_t RINGBUF_CountInSegment(RINGBUF *r);
uint8_t *RINGBUF_GetTailPointer(RINGBUF *r);
void RINGBUF_UpdateTail(RINGBUF *r, uint32_t size);
void RINGBUF_SetHead(RINGBUF *r, uint32_t newHead);

#ifdef __cplusplus
 }
#endif
	 
#endif
