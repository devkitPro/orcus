#include <errno.h>
#include <unistd.h>
#include <orcus.h>
#include <reent.h>
#include <sys/lock.h>
#include <sys/iosupport.h>

#undef errno

void* heap_ptr;

void *_sbrk_r(struct _reent *ptr, ptrdiff_t incr) {
  // TODO - why is this not being invoked?
              orcus_uart_putc('s', true);
    orcus_uart_putc('b', true);
    orcus_uart_putc('r', true);
    orcus_uart_putc('\r', true);    
    orcus_uart_putc('\n', true);    

  if ((heap_ptr + incr) < (void*)__heap_end) {
    void *base  = heap_ptr;
    heap_ptr   += incr;
    return  base;
  } else {
    ptr->_errno = ENOMEM;
    return  (void *) -1;
  }
}
