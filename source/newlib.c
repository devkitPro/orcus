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
  const uint32_t sleepUntilS = 7407407+1; // counter increments every 0.135uS, so 1s is 7407407 clocks
  for(time_t i = 0 ; i < req->tv_sec ; i++) {
    timerSet(1);
    while(timerGet() < sleepUntilS);    
  }
  
  uint32_t sleepUntilNs = (req->tv_nsec/135)+1;
  timerSet(1);
  while(timerGet() < sleepUntilNs);

  return 0;
}

void orcus_init_syscalls() {
  __syscalls.nanosleep = &orcus_nanosleep;
}

_ssize_t _write_r (struct _reent *ptr, int file , const void* buf, size_t nbytes) {
  char* castBuf = (char*) buf;
  if ((file != STDOUT_FILENO) && (file != STDERR_FILENO)) {
    ptr->_errno = EBADF;
    return -1;
  }

  for(int i = 0 ; i < nbytes ; i++) {
    uartPutc(castBuf[i], true);
  }
        
  return nbytes;
}


_ssize_t _read_r(struct _reent* ptr, int file, void* buf, size_t len) {
  char* castBuf = (char*) buf;
  if(STDIN_FILENO == file) {
    int  i;
    for(i = 0 ; i < len ; i++) {
      castBuf[i] = uartGetc(true);
      if('\n' == castBuf[i] || '\r' == castBuf[i]) {
	return i;
      }
    }
    return i;
  } else {
    ptr->_errno = EBADF;
    return -1;
  }
}
