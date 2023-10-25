#include "include/videoDriver.h"
#include <lib.h>
#include <naiveConsole.h>
#include <stdint.h>
#include <videoDriver.h>

#define KEY_BUF_SIZE 16
#define STDIN 0
#define STDKEYS 3

const unsigned char scan_chars[128] = {
    0,    27,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-',  '=',
    '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[',  ']',
    '\n', 0,    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0,    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,    '*',
    0,    ' ',  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,
    0,    0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,
    '-',  0,    0,   '+', 0,   0,   0,   0,   0,   0,   0,   0,   0,    0,
    0,    0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,
    0,    0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0};

const unsigned char scan_chars_shift[128] = {
    0,    27,   '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
    '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}',
    '\n', 0,    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '|',
    0,    '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,   '*',
    0,    ' ',  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,    0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    '-',  0,    0,   '+', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,    0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,    0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0};

int key_buf[KEY_BUF_SIZE];
int ascii_buf[KEY_BUF_SIZE];
int shifted = 0;
int insert_index = 0;
int ascii_insert_index = 0;

int ascii_read_index = 0;
int read_index = 0;
int to_read = 0;

void key_handler() { insert_key(readKey()); }

void insert_key(int key) {

  if (scan_chars[key] != 0) {
    if (!shifted)
        ascii_buf[ascii_insert_index] = scan_chars[key];
    else
        ascii_buf[ascii_insert_index] = scan_chars_shift[key];
  }

  key_buf[insert_index++] = key;
  to_read = 1;
  printChar(key_buf[key]);
  if (insert_index == KEY_BUF_SIZE)
    insert_index = 0;
}

// returns the actual key, 0 if nothing was read
int read_key(int fd) {
  if (!to_read)
    return 0;

  int toRet;

  if (fd == STDIN)
     toRet = ascii_buf[ascii_read_index++];

    else if (fd == STDKEYS)
     toRet = key_buf[read_index++];

  if (read_index == KEY_BUF_SIZE)
    read_index = 0;

  if (read_index == insert_index)
    to_read = 0;

  return toRet;
}

// flush the input buffer
void flush_buffer() {
  read_index = 0;
  insert_index = 0;
  to_read = 0;
}

void checkShift(int key){
    if (key == 0x2B || key == 0x37)
        shifted = 1;
    if (key == 0xAB || key == 0xB6)
        shifted = 0;
}
