#ifndef FIFO_H_
#define FIFO_H_

#include "uCXpresso.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	int		size;
	int		head;
	int 	tail;
	uint8_t	*buf;
} fifo_t, FIFO_T;

void fifo_init(fifo_t *fifo, uint8_t *buf, int size);
void fifo_reset(fifo_t *fifo);
bool fifo_put(fifo_t *fifo, uint8_t c);
bool fifo_get(fifo_t *fifo, uint8_t *pc);
int  fifo_avail(fifo_t *fifo);
int	 fifo_free(fifo_t *fifo);
int fifo_put_block(fifo_t *fifo, uint8_t *buf, int size);
int fifo_get_block(fifo_t *fifo, uint8_t *buf, int size);

#ifdef __cplusplus
}
#endif

#endif
