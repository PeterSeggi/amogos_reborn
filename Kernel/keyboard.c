#include "include/processManager.h"
#include <keyboard.h>
#include <lib.h>
#include <naiveConsole.h>
#include <stdint.h>
#include <videoDriver.h>
#include <pipe.h>
#include <processManager.h>
#include <sem.h>

#define KEY_BUF_SIZE 16
#define STDIN 0
#define STDKEYS 3
#define STDLAST 4
#define OUT_FD 5

sem_t * key_mutex; 

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

// All keys
int key_buf[KEY_BUF_SIZE];
int insert_index = 0;
int read_index = 0;
int to_read = 0;

// Ascii characters
int ascii_buf[KEY_BUF_SIZE];
int shifted = 0;
int caps = 0;
int control = 0;
int saved = 0;
int ascii_insert_index = 0;
int ascii_read_index = 0;
int ascii_to_read = 0;
char toWrite = 0;
int fore_fd = 0;
int fore = 0;


void key_handler() { insert_key(_getKey()); }

void key_init(){
    char * mutex_name = "keyboard";
    key_mutex = sem_open(mutex_name, 1);
}

void insert_key(int key) {

  checkShift(key);

  if (key > 0x52 || scan_chars[key] == 0) return;

    if (shifted || (caps && key >= 0x10)){
      toWrite = scan_chars_shift[key];
    }
    else{
      toWrite = scan_chars[key];
    }


  fore_fd = get_foreground_fd() + 1;
  write_pipe(fore_fd, &toWrite, 1);

    
  if (control == 1 && (toWrite == 'c')){
     fore = get_foreground();
     kill(fore);
  } 
    

}

// returns the actual key, 0 if nothing was read
int read_key(int fd) {
  int toRet;

  if (fd == STDIN) {
    if (!ascii_to_read)
      return 0;

    toRet = ascii_buf[ascii_read_index++];
    if (ascii_read_index == KEY_BUF_SIZE)
      ascii_read_index = 0;

    if (ascii_read_index == ascii_insert_index) {
      ascii_to_read = 0;
    }
  }

  else if (fd == STDKEYS) {
    if (!to_read)
      return 0;

    toRet = key_buf[read_index++];
    if (read_index == KEY_BUF_SIZE)
      read_index = 0;

    if (read_index == insert_index)
      to_read = 0;
  }

  else if (fd == STDLAST) {
    toRet = ascii_buf[ascii_insert_index - 1];
  }

  return toRet;
}

// flush the input buffer
void flush_buffer() {

  // scan codes
  read_index = 0;
  insert_index = 0;
  to_read = 0;

  // ascii
  ascii_insert_index = 0;
  ascii_read_index = 0;
  ascii_to_read = 0;
  
}

void checkShift(int key) {
  // shift
  if (key == 0x2A || key == 0x37)
    shifted = 1;
  if (key == 0xAA || key == 0xB6)
    shifted = 0;

  // caps
  if (key == 0x3A)
    caps = !caps;

  // control
  if (key == 0x1D)
    control = 1;
  if (key == 0x9D)
    control = 0;
}


int altPressed(){
    return saved;
}

