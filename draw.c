#include <curses.h>
#include <dirent.h>
#include <ncurses.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

//
void print_menu(WINDOW *menu_win, int highlight);
void get_dir_info();
///
#define MAX_FILE_NUMBER 10
#define OFFSET 40

int startx = 0;
int starty = 0;
char *file_list[MAX_FILE_NUMBER][256] = {};
intmax_t file_size_list[MAX_FILE_NUMBER] = {};
int file_list_lenght = 0;
DIR *d;
struct dirent *dir;
struct stat stat_buffer;
int iter = 0;

int main() {
  WINDOW *menu_win;
  int highlight = 1;
  int choice = 0;
  int c;

  initscr();
  clear();
  // noecho();
  // cbreak(); /* Line buffering disabled. pass on everything */

  get_dir_info();
  menu_win = newwin(LINES, COLS, starty, startx);
  keypad(menu_win, TRUE);
  mvprintw(0, 0,
           "Use arrow keys to go up and down, Press enter to select a choice");
  refresh();
  print_menu(menu_win, highlight);
  while (1) {
    c = wgetch(menu_win);
    switch (c) {
    case KEY_UP:
      if (highlight == 1)
        highlight = file_list_lenght;
      else
        --highlight;
      break;
    case KEY_DOWN:
      if (highlight == file_list_lenght)
        highlight = 1;
      else
        ++highlight;
      break;
    default:
      refresh();
      break;
    }
    print_menu(menu_win, highlight);
  }
  // clrtoeol();
  // refresh();
  endwin();
}

void get_dir_info() {
  iter = 0;
  file_list_lenght = 0;
  d = opendir(".");
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      stat(dir->d_name, &stat_buffer);

      strcpy(file_list[iter], dir->d_name);
      file_size_list[iter] = stat_buffer.st_size;

      // printf("%jd", stat_buffer.st_size);
      // printf("\n");

      file_list_lenght++;
      iter++;
    }
    closedir(d);
  }
}

void print_menu(WINDOW *menu_win, int highlight) {
  int x, y, i;

  x = 2;
  y = 2;
  box(menu_win, 0, 0);
  for (i = 0; i < file_list_lenght; ++i) {
    if (highlight == i + 1) /* High light the present choice */
    {
      wattron(menu_win, A_REVERSE);
      mvwprintw(menu_win, y, x, "%s", file_list[i]);
      mvwprintw(menu_win, y, x + OFFSET, "%jd", file_size_list[i]);
      wattroff(menu_win, A_REVERSE);
    } else
      mvwprintw(menu_win, y, x + OFFSET, "%jd", file_size_list[i]);
    mvwprintw(menu_win, y, x, "%s", file_list[i]);
    ++y;
  }
  wrefresh(menu_win);
}
