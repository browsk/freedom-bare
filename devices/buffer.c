#include <assert.h>
#include <string.h>
#include "buffer.h"
#include "MKL25Z4.h"
#include "core_cm0plus.h"

#include "core_cmFunc.h"
#include "freedom.h"

extern void uart0_write_char(char);

void test_buffer()
{
  
}

#define NEXT_INDEX(x) ((x + 1) & (2 * RING_BUF_SIZE - 1))

int is_empty(ring_buffer *buffer)
{
  return buffer->head == buffer->tail;
}

static int is_full(ring_buffer *buffer)
{
  return buffer->tail == (buffer->head ^ RING_BUF_SIZE);
}

void init_ring_buffer(ring_buffer *buffer)
{
  memset(buffer, 0, sizeof(buffer));
}

int add_bytes(ring_buffer *buffer, const uint8_t *source, int count)
{
  const uint8_t * data = source;

  while(count--)
  {
    __disable_irq();

    if (is_full(buffer))
    {
      __DMB();
      __enable_irq();
      while(!is_empty(buffer));
      __disable_irq();
    }

    buffer->data[buffer->head & (RING_BUF_SIZE - 1)] = *data++;
    buffer->head = NEXT_INDEX(buffer->head);

    __enable_irq();
  }
  return data - source;
}

int get_bytes(ring_buffer *buffer, uint8_t *dest, int count)
{
  uint8_t * data = dest;


  while(count--)
  {
    __disable_irq();

    if (is_empty(buffer))
      break;

    *data++ = buffer->data[buffer->tail & (RING_BUF_SIZE - 1)];
    buffer->tail = NEXT_INDEX(buffer->tail);

    __enable_irq();
  }

  return data - dest;
} 

int get_byte(ring_buffer *buffer, uint8_t *byte)
{
  return get_bytes(buffer, byte, 1);
}
