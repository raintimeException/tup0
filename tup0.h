#include <stdlib.h>
#include <stdio.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

typedef struct {
    char *key;
    int value;
} Hsh_t;

Hsh_t *hsh = NULL;

#define TUP_UNREACHABLE(message) do { fprintf(stderr, "%s:%d: UNREACHABLE: %s\n", __FILE__, __LINE__, message); abort(); } while(0)
#define TUP_TODO(message) do { fprintf(stderr, "%s:%d: TODO: %s\n", __FILE__, __LINE__, message); abort(); } while(0)

#define PROMPT_SIGN    "*"
#define UNIVERSAL_SIZE  512

typedef enum {
    MIN,
    MAX,
    EXTREME,
} Help_Level;

Help_Level help_Level = MIN;
static int desperation_level = 0;
static unsigned long long value = 1;

void tup_command_dispatcher(void);
void tup_help(Help_Level help_Level);
void tup_check_desperation_level(void);
void tup_insert(void);
void tup_append(void);
void tup_delete(void);
void tup_write(const char *path);
void tup_quit(void);

#ifdef TUP_IMPLEMENTATION
void tup_command_dispatcher(void)
{
    int i = 0;
    char *command = (char *)calloc(UNIVERSAL_SIZE, 0);
    if (!command) assert(command != NULL);

    while (1) {
        if (!fgets(command, UNIVERSAL_SIZE, stdin)) {
            fprintf(stderr, "Could not read the command: %s", command);
        }
        ++i;
        assert(i == 1);
        if (strnstr(command, ":", i)) {
            char cmd = command[i];
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
                    char * path = strtok(&command[++i], " ");
                    path[strcspn(path, "\n")] = '\0';
                    tup_write(path);
                }; break;
                case 'd': {
                    tup_delete();
                }; break;
                case 'q': {
                    tup_quit();
                }; break;
                default: {
                    fprintf(stdout, "?: %s", command);
                };
            };
        }
        memset(command, 0, UNIVERSAL_SIZE);
        i = 0;
    }
}

void tup_help(Help_Level help_Level)
{
    tup_check_desperation_level();
    switch (help_Level) {
    case MIN: {
        fprintf(stdout, ":[command]\n");
    } break;
    case MAX: {
    fprintf(stdout,
    PROMPT_SIGN":[command] [filename]                                       \n\
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
        fprintf(stdout, PROMPT_SIGN": are you stupid?\n");
    } break;
    default:
        TUP_UNREACHABLE("tup_help");
    };
    fprintf(stdout, "*");
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
    char *line = (char *)calloc(UNIVERSAL_SIZE, 0);
    if (!line) assert(line != NULL);

    do {
        line = fgets(line, UNIVERSAL_SIZE, stdin);
        if (!line) assert(line != NULL);
        shput(hsh, strdup(line), value++);
    } while (strnstr(line, ".", 1) == NULL);
    tup_command_dispatcher();
}

void tup_append(void)
{
    // TODO: implement this.
    tup_insert();
}

void tup_delete(void)
{
    TUP_TODO("tup_delete");
}

void tup_write(const char *path)
{
    FILE *f_strem = fopen(path, "w");
    assert(f_strem != NULL);

    for (int i = 0; i < shlen(hsh); ++i) {
        char *line = hsh[i].key;
        printf("%s ", line);
        assert(fwrite(line, sizeof(char), strlen(line), f_strem) != 0);
    }
    shfree(hsh);
    fprintf(stdout, "Buffer was written into: %s", path);
}

void tup_quit(void)
{
    int hm_len = shlen(hsh);
    if (hm_len > 0) {
        printf("are you sure?(y/n), the buffer is not empty: %d\n", hm_len);
        char response = fgetc(stdin);
        printf("fgets: %c\n", response);
        if (response == 'y' || response == 'Y') {
            printf("ok.\n");
            exit(1);
        } else {
            tup_command_dispatcher();
        }
    } else {
        printf("lines in the buffer: %d\n", hm_len);
    }
    shfree(hsh);
    exit(0);
}


#endif // TUP_IMPLEMENTATION
