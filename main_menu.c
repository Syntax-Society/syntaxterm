#include <curses.h>
#include "common.h"
#include "context.h"
#include "main_menu.h"
#include "checkin.h"
//#include "admin_menu.h"
//#include "about.h"

int main_menu(Context_t *context) {
	int running = 1;

	while (running) {
		print_title("Huvudmeny", NULL, NULL);

		show_file("header.txt");
		fkey_hints(
			"incheck",
			"admin",
			"",
			"",
			"",
			"",
			"om",
			"avsluta"
			);

		int c = wgetch(stdscr);
		switch (c) {
			case KEY_F(1):
				checkin(context);
				break;
			case KEY_F(2):
				//screen_admin();
				break;
			case KEY_F(7):
				//screen_about();
				break;
			case 'q':
			case KEY_F(8):
				running = 0;
				break;
		}
	}

	return 0;
}
