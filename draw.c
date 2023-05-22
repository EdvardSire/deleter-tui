#include <curses.h>
#include <dirent.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Declarations
void setup();
void dir_forward(WINDOW *menu_win);
void update_file_size(char *name, const int *iter);
void print_menu(WINDOW *menu_win, int highlight);
int handle_input(WINDOW *menu_win);
void get_dir_info(WINDOW *menu_win);

// Macros
#define MAX_FILE_NUMBER 100
#define MAX_FILE_NAME_SIZE 100
#define FILE_SIZE_OFFSET 40
#define LEFT_PADDING 2

// Global state instead of passing struct*'s around
char *file_names[MAX_FILE_NUMBER];
char *file_sizes[MAX_FILE_NUMBER];
int number_of_files = 0;
int current_selection = 1;

int main() {
  WINDOW *menu_win;
  setup();
  menu_win = newwin(LINES, COLS, 0, 0);
  keypad(menu_win, true); // Arrows

  int state = 1;
  while (true) {
    if (state == 1)
      get_dir_info(menu_win);
    print_menu(menu_win, current_selection);

    state = handle_input(menu_win);
    if (state == 2)
      return 0;
  }
}

void setup() {
  initscr();
  curs_set(0);
  noecho();
}

void dir_forward(WINDOW *menu_win) {
  char *selection = file_names[(current_selection - 1)];
  chdir(selection);
}

void update_file_size(char *name, const int *iter) {
  FILE *fp;
  char *buf[MAX_FILE_NAME_SIZE];
  char command[MAX_FILE_NAME_SIZE];
  strcpy(command, "sudo /usr/bin/du -bd0 -h ");
  strcat(command, name);
  strcat(command, " | awk '$0=$1'");
  fp = popen(command, "r");
  fgets(buf, sizeof(buf), fp);
  strcpy(file_sizes[*iter], buf);
  pclose(fp);
}

void get_dir_info(WINDOW *menu_win) {
  DIR *d;
  struct dirent *dir;

  // Free up string array
  for (int i = 0; i < number_of_files; i++) {
    free(file_names[i]);
    free(file_sizes[i]);
  }

  d = opendir(".");
  int iter = 0;
  number_of_files = 0;
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      file_names[iter] = malloc(MAX_FILE_NAME_SIZE * sizeof(char));
      strcpy(file_names[iter], dir->d_name);

      file_sizes[iter] = malloc(MAX_FILE_NAME_SIZE * sizeof(char));
      update_file_size(dir->d_name, &iter);

      number_of_files++;
      iter++;
    }
    closedir(d);
  }
}

void print_menu(WINDOW *menu_win, int highlight) {
  wclear(menu_win);

  int col = 2;
  for (int i = 0; i < number_of_files; ++i) {
    if (highlight == i + 1) /* High light the present choice */
      wattron(menu_win, A_REVERSE);
    mvwprintw(menu_win, col, LEFT_PADDING, "%s", file_names[i]);
    mvwprintw(menu_win, col, LEFT_PADDING + FILE_SIZE_OFFSET, "%s",
              file_sizes[i]);
    wattroff(menu_win, A_REVERSE);
    ++col;
  }
  box(menu_win, 0, 0);
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
    if (current_selection == 1)
      current_selection = number_of_files;
    else
      --current_selection;
    break;
  case 'k': // Same as KEY_UP
    if (current_selection == 1)
      current_selection = number_of_files;
    else
      --current_selection;
    break;
  case KEY_DOWN:
    if (current_selection == number_of_files)
      current_selection = 1;
    else
      ++current_selection;
    break;
  case 'j':
    if (current_selection == number_of_files)
      current_selection = 1;
    else
      ++current_selection;
    break;
  case 'l':
    dir_forward(menu_win);
    current_selection = 1;
    return 1;
  case 'h':
    chdir("..");
    current_selection = 1;
    return 1;
  }
  return 0;
}
