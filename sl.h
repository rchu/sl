/* ====================================================================================== *

    sl.c: SL
    
        Ronald Chu
        based on TVG fork, 2019 Matthias Gatto
        based on version 5.03, Copyright 1993,1998,2014-2015 Toyoda Masashi (mtoyoda@acm.org)

 * ====================================================================================== */
#define HELPTEXT  "\
SL (Steam Locomotive) runs across your terminal when you type \"sl\" as you meant to type \"ls\". It's just a joke command, and not useful at all.\n\
\n\
Usage:\n\
    sl <options>\n\
Options:\n\
    -a               Accident\n\
    -F               Fly\n\
    -d               Show logo\n\
    -c               Show C51\n\
    -G               Show TGV\n\
    -t <time>\n\
    --time <time>    Set animation time (miliseconds)\n\n"

#define TGV 1
#define TGVHEIGHT	8
#define TGVLENGTH	249
#define TGVPATTERNS	2

#define TGVSTR0  "                             ______________   /          "
#define TGVSTR1  "                ____________/______________|__\\_______   "
#define TGVSTR2  "          _____/  |IIIIIIIIIII|_______________________|  "
#define TGVSTR3  "       __/__/_|_|       ______________________________|  "
#define TGVSTR4  "    __/       | |      /         ||||||||||||  TGV || |  "
#define TGVSTR5  "   />  []     | |    /___________||||||||||||______|| |  "
#define TGVSTR6  "  \\_==================================================|  "
#define TGVWHL1  "    |___/==0==0==|_________________________/===0==0==\\|  "

#define TGVWHL2  "    |___/0==0==0=|_________________________/=0==0==0=\\|  "

#define TGVDEL   "                                                         "

#define TGVVAG0  "  ___________                                                                                                                                                                                   "
#define TGVVAG1  "  |_====____|\\________________________________________   ____________________________________________________        __________                      _____                                      "
#define TGVVAG2  "o|  ||||         ___                                  |o|               ___                                  |                           _____                              ___                 "
#define TGVVAG3  "H|______________|___|_________________________________|H|______________|___|_________________________________|   __________                                                                     "
#define TGVVAG4  "H|  |||| |[]|   |[]1|   []   [   ] [   ] [   ] [   ]  |H| [   ]   []   |[]2|   []   [   ] [   ] [   ] [   ]  |                                                                             ___  "
#define TGVVAG5  "H|__||||_|__|___|___|_________________________________|H|______________|___|_________________________________|                                                 ____                             "
#define TGVVAG6  "D|==============|===|=================================|D|==============|===|=================================|        __________               _____                                            "
#define TGVVAG7  " _/ 0==0  |____________________________________/    0=| _/ 0==0  |____________________________________/    0=|                                                                                  "

#define D51 2
#define D51HEIGHT	10
#define D51FUNNEL	 7
#define D51LENGTH	83
#define D51PATTERNS	 6

#define D51STR1  "      ====        ________                ___________ "
#define D51STR2  "  _D _|  |_______/        \\__I_I_____===__|_________| "
#define D51STR3  "   |(_)---  |   H\\________/ |   |        =|___ ___|   "
#define D51STR4  "   /     |  |   H  |  |     |   |         ||_| |_||   "
#define D51STR5  "  |      |  |   H  |__--------------------| [___] |   "
#define D51STR6  "  | ________|___H__/__|_____/[][]~\\_______|       |   "
#define D51STR7  "  |/ |   |-----------I_____I [][] []  D   |=======|__ "

#define D51WHL11 "__/ =| o |=-~~\\  /~~\\  /~~\\  /~~\\ ____Y___________|__ "
#define D51WHL12 " |/-=|___|=    ||    ||    ||    |_____/~\\___/        "
#define D51WHL13 "  \\_/      \\O=====O=====O=====O_/      \\_/            "

#define D51WHL21 "__/ =| o |=-~~\\  /~~\\  /~~\\  /~~\\ ____Y___________|__ "
#define D51WHL22 " |/-=|___|=O=====O=====O=====O   |_____/~\\___/        "
#define D51WHL23 "  \\_/      \\__/  \\__/  \\__/  \\__/      \\_/            "

#define D51WHL31 "__/ =| o |=-O=====O=====O=====O \\ ____Y___________|__ "
#define D51WHL32 " |/-=|___|=    ||    ||    ||    |_____/~\\___/        "
#define D51WHL33 "  \\_/      \\__/  \\__/  \\__/  \\__/      \\_/            "

#define D51WHL41 "__/ =| o |=-~O=====O=====O=====O\\ ____Y___________|__ "
#define D51WHL42 " |/-=|___|=    ||    ||    ||    |_____/~\\___/        "
#define D51WHL43 "  \\_/      \\__/  \\__/  \\__/  \\__/      \\_/            "

#define D51WHL51 "__/ =| o |=-~~\\  /~~\\  /~~\\  /~~\\ ____Y___________|__ "
#define D51WHL52 " |/-=|___|=   O=====O=====O=====O|_____/~\\___/        "
#define D51WHL53 "  \\_/      \\__/  \\__/  \\__/  \\__/      \\_/            "

#define D51WHL61 "__/ =| o |=-~~\\  /~~\\  /~~\\  /~~\\ ____Y___________|__ "
#define D51WHL62 " |/-=|___|=    ||    ||    ||    |_____/~\\___/        "
#define D51WHL63 "  \\_/      \\_O=====O=====O=====O/      \\_/            "

#define D51DEL   "                                                      "

#define COAL01 "                              "
#define COAL02 "                              "
#define COAL03 "    _________________         "
#define COAL04 "   _|                \\_____A  "
#define COAL05 " =|                        |  "
#define COAL06 " -|                        |  "
#define COAL07 "__|________________________|_ "
#define COAL08 "|__________________________|_ "
#define COAL09 "   |_D__D__D_|  |_D__D__D_|   "
#define COAL10 "    \\_/   \\_/    \\_/   \\_/    "

#define COALDEL "                              "

#define LOGO 3
#define LOGOHEIGHT    	 6
#define LOGOFUNNEL  	 4
#define LOGOLENGTH      84
#define LOGOPATTERNS	 6

#define LOGO1  "     ++      +------ "
#define LOGO2  "     ||      |+-+ |  "
#define LOGO3  "   /---------|| | |  "
#define LOGO4  "  + ========  +-+ |  "

#define LWHL11 " _|--O========O~\\-+  "
#define LWHL12 "//// \\_/      \\_/    "

#define LWHL21 " _|--/O========O\\-+  "
#define LWHL22 "//// \\_/      \\_/    "

#define LWHL31 " _|--/~O========O-+  "
#define LWHL32 "//// \\_/      \\_/    "

#define LWHL41 " _|--/~\\------/~\\-+  "
#define LWHL42 "//// \\_O========O    "

#define LWHL51 " _|--/~\\------/~\\-+  "
#define LWHL52 "//// \\O========O/    "

#define LWHL61 " _|--/~\\------/~\\-+  "
#define LWHL62 "//// O========O_/    "

#define LCOAL1 "____                 "
#define LCOAL2 "|   \\@@@@@@@@@@@     "
#define LCOAL3 "|    \\@@@@@@@@@@@@@_ "
#define LCOAL4 "|                  | "
#define LCOAL5 "|__________________| "
#define LCOAL6 "   (O)       (O)     "

#define LCAR1  "____________________ "
#define LCAR2  "|  ___ ___ ___ ___ | "
#define LCAR3  "|  |_| |_| |_| |_| | "
#define LCAR4  "|__________________| "
#define LCAR5  "|__________________| "
#define LCAR6  "   (O)        (O)    "

#define DELLN  "                     "

#define C51 4
#define C51HEIGHT 11
#define C51FUNNEL 7
#define C51LENGTH 87
#define C51PATTERNS 6

#define C51DEL "                                                       "

#define C51STR1 "        ___                                            "
#define C51STR2 "       _|_|_  _     __       __             ___________"
#define C51STR3 "    D__/   \\_(_)___|  |__H__|  |_____I_Ii_()|_________|"
#define C51STR4 "     | `---'   |:: `--'  H  `--'         |  |___ ___|  "
#define C51STR5 "    +|~~~~~~~~++::~~~~~~~H~~+=====+~~~~~~|~~||_| |_||  "
#define C51STR6 "    ||        | ::       H  +=====+      |  |::  ...|  "
#define C51STR7 "|    | _______|_::-----------------[][]-----|       |  "

#define C51WH61 "| /~~ ||   |-----/~~~~\\  /[I_____I][][] --|||_______|__"
#define C51WH62 "------'|oOo|==[]=-     ||      ||      |  ||=======_|__"
#define C51WH63 "/~\\____|___|/~\\_|   O=======O=======O  |__|+-/~\\_|     "
#define C51WH64 "\\_/         \\_/  \\____/  \\____/  \\____/      \\_/       "

#define C51WH51 "| /~~ ||   |-----/~~~~\\  /[I_____I][][] --|||_______|__"
#define C51WH52 "------'|oOo|===[]=-    ||      ||      |  ||=======_|__"
#define C51WH53 "/~\\____|___|/~\\_|    O=======O=======O |__|+-/~\\_|     "
#define C51WH54 "\\_/         \\_/  \\____/  \\____/  \\____/      \\_/       "

#define C51WH41 "| /~~ ||   |-----/~~~~\\  /[I_____I][][] --|||_______|__"
#define C51WH42 "------'|oOo|===[]=- O=======O=======O  |  ||=======_|__"
#define C51WH43 "/~\\____|___|/~\\_|      ||      ||      |__|+-/~\\_|     "
#define C51WH44 "\\_/         \\_/  \\____/  \\____/  \\____/      \\_/       "

#define C51WH31 "| /~~ ||   |-----/~~~~\\  /[I_____I][][] --|||_______|__"
#define C51WH32 "------'|oOo|==[]=- O=======O=======O   |  ||=======_|__"
#define C51WH33 "/~\\____|___|/~\\_|      ||      ||      |__|+-/~\\_|     "
#define C51WH34 "\\_/         \\_/  \\____/  \\____/  \\____/      \\_/       "

#define C51WH21 "| /~~ ||   |-----/~~~~\\  /[I_____I][][] --|||_______|__"
#define C51WH22 "------'|oOo|=[]=- O=======O=======O    |  ||=======_|__"
#define C51WH23 "/~\\____|___|/~\\_|      ||      ||      |__|+-/~\\_|     "
#define C51WH24 "\\_/         \\_/  \\____/  \\____/  \\____/      \\_/       "

#define C51WH11 "| /~~ ||   |-----/~~~~\\  /[I_____I][][] --|||_______|__"
#define C51WH12 "------'|oOo|=[]=-      ||      ||      |  ||=======_|__"
#define C51WH13 "/~\\____|___|/~\\_|  O=======O=======O   |__|+-/~\\_|     "
#define C51WH14 "\\_/         \\_/  \\____/  \\____/  \\____/      \\_/       "
