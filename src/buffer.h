#ifndef _BUFFER_H
#define _BUFFER_H

#include <stdint.h>

#define RING_BUF_SIZE 64

typedef struct _ring_buffer
{
  uint8_t data[RING_BUF_SIZE];
  int head;
  int tail;
}
ring_buffer;

/**
 * Initialise ring buffer ready for use. This function MUST be called prior to
 * using a ring buffer
 *
 * @param buffer  The buffer to initialise
 */
void init_ring_buffer(ring_buffer *buffer);

/**
 * Add bytes to the specified buffer. Bytes are added up until the buffer is full
 * 
 * @param buffer  pointer to the ring buffer to add chars to
 * @param source  pointer to the source data
 * @param count   number of bytes to add to the ring buffer
 *
 * @return The number of bytes added. This may be less than count
 *
 */
int add_bytes(ring_buffer *buffer, void *source, int count);

/**
 * Read bytes from the specified buffer. Bytes are copied into destination memory
 *
 * @param buffer  pointer to the ring buffer to get chars from
 * @param source  pointer to the destination data
 * @param count   number of bytes to remove from the ring buffer
 *
 * @return The number of bytes removed. This may be less than count.
 *
 */
int get_bytes(ring_buffer *buffer, void *dest, int count);

/**
 * Get a single byte from the specified buffer
 *
 * @param buffer  pointer to the buffer to get a byte from
 * @param byte    pointer to the memory that will hold the removed byte on return
 *
 * @return  1 if a byte was removeed, 0 otherwiseA
 *
 */
int get_byte(ring_buffer *buffer, uint8_t *byte);

#endif
