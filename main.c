#include <curses.h>
#include <dirent.h>
#include <ncurses.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

////
char *readable_fs(double size /*in bytes*/, char *buf);
void print_menu(WINDOW *menu_win, int highlight);
///

#define HEIGHT 50
#define WIDTH 50
int startx = HEIGHT;
int starty = WIDTH;
WINDOW *menu_win;
int highlight = 1;

char *navigation_list[] = {};
int *navigation_list_len = 0;

int main() {

  menu_win = newwin(HEIGHT, WIDTH, starty, startx);
  // print_menu(menu_win, highlight);

  DIR *d;
  struct dirent *dir;
  struct stat stat_buffer;
  d = opendir(".");
  char current_size[20];
  int row = 10;
  int col = 0;

  initscr();
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      mvprintw(row, col, "%s", dir->d_name);
      row++;
      stat(dir->d_name, &stat_buffer);
      // sprintf(current_size, "%jd", stat_buffer.st_size);
      mvprintw(row, col, "%s", readable_fs(stat_buffer.st_size, current_size));
      row++;
    }
    closedir(d);
  }

  // stat("/home/e/repos/c-ncurses/main.c", &stat_buffer);
  // intmax_t test = stat_buffer.st_size;
  // addstr("boom");
  // char swag[256];
  // sprintf(swag, "%jd", test);
  // addstr(swag);

  getch();
  endwin();
}

void print_menu(WINDOW *menu_win, int highlight) {
  int x, y, i;

  x = 2;
  y = 2;
  box(menu_win, 0, 0);
  for (i = 0; i < navigation_list_len; ++i) {
    if (highlight == i + 1) /* High light the present choice */
    {
      wattron(menu_win, A_REVERSE);
      mvwprintw(menu_win, y, x, "%s", navigation_list[i]);
      wattroff(menu_win, A_REVERSE);
    } else
      mvwprintw(menu_win, y, x, "%s", navigation_list[i]);
    ++y;
  }
  wrefresh(menu_win);
}

// https://programanddesign.com/cpp/human-readable-file-size-in-c/
char *readable_fs(double size /*in bytes*/, char *buf) {
  int i = 0;
  const char *units[] = {"B", "kB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
  while (size > 1024) {
    size /= 1024;
    i++;
  }
  sprintf(buf, "%.*f %s", i, size, units[i]);
  return buf;
}

// Links
// https://pubs.opengroup.org/onlinepubs/009604499/functions/stat.html
