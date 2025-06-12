#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT 256

WINDOW* draw_status_bar() {

    int width = COLS;
    int height = 1;
    int starty = LINES - 1;

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);

    WINDOW *bar = newwin(height, width, starty, 0);
    wbkgd(bar, COLOR_PAIR(1));
    werase(bar);

    mvwprintw(bar, 0, 0,   "Mode: NORMAL");
    mvwprintw(bar, 0, 20,  "Status: READY");
    mvwprintw(bar, 0, 45,  "Press ':' to enter command");

    wrefresh(bar);
    return bar;
}

char* get_command_input(WINDOW *bar, int* cancelled) {
    char *input = malloc(MAX_INPUT);
    memset(input, 0, MAX_INPUT);

    werase(bar);
    wmove(bar, 0, 0);
    wrefresh(bar);

    echo();
    curs_set(1);
    mvwprintw(bar, 0, 0, ":");
    wrefresh(bar);

    int i = 0;
    int ch;
    while ((ch = wgetch(bar)) != '\n') {
        if (ch == 27) {  // ESC tuşu
            *cancelled = 1;
            break;
        } else if (ch == KEY_BACKSPACE || ch == 127 || ch == '\b') {
            if (i > 0) {
                i--;
                input[i] = '\0';
                mvwprintw(bar, 0, i+1, " "); // karakteri sil
                wmove(bar, 0, i+1);
                wrefresh(bar);
            }
        } else if (i < MAX_INPUT - 1 && ch >= 32 && ch <= 126) {
            input[i++] = ch;
            mvwprintw(bar, 0, i, "%c", ch);
            wrefresh(bar);
        }
    }

    noecho();
    curs_set(0);
    werase(bar);
    wrefresh(bar);

    if (*cancelled) {
        free(input);
        return NULL;
    }

    return input;
}

int main() {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);

    // Başlık ve açıklama
    mvprintw(0, 0, "Vim-like editor is running. Press ':' to enter command, 'q' to quit.");
    refresh();

    WINDOW *status_bar = draw_status_bar();
    int ch;
    char *command = NULL;

    while ((ch = getch()) != 'q') {
        if (ch == ':') {
            int cancelled = 0;
            command = get_command_input(status_bar, &cancelled);

            if (!cancelled && command && strlen(command) > 0) {
                // Komutu üst kısımda göster
                move(1, 0);
                clrtoeol();
                printw("Son komut: :%s", command);
                refresh();
                free(command);
            }

            // Cmd bar'ı yeniden çiz
            werase(status_bar);
            wrefresh(status_bar);
            delwin(status_bar);
            status_bar = draw_status_bar();
        }
    }

    endwin();
    return 0;
}

