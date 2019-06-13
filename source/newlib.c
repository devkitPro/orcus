#include <errno.h>
#include <unistd.h>
#include <orcus.h>
#include <reent.h>
#include <sys/lock.h>
#include <sys/time.h>
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

int orcus_nanosleep(const struct timespec *req, struct timespec *rem) {
  // TODO - need interrupts enabled to use the timer
  return 0;
}

void orcus_init_syscalls() {
  __syscalls.nanosleep = &orcus_nanosleep;
}
