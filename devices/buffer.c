#include <assert.h>
#include <string.h>
#include "buffer.h"
#include "MKL25Z4.h"
#include "core_cm0plus.h"

#include "core_cmFunc.h"
#include "freedom.h"

#define NEXT_INDEX(x) ((x + 1) & (2 * RING_BUF_SIZE - 1))

int is_empty(struct ring_buffer *buffer)
{
  return buffer->head == buffer->tail;
}

static int is_full(struct ring_buffer *buffer)
{
  return buffer->tail == (buffer->head ^ RING_BUF_SIZE);
}

void init_ring_buffer(struct ring_buffer *buffer)
{
  memset(buffer, 0, sizeof(buffer));
}

int add_bytes(struct ring_buffer *buffer, const uint8_t *source, int count)
{
  const uint8_t * data = source;

  while(count--)
  {
    // diable and enable interrupts each time around the loop to minimise the amount of time 
    // with ints disabled
    __disable_irq();

    if (is_full(buffer))
    {
      __enable_irq();
      break;
    }

    buffer->data[buffer->head & (RING_BUF_SIZE - 1)] = *data++;
    buffer->head = NEXT_INDEX(buffer->head);

    __enable_irq();
  }
  return data - source;
}

int get_bytes(struct ring_buffer *buffer, uint8_t *dest, int count)
{
  uint8_t * data = dest;


  while(count--)
  {
    __disable_irq();

    // no point continuing if buffer is empty
    if (is_empty(buffer))
    {
      __enable_irq();
      break;
    }

    *data++ = buffer->data[buffer->tail & (RING_BUF_SIZE - 1)];
    buffer->tail = NEXT_INDEX(buffer->tail);

    __enable_irq();
  }

  return data - dest;
} 

int get_byte(struct ring_buffer *buffer, uint8_t *byte)
{
  return get_bytes(buffer, byte, 1);
}
