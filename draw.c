#include <curses.h>
#include <dirent.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void setup();
void dir_forward(WINDOW *menu_win);
void get_dir_info(WINDOW *menu_win);
void print_menu(WINDOW *menu_win, int highlight);
int handle_input(WINDOW *menu_win);

#define MAX_FILE_NUMBER 100
#define MAX_FILE_NAME_SIZE 100
#define OFFSET 40

char *file_list[MAX_FILE_NUMBER];
intmax_t file_size_list[MAX_FILE_NUMBER] = {};
int file_list_lenght = 0;

int highlight = 1;

int main() {
  WINDOW *menu_win;
  setup();
  menu_win = newwin(LINES, COLS, 0, 0);
  keypad(menu_win, true); // Arrows

  int state = 1;
  while (true) {
    if (state == 1)
      get_dir_info(menu_win);
    wclear(menu_win);
    print_menu(menu_win, highlight);

    state = handle_input(menu_win);
    if (state == 2)
      return 0;
  }
}

void setup() {
  initscr();
  curs_set(0);
  clear();
  noecho();
}

void dir_forward(WINDOW *menu_win) {
  char *selection = file_list[(highlight - 1)];
  chdir(selection);
}

void get_dir_info(WINDOW *menu_win) {
  DIR *d;
  struct dirent *dir;
  struct stat stat_buffer;

  // Free up string array
  for (int i = 0; i < file_list_lenght; i++) {
    free(file_list[i]);
  }

  d = opendir(".");
  int iter = 0;
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

// Return values
// 0 -> nothing
// 1 -> reindex dirs
// 2 -> close
int handle_input(WINDOW *menu_win) {
  int input_char = wgetch(menu_win);
  switch (input_char) {
  case 'q':
    endwin();
    return 2;
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
    dir_forward(menu_win);
    highlight = 1;
    return 1;
    break;
  case 'h':
    chdir("..");
    highlight = 1;
    return 1;
    break;
  default:
    refresh();
    break;
  }
  return 0;
}

void print_menu(WINDOW *menu_win, int highlight) {
  int x, y, i;

  x = 2;
  y = 2;
  wattroff(menu_win, A_REVERSE);
  box(menu_win, 0, 0);
  for (i = 0; i < file_list_lenght; ++i) {
    if (highlight == i + 1) /* High light the present choice */
      wattron(menu_win, A_REVERSE);
    mvwprintw(menu_win, y, x, "%s", file_list[i]);
    mvwprintw(menu_win, y, x + OFFSET, "%jd", file_size_list[i]);
    wattroff(menu_win, A_REVERSE);
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
