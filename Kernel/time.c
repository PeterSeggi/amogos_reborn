#include "include/lib.h"
#include <lib.h>
#include <naiveConsole.h>
#include <stdint.h>
#include <time.h>

static unsigned long ticks = 0;
uint8_t *clockLocation = (uint8_t *)0xB808E;

void timer_handler() {
  ticks++;
  update_clock();
}

int ticks_elapsed() { return ticks; }

void update_clock() {
  if (ticks % 18 == 0) {

    uint8_t *current = getCurrentVideo();
    setCurrentVideo(clockLocation);

    ncPrint("");
    ncPrintHex(rtcInfo(4));
    ncPrint(":");
    ncPrintHex(rtcInfo(2));
    ncPrint(":");
    int seconds = rtcInfo(0);
    if (seconds <= 9) ncPrint("0");
    ncPrintHex(seconds);

    setCurrentVideo(current);
  }
}

void set_clock_location(uint8_t *location) { clockLocation = location; }

int seconds_elapsed() { return ticks / 18; }

void sleep(int sec) {
  // unsigned long t0 = ticks_elapsed();
  // while( (ticks_elapsed()-t0)/18 < sec );
  int i = 0;
  while (i++ < 100000000)
    ;
}
