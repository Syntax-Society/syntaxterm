#include <curses.h>
#include "common.h"
#include "context.h"
#include "views/checkin.h"
#include "views/main_menu.h"
//#include "admin_menu.h"
//#include "about.h"

int main_menu(Context_t *context) {
	int running = 1;

	while (running) {
		print_title(context, "Huvudmeny", NULL, NULL);

		show_banner(context);
		fkey_hints(
			context,
			"incheck",
			"admin",
			"",
			"",
			"",
			"",
			"om",
			"avsluta"
			);

		wclear(context->main);
		wrefresh(context->main);

		int c = wgetch(context->bottom);
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
