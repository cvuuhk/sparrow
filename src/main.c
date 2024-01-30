#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

void highlight_pixel(int y, int x);
void clear_pixel(int y, int x);

int main(int argc, char **argv) {

    initscr();
    noecho();
    curs_set(0);
    cbreak();
    refresh();
    keypad(stdscr, TRUE);

    mousemask(BUTTON1_CLICKED | BUTTON3_CLICKED, NULL);

    int ch;
    while ((ch = getch()) != 'q') {
        if (ch != KEY_MOUSE) {
            continue;
        }

        MEVENT event;
        if (OK != getmouse(&event)) {
            continue;
        }

        if (event.bstate & BUTTON1_CLICKED) {
            highlight_pixel(event.y, event.x);
        } else if (event.bstate & BUTTON3_CLICKED) {
            clear_pixel(event.y, event.x);
        } else {
            printw("shit");
        }

        refresh();
    }
    endwin();
}


void highlight_pixel(int y, int x) {
    attron(A_REVERSE);
    mvprintw(y, x, " ");
    attroff(A_REVERSE);
}

void clear_pixel(int y, int x) {
    mvprintw(y, x, " ");
}
