#include <curses.h>
#include <dirent.h>
#include <ncurses.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

//
void print_menu(WINDOW *menu_win, int highlight);
void change_dir();
void dir_back(WINDOW *menu_win);
///
#define MAX_FILE_NUMBER 100
#define MAX_FILE_NAME_SIZE 100
#define OFFSET 40

int startx = 0;
int starty = 0;
char *file_list[MAX_FILE_NUMBER];
int file_list_lenght = 0;

intmax_t file_size_list[MAX_FILE_NUMBER] = {};
struct dirent *dir;
struct stat stat_buffer;
int iter = 0;
int highlight = 1;

void dir_back(WINDOW *menu_win) {}

void dir_forward() {
  // char *selection = {file_list[highlight - 1]};
  // wprintw(menu_win, "%s", selection);
  // wprintw(menu_win, "%s", PATH);
}

void get_dir_info(WINDOW *menu_win) {

  // Free up string array
  for (int i = 0; i < file_list_lenght; i++) {
    free(file_list[i]);
  }

  // Reset dir logic
  // wprintw(print_menu, PATH);
  DIR *d;
  d = opendir(".");
  iter = 0;
  file_list_lenght = 0;
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      file_list[iter] = malloc(MAX_FILE_NAME_SIZE * sizeof(char));
      strcpy(file_list[iter], dir->d_name);

      stat(dir->d_name, &stat_buffer);
      file_size_list[iter] = stat_buffer.st_size;

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
  wattroff(menu_win, A_REVERSE);
  box(menu_win, 0, 0);
  for (i = 0; i < file_list_lenght; ++i) {
    if (highlight == i + 1) /* High light the present choice */
    {
      wattron(menu_win, A_REVERSE);
      mvwprintw(menu_win, y, x, "%s", file_list[i]);
      mvwprintw(menu_win, y, x + OFFSET, "%jd", file_size_list[i]);
      wattroff(menu_win, A_REVERSE);
    } else {
      mvwprintw(menu_win, y, x, "%s", file_list[i]);
      mvwprintw(menu_win, y, x + OFFSET, "%jd", file_size_list[i]);
    }
    ++y;
  }
  wrefresh(menu_win);
}
int main() {
  WINDOW *menu_win;
  int choice = 0;
  int c;

  initscr();
  curs_set(0);
  clear();
  menu_win = newwin(LINES, COLS, starty, startx);
  keypad(menu_win, true); // Arrow suuport
  noecho();
  // break(); /* Line buffering disabled. pass on everything */

  while (1) {
    wclear(menu_win);
    get_dir_info(menu_win);
    clear();
    print_menu(menu_win, highlight);
    c = wgetch(menu_win);
    switch (c) {
    case 'q':
      endwin();
      return 0;
      break;
    case KEY_UP:
      if (highlight == 1)
        highlight = file_list_lenght;
      else
        --highlight;
      break;
    case 'k': // Same as KEY_UP
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
    case 'j':
      if (highlight == file_list_lenght)
        highlight = 1;
      else
        ++highlight;
      break;
    case 'l':
      endwin();
      return 0;
      break;
    case 'h':
      chdir("..");
      highlight = 1;
      break;
    default:
      refresh();
      break;
    }
  }
}
