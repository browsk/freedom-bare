#include <assert.h>
#include "buffer.h"


void init_ring_buffer(ring_buffer *buffer)
{
  ASSERT(buffer != 0);
}

int add_bytes(ring_buffer *buffer, void *source, int count)
{
  // TODO locking

}
