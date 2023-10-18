#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h>

void syscall_handler(uint64_t id);

void sys_write();

#endif