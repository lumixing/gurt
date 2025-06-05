#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>
#include <string.h>

#define panic(fmt, ...) do { \
	endwin(); \
    printf("panic(%s:%d): " fmt "\n", \
            __FILE__, __LINE__, ##__VA_ARGS__); \
    exit(1); \
} while(0)

// make sure to free `out`
char *str_span(char *str, size_t len) {
	char *out = malloc(len+1);
	strcpy(out, str);

	if (strlen(str) < len) { // expand
		for (int i = strlen(str); i < len; i++) {
			out[i] = ' ';
		}
		out[len] = '\0';
	} else { // trunc
		out[len - 3] = '.';
		out[len - 2] = '.';
		out[len - 1] = '.';
		out[len] = '\0';
	}

	return out;
}

int main() {
	initscr();

	curs_set(0);
	noecho();
	cbreak();
	nodelay(stdscr, TRUE);

	int px = 0;
	int py = 0;
	bool update = true;

	char *footer = malloc(1024);
	footer = strcpy(footer, "footer bitches!!1");

	char *list[] = {
		"apple",
		"banana", 
		"orange",
		"grapeeeeeeeeeeee",
	};

	size_t list_len = 4;
	size_t list_len_r = 4 * 10;
	int list_idx = 0;
	int list_idx_r = 0;

	bool is_left = true;;

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
				char *spanned_list = str_span(list[i], x / 3);

				if (list_idx == i && is_left) attron(A_REVERSE);
				printw("%s\n", spanned_list);
				if (list_idx == i && is_left) attroff(A_REVERSE);

				free(spanned_list);
			}

			if (list_idx_r >= list_len_r - (y - 2 - 3) / 2 - 1) { // bot
				int s = list_len_r - (y - 2 - 3);
				for (int i = s; i < list_len_r; i++) {
					int j = i % list_len;
					char *spanned_list = str_span(list[j], 2 * x / 3 - 10);

					if (list_idx_r == i && !is_left) attron(A_REVERSE);
					mvprintw(i + 2 - s, x / 3 + 1, "%d->%s", i, spanned_list);
					if (list_idx_r == i && !is_left) attroff(A_REVERSE);

					free(spanned_list);
				}	
			} else if (list_idx_r >= (y - 2 - 3) / 2) { // mid
				int s = -((y - 2 - 3) / 2 - list_idx_r) + 1;
				for (int i = s; i < s + y - 2 - 3; i++) {
					int j = i % list_len;
					char *spanned_list = str_span(list[j], 2 * x / 3 - 10);

					if (list_idx_r == i && !is_left) attron(A_REVERSE);
					mvprintw(i + 2 - s, x / 3 + 1, "%d->%s", i, spanned_list);
					if (list_idx_r == i && !is_left) attroff(A_REVERSE);

					free(spanned_list);
				}
			} else { // top
				for (int i = 0; i < y - 2 - 3; i++) {
					// if (i + 2 >= y - 3)
					// 	break;

					int j = i % list_len;
					char *spanned_list = str_span(list[j], 2 * x / 3 - 10);

					if (list_idx_r == i && !is_left) attron(A_REVERSE);
					mvprintw(i + 2, x / 3 + 1, "%d->%s", i, spanned_list);
					if (list_idx_r == i && !is_left) attroff(A_REVERSE);

					free(spanned_list);
				}
			}

			if (list_idx_r >= list_len_r - (y - 2 - 3) / 2 - 1)
				sprintf(footer, "bot list_idx_r -> %d (start from %zu)", list_idx_r, list_len_r - (y - 2 - 3));
			else if (list_idx_r >= (y - 2 - 3) / 2)
				sprintf(footer, "mid list_idx_r -> %d (bot %d) (start from %d)", list_idx_r, y - 2 - 3, -((y - 2 - 3) / 2 - list_idx_r) + 1);
			else
				sprintf(footer, "top list_idx_r -> %d", list_idx_r);

			attron(A_REVERSE);
			char *spanned_footer = str_span(footer, x);
			mvprintw(y - 1, 0, "%s", spanned_footer);
			free(spanned_footer);
			attroff(A_REVERSE);

			refresh();
		}
		
		timeout(16);
		int ch = getch();
		if (ch == 'q') {
			break;
		} else if (ch == ' ') {
			update = true;
			strcpy(footer, "manually updated!");
		} else if (ch == 'w') {
			if (is_left) {
				list_idx -= 1;
				if (list_idx < 0) list_idx = list_len - 1;
			} else {
				list_idx_r -= 1;
				if (list_idx_r < 0) list_idx_r = list_len_r - 1;
			}
			update = true;
		} else if (ch == 's') {
			if (is_left) {
				list_idx += 1;
				if (list_idx >= list_len) list_idx = 0;
			} else {
				list_idx_r += 1;
				if (list_idx_r >= list_len_r) list_idx_r = 0;
			}
			update = true;
		} else if (ch == 'e') {
			sprintf(footer, "selected %s", list[list_idx]);
			update = true;
		} else if (ch == 'd' || ch == 'a') {
			is_left = !is_left;
			update = true;
		}
	}
	
	free(footer);
	endwin();

	return 0;
}
