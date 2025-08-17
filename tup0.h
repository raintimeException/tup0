#define NOB_DA_INIT_CAP 256
#define NOB_IMPLEMENTATION

#include "nob.h"

//
// DEPs stuff
//
Nob_String_Builder lines = {0};
//nob_sb_appendf(&lines, "%s", "one");

//
// APPs stuff
//

#define PROMPT_SIGN    "*"

typedef enum {
    MIN,
    MAX,
    EXTREME,
} Help_Level;

Help_Level help_Level = MIN;
static int desperation_level = 0;

void tup_command_dispatcher(void);
void tup_help(Help_Level help_Level);
void tup_check_desperation_level(void);
void tup_insert(void);
void tup_append(void);
void tup_delete(void);
void tup_write(void);
void tup_quit(void);


#ifdef TUP_IMPLEMENTATION
void tup_command_dispatcher(void)
{
    char *command = (char *)calloc(NOB_DA_INIT_CAP, sizeof(char));
    if (!command) NOB_ASSERT(command == NULL && "Buy more RAM lol!!!");

    while (1) {
        tup_help(help_Level);

        if (!fgets(command, NOB_DA_INIT_CAP, stdin)) {
            nob_log(NOB_ERROR, "Could not read the command: %s", command);
        }
        if (strnstr(command, ":", 1)) {
            char cmd = command[1]; // TODO: change this for 'ex' and commands len > 1
            switch (cmd) {
                case 'h': {
                    tup_help(help_Level);
                }; break;
                case 'i': {
                    tup_insert();
                }; break;
                case 'a': {
                    tup_append();
                }; break;
                case 'w': {
                    tup_write();
                }; break;
                case 'd': {
                    tup_delete();
                }; break;
                case 'q': {
                    tup_quit();
                }; break;
                default: {
                    printf("?: %s", command);
                };
            };
        }
        memset(command, 0, NOB_DA_INIT_CAP);
    }
}

void tup_help(Help_Level help_Level)
{
    tup_check_desperation_level();
    switch (help_Level) {
    case MIN: {
        printf(":[command]\n");
    } break;
    case MAX: {
    printf(PROMPT_SIGN":[command] [filename]                                \n\
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
    } break;
    case EXTREME: {
        printf(PROMPT_SIGN": are you stupid?\n");
    } break;
    default:
        NOB_UNREACHABLE("tup_help");
    };
    printf("*");
}

void tup_check_desperation_level(void)
{
    desperation_level++;
    if (desperation_level > 0 && desperation_level < 3) {
        help_Level = MIN;
    } else if (desperation_level >= 3 && desperation_level <= 10) {
        help_Level = MAX;
    } else {
        help_Level = EXTREME;
    }
}

void tup_insert(void)
{
    char *line = (char *)calloc(NOB_DA_INIT_CAP, sizeof(char));
    if (!line) NOB_ASSERT(line == NULL && "Buy more RAM lol!!!");

    do {
        line = fgets(line, NOB_DA_INIT_CAP, stdin);
        if (!line) {
            NOB_ASSERT(line == NULL && "Could not read the line, sorry :(");
        }
        nob_sb_appendf(&lines, "%s", line);
    } while (strnstr(line, ".", 1) == NULL);
    tup_command_dispatcher();
}
void tup_append(void)
{
    NOB_TODO("tup_insert");
}
void tup_delete(void)
{
    NOB_TODO("tup_insert");
}
void tup_write(void)
{
    NOB_TODO("tup_insert");
}
void tup_quit(void)
{
    NOB_TODO("tup_insert");
}

#endif // TUP_IMPLEMENTATION
