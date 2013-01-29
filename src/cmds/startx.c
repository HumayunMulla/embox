/**
 * @file
 * @brief Here is the entry point for XWnd.
 *
 * @date Oct 19, 2012
 * @author Alexandr Chernakov
 */


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <kernel/time/timer.h>
#include <kernel/task.h>
#include <drivers/video/vesa.h>
#include <drivers/keyboard.h>
#include <embox/cmd.h>

#include <xwnd/xappreg.h>
#include <lib/xwnd/application.h>

#include <drivers/video/display.h>

static int exec(int argc, char ** argv);

EMBOX_CMD(exec);

static struct display display;

struct display *display_get(void) {
	return &display;
}

int xwnd_init(void) {
#if 0
	if(NULL == vga_setup_mode(&display, 0x13)) {
		return -1;
	}

	display_clear_screen(&display);
#else
	/* TEST for setpixel */
	extern struct display * get_bochs_display(void);
	size_t i, j;
	struct display *displ;

	displ = display_get_default();
	displ->ops->setup(displ, 1280, 1024, DISPLAY_MODE_DEPTH16);

	for (i = 0; i < displ->width / 2; ++i) {
		for (j = 0; j < displ->height / 2; ++j) {
			displ->ops->set_pixel(displ, i, j, 0xF000);
		}
		for (; j < displ->height; ++j) {
			displ->ops->set_pixel(displ, i, j, 0x0F00);
		}
	}
	for (; i < displ->width; ++i) {
		for (j = 0; j < displ->height / 2; ++j) {
			displ->ops->set_pixel(displ, i, j, 0xF0F0);
		}
		for (; j < displ->height; ++j) {
			displ->ops->set_pixel(displ, i, j, 0x000F);
		}
	}
#endif
	return 0;
}


void xwnd_quit(void){
	display_clear_screen(&display);

	vga_setup_mode(&display, 0x3);
}

static int exec (int argc, char ** argv) {
	int err, i;

	if (argc < 2) {
		for (i = 0; i < ARRAY_SPREAD_SIZE(__xwnd_app_repository); i++) {
			printf("%s\n", __xwnd_app_repository[i].app_name);
		}
		return 0;
	} else {
		xwnd_init();


		if (argc < 3) {
			err = xwnd_app_start((const char *)argv[1], NULL);
		} else {
			err = xwnd_app_start((const char *)argv[1], argv[2]);
		}
		if (err) {
			xwnd_quit();
			return 1;
		}

		while (1) {
			if (!keyboard_has_symbol()) {
				msleep(100);
			}
			else {
				char key = keyboard_getc();
				if ('q' == key) {
					xwnd_app_registry_broadcast_quit_event();
					sleep(1);
					break;
				} else if ('a' == key) {
					xwnd_app_registry_move_focus();
				} else {
					xwnd_app_registry_send_kbd_event(key);
					xwnd_app_registry_send_sys_event(XEV_DRAW);
				}
			}
		}
		xwnd_quit();
	}

	return 0;
}
