#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <unistd.h>

typedef struct {
    chtype left_top;
    chtype right_top;
    chtype left_bottom;
    chtype right_bottom;
} Corner;

typedef struct {
    int width;
    int height;
    int begin_x;
    int begin_y;
    Corner corner;
} Win;

void draw_win(Win*);
void handle_sigwinch(int signum);
Corner* corner_list;

int main(int argc, char **argv) {

    // todo: why
    setlocale(LC_ALL, "");
    // todo: kitty vs xterm-kitty
    setenv("TERM", "kitty", 1);

    initscr();
    noecho();
    curs_set(0);
    cbreak();
    keypad(stdscr, TRUE);
    refresh();

    Corner border_list[] = {
        { .left_top = ACS_ULCORNER, .right_top = ACS_TTEE,     .left_bottom = ACS_LLCORNER, .right_bottom = ACS_BTEE,     },
        { .left_top = ACS_TTEE,     .right_top = ACS_TTEE,     .left_bottom = ACS_BTEE,     .right_bottom = ACS_BTEE,     },
        { .left_top = ACS_TTEE,     .right_top = ACS_URCORNER, .left_bottom = ACS_BTEE,     .right_bottom = ACS_LRCORNER, }
    };
    corner_list = border_list;

    // todo: when LINES or COLS too small
    int maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);
    int begin_x0 = 0, begin_x1 = maxx / 8, begin_x2 = maxx / 2 - 1;

    Win w0 = {
        .corner = border_list[0],
        .width = begin_x1 - begin_x0 + 1,
        .height = maxy - 2,
        .begin_x = begin_x0,
        .begin_y = 1,
    };
    Win w1 = {
        .corner = border_list[1],
        .width = begin_x2 - begin_x1 + 1,
        .height = maxy - 2,
        .begin_x = begin_x1,
        .begin_y = 1,
    };
    Win w2 = {
        .corner = border_list[2],
        .width = maxx - begin_x2,
        .height = maxy - 2,
        .begin_x = begin_x2,
        .begin_y = 1,
    };

    mvprintw(0, 0, "maxy = %d, maxx = %d; w0(%d,%d), w1(%d,%d), w2(%d,%d)", maxy, maxx, w0.begin_x, w0.begin_x+w0.width-1, w1.begin_x, w1.begin_x+w1.width-1, w2.begin_x, w2.begin_x+w2.width-1);
    mvprintw(maxy-1, 0, "w0(%d), w1(%d), w2(%d)", w0.width, w1.width, w2.width);

    draw_win(&w0);
    draw_win(&w1);
    draw_win(&w2);

    signal(SIGWINCH, handle_sigwinch);

    int ch;
    while ((ch = wgetch(stdscr)) != 'q') {
        // if ( ch == KEY_RESIZE ) {
        //     handle_sigwinch(0);
        // }
    }
    endwin();
}

// todo: use mvaddch,mvhline,mvvline instead
void draw_win(Win* w) {
    WINDOW* win = newwin(w->height, w->width, w->begin_y, w->begin_x);
    wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, w->corner.left_top, w->corner.right_top, w->corner.left_bottom, w->corner.right_bottom);
    wrefresh(win);
    delwin(win);
}

void handle_sigwinch(int signum) {
    if(!isatty(STDOUT_FILENO)) {
        return;
    }

    struct winsize win;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &win) < 0) {
        return;
    }

    resizeterm(win.ws_row, win.ws_col);

    int maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);
    int begin_x0 = 0, begin_x1 = maxx / 8, begin_x2 = maxx / 2 - 1;
    Corner* border_list = corner_list;

    Win w0 = {
        .corner = border_list[0],
        .width = begin_x1 - begin_x0 + 1,
        .height = maxy - 2,
        .begin_x = begin_x0,
        .begin_y = 1,
    };
    Win w1 = {
        .corner = border_list[1],
        .width = begin_x2 - begin_x1 + 1,
        .height = maxy - 2,
        .begin_x = begin_x1,
        .begin_y = 1,
    };
    Win w2 = {
        .corner = border_list[2],
        .width = maxx - begin_x2,
        .height = maxy - 2,
        .begin_x = begin_x2,
        .begin_y = 1,
    };

    move(0, 0);
    clrtoeol();
    mvprintw(0, 0, "maxy = %d, maxx = %d; w0(%d,%d), w1(%d,%d), w2(%d,%d)", maxy, maxx, w0.begin_x, w0.begin_x+w0.width-1, w1.begin_x, w1.begin_x+w1.width-1, w2.begin_x, w2.begin_x+w2.width-1);
    mvprintw(maxy-1, 0, "w0(%d), w1(%d), w2(%d)", w0.width, w1.width, w2.width);
    refresh();

    draw_win(&w0);
    draw_win(&w1);
    draw_win(&w2);
}
