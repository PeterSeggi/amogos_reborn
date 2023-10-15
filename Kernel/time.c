#include <naiveConsole.h>
#include <stdint.h>
#include <lib.h>
#include <time.h>

static unsigned long ticks = 0;
uint8_t *clockLocation = (uint8_t*)0xB8000;

void timer_handler() {
  ticks++;
  update_clock();
}

int ticks_elapsed() { return ticks; }

void update_clock() {
  if (ticks % 18 == 0) {
    uint8_t *bufferVideo = getCurrentVideo();
    setCurrentVideo(clockLocation);

    ncPrintHex(rtcInfo(4));
    ncPrint(":");
    ncPrintHex(rtcInfo(2));
    ncPrint(":");
    ncPrintHex(rtcInfo(0));

    setCurrentVideo(bufferVideo);
  }
}

void set_clock_location(uint8_t location) { *clockLocation = location; }

int seconds_elapsed() { return ticks / 18; }
