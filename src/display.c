#include <display.h>
#include <assert.h>
#include <stdlib.h>

#define min(a,b) ((a)>(b)?(b):(a))

WINDOW *main_screen;

/* private functions */
void init_colors(void);

void init_screen(void){
  main_screen = initscr();
  keypad(stdscr, TRUE);           // keypad enabled
  (void) nodelay(main_screen, 1); // no blocking
  (void) noecho();                // don't print to screen
  (void) nonl();                  // no new lines
  init_colors();
}

/*
 * crazy shifting is to set up every color 
 */
void init_colors(void) {
  int i;
  start_color();
  if (COLORS < 255) {
    for (i = 0; i < (1 << 8); i ++) {
      init_pair(i, 255, 0); // 0 --> i if you want pure blocks, otherwise ascii
    }
  } else {
    for (i = 0; i < (1 << 8); i ++) {
      init_pair(i, i, 0); // 0 --> i if you want pure blocks, otherwise ascii
    }
  }
  return;
}

void end_screen(void) {
  endwin();
}

/* allow us to directly map to the 216 colors ncurses makes available */
static inline int get_color(int r, int g, int b) {
  return 16+r/48*36+g/48*6+b/48;
}

const char ascii_values[] = " ..::--==+++***###%%%%%%%%@@@@@@@";

int draw_line(char *data, int width, int y, int channels) {
  int j;
  unsigned char b, g, r;
  int intensity;
  for (j = 0; j < width; j++){
    b = data[j * channels];
    g = data[j * channels + 1];
    r = data[j * channels + 2];
    intensity = (sizeof(ascii_values) - 1) * ((r/255.0 + g/255.0 + b/255.0) / 3);
    char val = ascii_values[intensity];
    int color = get_color(r, g, b);
    if (COLORS < 255) {
      color = 0;
    }
    mvaddch(y, j, val|COLOR_PAIR(color));
  } 

  refresh();
  return 0;
}

int write_bandwidth(char *bandstr, int bandlen, int width, int height) {
  int i;

  if (width < bandlen) {
    /* can't write it to the screen */
    return -1;
  }

  for (i = width - bandlen; i < width; i++) {
    mvaddch(0, i, bandstr[i - width + bandlen]);
  }

  refresh();
  return 0;
}
