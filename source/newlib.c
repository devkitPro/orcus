#include <errno.h>
#include <unistd.h>
#include <orcus.h>
#include <reent.h>
#include <sys/lock.h>
#include <sys/time.h>
#include <sys/iosupport.h>

#undef errno

void* heap_ptr;
extern uint32_t __end_of_heap;
void* heap_end_ptr = (void*)&__end_of_heap;

void* _sbrk_r(struct _reent *ptr, ptrdiff_t incr) {
  if ((heap_ptr + incr) < heap_end_ptr) {
    void *base = heap_ptr;
    heap_ptr += incr;
    return base;
  } else {
    ptr->_errno = ENOMEM;
    return  (void *) -1;
  }
}

int orcus_nanosleep(const struct timespec *req, struct timespec *rem) {
  for(time_t i = 0 ; i < req->tv_sec ; i++) {
    timerSleepNs(1000000000);
  }
  timerSleepNs(req->tv_nsec);
  return 0;
}

static _ssize_t _uart_write_r(struct _reent *r, void *fd, const char *ptr, size_t len) {
  for(int i = 0 ; i < len ; i++) {
    uartPutc(ptr[i], true);
  }
        
  return len;
}

static _ssize_t _uart_read_r(struct _reent *r, void *fd, char *ptr, size_t len) {
  int  i;
  for(i = 0 ; i < len ; i++) {
    ptr[i] = uartGetc(true);
    if('\n' == ptr[i] || '\r' == ptr[i]) {
      return i;
    }
  }
  return i;
}

static const devoptab_t dotab_stdout = {
        "uart",
        0,
        NULL,
        NULL,
        _uart_write_r,
        NULL,
        NULL,
        NULL
};

static const devoptab_t dotab_stdin = {
        "uart",
        0,
        NULL,
        NULL,
	NULL,
        _uart_read_r,
        NULL,
        NULL
};

void orcus_init_syscalls() {
  __syscalls.nanosleep = &orcus_nanosleep;
  devoptab_list[STD_OUT] = &dotab_stdout;
  devoptab_list[STD_ERR] = &dotab_stdout;
  devoptab_list[STD_IN] = &dotab_stdin;
}

