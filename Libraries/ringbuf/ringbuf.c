/**
	Source file for ringbuffer implementation base on contiki-os core and many
	resources
	
*/

#include "ringbuf.h"

void RINGBUF_Init(RINGBUF *r, uint8_t* buf, uint32_t size, ringbuf_interrupt_t interrupt)
{
	r->head = r->tail = 0;
	r->size = size;
	r->pt = buf;
	r->interrupt = interrupt;
}

int32_t RINGBUF_Put(RINGBUF *r, uint8_t c)
{
	int32_t res;
	r->interrupt(false);
	if (r->head < r->tail){
		if (r->tail-r->head == 1){ // full
			res = 0; 
			goto RINGBUF_Put_exit;
		}
	}
	else if (r->head > r->tail){
		if (r->head-r->tail == (r->size-1)){ // full
			res = 0; 
			goto RINGBUF_Put_exit;
		}
	}
	r->pt[r->head++] = c;
	if (r->head == r->size) r->head = 0;
	res = 1;
	RINGBUF_Put_exit:
	r->interrupt(true);
	return res;
}

int32_t RINGBUF_Get(RINGBUF *r, uint8_t* c)
{
	int32_t res;
	r->interrupt(false);
	if (r->head == r->tail){ // empty
		res = 0;
		goto RINGBUF_Get_exit;
	}
	*c = r->pt[r->tail++];
	if (r->tail == r->size) r->tail = 0;
	res = 1;
	RINGBUF_Get_exit:
	r->interrupt(true);
	return res;
}

int32_t RINGBUF_Count(RINGBUF *r)
{
	if (r->head >= r->tail){
		return (r->head-r->tail);
	}
	else if (r->tail > r->head){
		return (r->size+r->head-r->tail);
	}
	return 0;
}

int32_t RINGBUF_Free(RINGBUF *r)
{
	return (r->size-RINGBUF_Count(r)-1);// always reserve 1 item!
}

void RINGBUF_Flush(RINGBUF *r)
{
	r->tail = r->head;
}

int32_t RINGBUF_Available(RINGBUF *r)
{
	return (r->head != r->tail);
}

int32_t RINGBUF_Size(RINGBUF *r)
{
	return r->size;
}

int32_t RINGBUF_Put_Buff(RINGBUF *r, uint8_t *buff, uint16_t len)
{
	uint16_t count=0;
	while(count <len){
		if(RINGBUF_Put(r, buff[count])==0){
			break;
		}
		count++;
	}
	return count;
}
/* --------------- DMA Specific ------------------- */
int32_t RINGBUF_CountInSegment(RINGBUF *r)
{
	if (r->head >= r->tail){
		return (r->head - r->tail);
	}
	else {
		return (r->size - r->tail);
	}
}

uint8_t *RINGBUF_GetTailPointer(RINGBUF *r)
{
	return (uint8_t *)(&(r->pt[r->tail]));
}

void RINGBUF_UpdateTail(RINGBUF *r, uint32_t size)
{
	r->tail += size;
	r->tail	%= r->size;
}

void RINGBUF_SetHead(RINGBUF *r, uint32_t newHead)
{
	r->head = newHead % r->size;
}

