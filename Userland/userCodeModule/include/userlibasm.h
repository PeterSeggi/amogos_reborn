#ifndef _OURASMLIB_H_
#define _OURASMLIB_H_

void _print(int fd, char * string, int length);
int _read(int fd, char * buffer, int length);
int int_test();
void _sleep(int cant, int unidad);
void _getClock(uint32_t *hrs, uint32_t *min, uint32_t *seg);
int _getSeconds();
int _getHours();
int _getMinutes();
int _getDateTimeFormat();

#endif
