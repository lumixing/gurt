#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>

#define panic(fmt, ...) do { \
	endwin(); \
    printf("panic(%s:%d): " fmt "\n", \
            __FILE__, __LINE__, ##__VA_ARGS__); \
    exit(1); \
} while(0)

int main() {
	initscr();

	curs_set(0);
	noecho();
	cbreak();
	nodelay(stdscr, TRUE);

	start_color();
	init_pair(1, COLOR_BLACK, COLOR_WHITE);

	int px = 0;
	int py = 0;
	bool update = true;

	char *footer = "footer bitches";

	char *list[] = {
		"apple",
		"banana", 
		"orange",
		"grape",
	};

	size_t list_len = 4;
	int list_idx = 0;

	while (1) {
		int x, y;
		getmaxyx(stdscr, y, x);

		if (x != px || y != py || update) {
			px = x;
			py = y;
			update = false;
			clear();
			printw("%d\n", rand());
			printw("%dx%d\n", x, y);
			for (int i = 0; i < list_len; i++) {
				if (list_idx == i) attron(COLOR_PAIR(1));
				printw("%d->%s\n", i, list[i]);
				if (list_idx == i) attroff(COLOR_PAIR(1));
			}
			attron(COLOR_PAIR(1));
			for (int i = 0; i < x; i++)
				mvaddch(y - 1, i, ' ');
			mvprintw(y - 1, 0, "%s", footer);
			attroff(COLOR_PAIR(1));
			refresh();
		}
		
		timeout(16);
		int ch = getch();
		if (ch == 'q') {
			break;
		} else if (ch == ' ') {
			update = true;
			footer = "manually updated!";
		} else if (ch == 'w') {
			list_idx -= 1;
			if (list_idx < 0) list_idx = list_len - 1;
			update = true;
		} else if (ch == 's') {
			list_idx += 1;
			if (list_idx >= list_len) list_idx = 0;
			update = true;
		} else if (ch == 'e') {
			sprintf(footer, "selected %s", list[list_idx]);
			update = true;
		}
	}
	
	endwin();

	return 0;
}
