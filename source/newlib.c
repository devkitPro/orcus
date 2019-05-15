#include <errno.h>
#include <unistd.h>
#include <orcus.h>
#include <reent.h>
#include <sys/lock.h>
#include <sys/iosupport.h>

#undef errno

void* heap_ptr;

void *_sbrk_r(struct _reent *ptr, ptrdiff_t incr) {
  if ((heap_ptr + incr) < (void*)__heap_end) {
    void *base = heap_ptr;
    heap_ptr += incr;
    return base;
  } else {
    ptr->_errno = ENOMEM;
    return  (void *) -1;
  }
}
