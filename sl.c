/* ====================================================================================== *

    sl.c: SL

        Ronald Chu
        based on TVG fork, 2019 Matthias Gatto
        based on version 5.03, Copyright 1993,1998,2014-2015 Toyoda Masashi (mtoyoda@acm.org)

 * ====================================================================================== *

    v6.00       Ronald Chu          2022/01/01      Added animation timing
                                                    Changed argv handling
                                                    Made TGV longer (makes it look faster)
                Matthias Gatto      2019/06/25      add -G option that add an orange TGV
    v5.03       Ryan Jacobs         2015/01/19      Fix some more compiler warnings
    v5.02       Jeff Schwab         2014/06/03      Fix compiler warnings
    v5.01       Chris Seymour       2014/01/03      removed cursor and handling of IO
    v5.00       Toyoda Masashi      2013/05/05      add -c option
    v4.00       Toyoda Masashi      2002/12/31      add C51, usleep(40000
    v3.03       Toyoda Masashi      1998/07/22      add usleep(20000
    v3.02       Toyoda Masashi      1993/01/19      D51 flies! Change options
    v3.01       Toyoda Masashi      1992/12/25      Wheel turns smoother
    v3.00       Toyoda Masashi      1992/12/24      Add d(D51) option
    v2.02       Toyoda Masashi      1992/12/17      Bug fixed.(dust remains in screen
    v2.01       Toyoda Masashi      1992/12/16      Smoke run and disappear.
                                                    Change '-a' to accident option.
    v2.00       Toyoda Masashi      1992/12/15      Add a(all),l(long),F(Fly!) options
    v1.02       Toyoda Masashi      1992/12/14      Add turning wheel
    v1.01       Toyoda Masashi      1992/12/14      Add more complex smoke
    v1.00       Toyoda Masashi      1992/12/11      SL runs vomiting out smoke

 * ====================================================================================== */

#include <curses.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h> // strtol
#include <string.h> // strcmp
#include <errno.h>  // errno
#include <time.h> // clock_gettime

#include "sl.h"

void add_smoke(int y, int x);
void add_man(int y, int x);
int add_C51(int x);
int add_D51(int x);
int add_TGV(int x);
int add_sl(int x);
int my_mvaddstr(int y, int x, char *str);

int ACCIDENT  = 0;
int FLY       = 0;
int TRAIN     = D51;

int my_mvaddstr(int y, int x, char *str)
{
    for ( ; x < 0; ++x, ++str)
        if (*str == '\0')  return ERR;
    for ( ; *str != '\0'; ++str, ++x)
        if (mvaddch(y, x, *str) == ERR)  return ERR;
    return OK;
}

int main(int argc, char *argv[])
{
    int x, ani_adjusted_delay;
    struct timespec start, stop;
    int ani_delay = 4000;

    // Get Start time, select random train
    clock_gettime(CLOCK_REALTIME, &start);
    TRAIN = 1 + (start.tv_sec % 4);
 
    // Parse argv
    for (x = 1; x < argc; ++x) {
        if (ani_delay == -1) {
            char *p;
            errno = 0;
            long conv = strtol(argv[x], &p, 10);
            if (errno != 0 || *p != '\0') return 2;
            if (conv > 60000) ani_delay = 60000; else
            if (conv < 1) ani_delay = 1; else
                ani_delay = conv;
        } 
        else if (strcmp(argv[x],"--time") == 0) {
            ani_delay = -1;
        } 
        else if (strcmp(argv[x], "--help") == 0) {
            printf("%s", HELPTEXT);
            return 0;
        }
        else if (*argv[x]++ == '-') {
            while (*argv[x] != '\0') {
                switch (*argv[x]++) {
                    case 'a': ACCIDENT = 1; break;
                    case 'F': FLY      = 1; break;
                    case 'l': TRAIN = LOGO; break;
                    case 'c': TRAIN = C51; break;
                    case 'G': TRAIN = TGV; break;
                    case 'd': TRAIN = D51; break;
                    case 't': ani_delay = -1; break;
                    default:  return 1;     break;
                }
            }
        }
        else
            return 1;
    }
    if (ani_delay == -1) return 3;
    
    // Init Screen
    initscr();
    signal(SIGINT, SIG_IGN);
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    leaveok(stdscr, TRUE);
    scrollok(stdscr, FALSE);

    // Set animation length
    int frames = 0;
    switch(TRAIN) {
        case LOGO: frames = LOGOLENGTH; break;
        case C51:  frames = C51LENGTH; break;
        case D51:  frames = D51LENGTH; break;
        case TGV:  frames = TGVLENGTH;
            if (has_colors()) {
                start_color();
                init_pair(1, COLOR_WHITE, COLOR_BLACK); // first Non prototype TGV was orange
                init_pair(2, COLOR_YELLOW, COLOR_BLACK);
            }
        break;
        default: return 201; break;
    }
    frames = frames + COLS;
    ani_delay = 1000 * ani_delay / frames;

    // Main Loop
    for (x = frames - 1; x >= 0 ; --x)
    {
        switch(TRAIN) {
            case LOGO: add_sl(x);  break;
            case C51:  add_C51(x); break;
            case TGV:  add_TGV(x); break;
            case D51:  add_D51(x); break;
        }
        getch();
        refresh();
        

        clock_gettime(CLOCK_REALTIME, &stop);
        ani_adjusted_delay = (frames - x) * ani_delay - (int)((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_nsec/1000 - start.tv_nsec/1000);
        if (ani_adjusted_delay > 1000000) {
            mvcur(0, COLS - 1, LINES - 1, 0);
            endwin();
            return 4;
        }
        if (ani_adjusted_delay > 0) usleep(ani_adjusted_delay);
    }

    // Cleanup
    mvcur(0, COLS - 1, LINES - 1, 0);
    endwin();
    return 0;
}


int add_sl(int x)
{
    static char *sl[LOGOPATTERNS][LOGOHEIGHT + 1]
        = {{LOGO1, LOGO2, LOGO3, LOGO4, LWHL11, LWHL12, DELLN},
           {LOGO1, LOGO2, LOGO3, LOGO4, LWHL21, LWHL22, DELLN},
           {LOGO1, LOGO2, LOGO3, LOGO4, LWHL31, LWHL32, DELLN},
           {LOGO1, LOGO2, LOGO3, LOGO4, LWHL41, LWHL42, DELLN},
           {LOGO1, LOGO2, LOGO3, LOGO4, LWHL51, LWHL52, DELLN},
           {LOGO1, LOGO2, LOGO3, LOGO4, LWHL61, LWHL62, DELLN}};

    static char *coal[LOGOHEIGHT + 1]
        = {LCOAL1, LCOAL2, LCOAL3, LCOAL4, LCOAL5, LCOAL6, DELLN};

    static char *car[LOGOHEIGHT + 1]
        = {LCAR1, LCAR2, LCAR3, LCAR4, LCAR5, LCAR6, DELLN};

    int i, y, py1 = 0, py2 = 0, py3 = 0;

    x -= LOGOLENGTH;
    if (x < - LOGOLENGTH)  return ERR;
    y = LINES / 2 - 3;

    if (FLY == 1) {
        y = (x / 6) + LINES - (COLS / 6) - LOGOHEIGHT;
        py1 = 2;  py2 = 4;  py3 = 6;
    }
    for (i = 0; i <= LOGOHEIGHT; ++i) {
        my_mvaddstr(y + i, x, sl[(LOGOLENGTH + x) / 3 % LOGOPATTERNS][i]);
        my_mvaddstr(y + i + py1, x + 21, coal[i]);
        my_mvaddstr(y + i + py2, x + 42, car[i]);
        my_mvaddstr(y + i + py3, x + 63, car[i]);
    }
    if (ACCIDENT == 1) {
        add_man(y + 1, x + 14);
        add_man(y + 1 + py2, x + 45);  add_man(y + 1 + py2, x + 53);
        add_man(y + 1 + py3, x + 66);  add_man(y + 1 + py3, x + 74);
    }
    add_smoke(y - 1, x + LOGOFUNNEL);
    return OK;
}


int add_D51(int x)
{
    static char *d51[D51PATTERNS][D51HEIGHT + 1]
        = {{D51STR1, D51STR2, D51STR3, D51STR4, D51STR5, D51STR6, D51STR7,
            D51WHL11, D51WHL12, D51WHL13, D51DEL},
           {D51STR1, D51STR2, D51STR3, D51STR4, D51STR5, D51STR6, D51STR7,
            D51WHL21, D51WHL22, D51WHL23, D51DEL},
           {D51STR1, D51STR2, D51STR3, D51STR4, D51STR5, D51STR6, D51STR7,
            D51WHL31, D51WHL32, D51WHL33, D51DEL},
           {D51STR1, D51STR2, D51STR3, D51STR4, D51STR5, D51STR6, D51STR7,
            D51WHL41, D51WHL42, D51WHL43, D51DEL},
           {D51STR1, D51STR2, D51STR3, D51STR4, D51STR5, D51STR6, D51STR7,
            D51WHL51, D51WHL52, D51WHL53, D51DEL},
           {D51STR1, D51STR2, D51STR3, D51STR4, D51STR5, D51STR6, D51STR7,
            D51WHL61, D51WHL62, D51WHL63, D51DEL}};
    static char *coal[D51HEIGHT + 1]
        = {COAL01, COAL02, COAL03, COAL04, COAL05,
           COAL06, COAL07, COAL08, COAL09, COAL10, COALDEL};

    int y, i, dy = 0;

    x -= D51LENGTH;
    if (x < - D51LENGTH)  return ERR;
    y = LINES / 2 - 5;

    if (FLY == 1) {
        y = (x / 7) + LINES - (COLS / 7) - D51HEIGHT;
        dy = 1;
    }
    for (i = 0; i <= D51HEIGHT; ++i) {
        my_mvaddstr(y + i, x, d51[(D51LENGTH + x) % D51PATTERNS][i]);
        my_mvaddstr(y + i + dy, x + 53, coal[i]);
    }
    if (ACCIDENT == 1) {
        add_man(y + 2, x + 43);
        add_man(y + 2, x + 47);
    }
    add_smoke(y - 1, x + D51FUNNEL);
    return OK;
}

int add_TGV(int x)
{
    static char *tgv[TGVPATTERNS][TGVHEIGHT + 1]
      = {{TGVSTR0, TGVSTR1, TGVSTR2, TGVSTR3, TGVSTR4, TGVSTR5, TGVSTR6,
            TGVWHL1, TGVDEL},
           {TGVSTR0, TGVSTR1, TGVSTR2, TGVSTR3, TGVSTR4, TGVSTR5, TGVSTR6,
            TGVWHL2, TGVDEL}};
    static char *vagoon[TGVHEIGHT + 1]
        = {TGVVAG0, TGVVAG1, TGVVAG2, TGVVAG3, TGVVAG4, TGVVAG5, TGVVAG6, TGVVAG7, TGVDEL};

    int y, i, dy = 0;

    x -= TGVLENGTH;
    if (x < - TGVLENGTH)  return ERR;
    y = LINES / 2 - 5;

    if (FLY == 1) {
        y = (x / 7) + LINES - (COLS / 7) - TGVHEIGHT;
        dy = 1;
    }
    if (has_colors()) attron(COLOR_PAIR(2));
    for (i = 0; i <= TGVHEIGHT; ++i) {
        my_mvaddstr(y + i, x, tgv[((TGVLENGTH + x) / 2) % TGVPATTERNS][i]);
        my_mvaddstr(y + i + dy, x + 55, vagoon[i]);
    }
    attroff(COLOR_PAIR(2));

    if (ACCIDENT == 1) {
        add_man(y + 2, x + 14);

        add_man(y + dy + 3, x + 85);
        add_man(y + dy + 3, x + 91);
        add_man(y + dy + 3, x + 103);
        add_man(y + dy + 3, x + 114);
        add_man(y + dy + 3, x + 146);
        add_man(y + dy + 3, x + 152);
    }
    return OK;
}

int add_C51(int x)
{
    static char *c51[C51PATTERNS][C51HEIGHT + 1]
        = {{C51STR1, C51STR2, C51STR3, C51STR4, C51STR5, C51STR6, C51STR7,
            C51WH11, C51WH12, C51WH13, C51WH14, C51DEL},
           {C51STR1, C51STR2, C51STR3, C51STR4, C51STR5, C51STR6, C51STR7,
            C51WH21, C51WH22, C51WH23, C51WH24, C51DEL},
           {C51STR1, C51STR2, C51STR3, C51STR4, C51STR5, C51STR6, C51STR7,
            C51WH31, C51WH32, C51WH33, C51WH34, C51DEL},
           {C51STR1, C51STR2, C51STR3, C51STR4, C51STR5, C51STR6, C51STR7,
            C51WH41, C51WH42, C51WH43, C51WH44, C51DEL},
           {C51STR1, C51STR2, C51STR3, C51STR4, C51STR5, C51STR6, C51STR7,
            C51WH51, C51WH52, C51WH53, C51WH54, C51DEL},
           {C51STR1, C51STR2, C51STR3, C51STR4, C51STR5, C51STR6, C51STR7,
            C51WH61, C51WH62, C51WH63, C51WH64, C51DEL}};
    static char *coal[C51HEIGHT + 1]
        = {COALDEL, COAL01, COAL02, COAL03, COAL04, COAL05,
           COAL06, COAL07, COAL08, COAL09, COAL10, COALDEL};

    int y, i, dy = 0;

    x -= C51LENGTH;
    if (x < - C51LENGTH)  return ERR;
    y = LINES / 2 - 5;

    if (FLY == 1) {
        y = (x / 7) + LINES - (COLS / 7) - C51HEIGHT;
        dy = 1;
    }
    for (i = 0; i <= C51HEIGHT; ++i) {
        my_mvaddstr(y + i, x, c51[(C51LENGTH + x) % C51PATTERNS][i]);
        my_mvaddstr(y + i + dy, x + 55, coal[i]);
    }
    if (ACCIDENT == 1) {
        add_man(y + 3, x + 45);
        add_man(y + 3, x + 49);
    }
    add_smoke(y - 1, x + C51FUNNEL);
    return OK;
}


void add_man(int y, int x)
{
    static char *man[2][2] = {{"", "(O)"}, {"Help!", "\\O/"}};
    int i;

    for (i = 0; i < 2; ++i) {
        my_mvaddstr(y + i, x, man[(LOGOLENGTH + x) / 12 % 2][i]);
    }
}


void add_smoke(int y, int x)
#define SMOKEPTNS        16
{
    static struct smokes {
        int y, x;
        int ptrn, kind;
    } S[1000];
    static int sum = 0;
    static char *Smoke[2][SMOKEPTNS]
        = {{"(   )", "(    )", "(    )", "(   )", "(  )",
            "(  )" , "( )"   , "( )"   , "()"   , "()"  ,
            "O"    , "O"     , "O"     , "O"    , "O"   ,
            " "                                          },
           {"(@@@)", "(@@@@)", "(@@@@)", "(@@@)", "(@@)",
            "(@@)" , "(@)"   , "(@)"   , "@@"   , "@@"  ,
            "@"    , "@"     , "@"     , "@"    , "@"   ,
            " "                                          }};
    static char *Eraser[SMOKEPTNS]
        =  {"     ", "      ", "      ", "     ", "    ",
            "    " , "   "   , "   "   , "  "   , "  "  ,
            " "    , " "     , " "     , " "    , " "   ,
            " "                                          };
    static int dy[SMOKEPTNS] = { 2,  1, 1, 1, 0, 0, 0, 0, 0, 0,
                                 0,  0, 0, 0, 0, 0             };
    static int dx[SMOKEPTNS] = {-2, -1, 0, 1, 1, 1, 1, 1, 2, 2,
                                 2,  2, 2, 3, 3, 3             };
    int i;

    if (x % 4 == 0) {
        for (i = 0; i < sum; ++i) {
            my_mvaddstr(S[i].y, S[i].x, Eraser[S[i].ptrn]);
            S[i].y    -= dy[S[i].ptrn];
            S[i].x    += dx[S[i].ptrn];
            S[i].ptrn += (S[i].ptrn < SMOKEPTNS - 1) ? 1 : 0;
            my_mvaddstr(S[i].y, S[i].x, Smoke[S[i].kind][S[i].ptrn]);
        }
        my_mvaddstr(y, x, Smoke[sum % 2][0]);
        S[sum].y = y;    S[sum].x = x;
        S[sum].ptrn = 0; S[sum].kind = sum % 2;
        sum ++;
    }
}
