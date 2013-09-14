#include <assert.h>
#include "buffer.h"

void test_buffer()
{
  
}

#define NEXT_INDEX(x) ((x + 1) & (2 * RING_BUF_SIZE - 1))

static is_empty(ring_buffer *buffer)
{
  return buffer->head == buffer->tail;
}

static is_full(ring_buffer *buffer)
{
  return buffer->tail == buffer->head ^ RING_BUF_SIZE;
}

void init_ring_buffer(ring_buffer *buffer)
{
  ASSERT(buffer != 0);

  memset(buffer, 0, sizeof(buffer));
}

int add_bytes(ring_buffer *buffer, uint8_t *source, int count)
{
  uint8_t * data = source;

  // TODO locking
  while(!is_full(buffer) && count--)
  {
    buffer->data[buffer->head & RING_BUF_SIZE - 1] = *data++;
    buffer->head = NEXT_INDEX(buffer->head);
  }
  return data - source;
}

int get_bytes(ring_buffer *buffer, uint8_t *dest, int count)
{
  uint8_t * data = dest;

  // TODO locking
  while(!is_empty(buffer) && count--)
  {
    *data++ = buffer->data[buffer->tail];
    buffer->tail = NEXT_INDEX(buffer->tail);
  }
  return data - dest;
}

int get_byte(ring_buffer *buffer, uint8_t *byte)
{
  return get_bytes(buffer, byte, 1);
}
