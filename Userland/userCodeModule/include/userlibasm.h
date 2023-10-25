#ifndef _OURASMLIB_H_
#define _OURASMLIB_H_

void _print(int fd, char * string, int length);
int _read(int fd, char * buffer, int length);
int int_test();
void _nanosleep(int cant, int unidad);
int _getSeconds();
int _getHours();
int _getMinutes();
int _getDateTimeFormat();

#endif
