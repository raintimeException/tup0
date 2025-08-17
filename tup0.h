#define NOB_DA_INIT_CAP 256
#define NOB_IMPLEMENTATION

#include "nob.h"


#define FULL_HELP 1

Nob_String_Builder lines = {0};
//nob_sb_appendf(&lines, "%s", "one");

void command_dispatcher(void);
void usage_message(void);

void command_dispatcher(void)
{
    char *command = (char *)calloc(NOB_DA_INIT_CAP, sizeof(char));
    if (!command) {
        NOB_ASSERT(command != NULL && "Buy more RAM lol!!!");
    }
    while (1) {
        usage_message();

        if (!fgets(command, NOB_DA_INIT_CAP, stdin)) {
            nob_log(NOB_ERROR, "Could not read the command: %s", command);
        }
        if (strnstr(command, ":", 1)) {
            char cmd = command[1]; // TODO: change this for 'ex' and commands len > 1
            switch (cmd) {
                case 'h': {
                    NOB_TODO("help");
                }; break;
                case 'i': {
                    NOB_TODO("insert");
                }; break;
                case 'a': {
                    NOB_TODO("append");
                }; break;
                case 'w': {
                    NOB_TODO("write");
                }; break;
                case 'd': {
                    NOB_TODO("delete");
                }; break;
                case 'q': {
                    NOB_TODO("quit");
                }; break;
                default: {
                    printf("?: %s", command);
                };
            };
        }
        memset(command, 0, NOB_DA_INIT_CAP);
    }
}

void usage_message(void)
{
    printf("*");
#if FULL_HELP
    printf(":[command] [filename]                                           \n\
    :h                     - help.                                          \n\
    :i                     - insert.                                        \n\
    :a                     - append.                                        \n\
    :w [filename]          - write.                                         \n\
    :d[linenumber]         - delete line.                                   \n\
    :ex                    - explore current directory (?)                  \n\
    :q                     - quit.                                          \n\
............................................................................\n\
    NOTE: options marked with '(?)' are not implemented yet                 \n\
    \n");
#else
    printf(":[command]\n");
#endif
    printf("*");
}
