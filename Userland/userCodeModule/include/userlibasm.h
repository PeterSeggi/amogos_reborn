#include <stdint.h>

void _print(int fd, char * string, int length);
int _read(int fd, char * buffer, int length);
int int_test();
void _sleep(int cant, int unidad);
void _getClock(int *hrs, int *min, int *seg);
void _draw(uint16_t * bitmap, uint32_t color, uint16_t height, uint64_t x, uint64_t y);
void _screenData(uint16_t * screenHeight, uint16_t * screenWidth, uint8_t * fontSize, uint8_t * drawSize);
void _changeSize(uint8_t newSize, uint8_t fd);
int _getRegs(uint64_t regs[]);
void _opError();
void _beep(uint32_t frequency, int duration);
Process ** _get_processes(uint16_t * proc_amount);
void _getMemState(uint64_t * states);
void * _my_malloc(uint16_t size);
void _my_free(void * addr_to_free);

sem_t * _sem_open(const char *name, uint16_t value);
int _sem_close(sem_t *sem);
int _sem_up(sem_t *sem);
int _sem_down(sem_t *sem);

int _pipe(int pipefd[2]);
int _pclose(int fd);