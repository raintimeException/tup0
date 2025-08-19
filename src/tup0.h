#include <stdlib.h>
#include <stdio.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

typedef struct {
    unsigned long long idx;
    char *cntnt;
} Item_t;

Item_t **arr_items = NULL;

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
static unsigned long long idx = 1;
static unsigned long long curr_idx = 1;
static const char *default_path = "unnamed_file";

void tup_command_dispatcher(void);
void tup_help(Help_Level help_Level);
void tup_check_desperation_level(void);
void tup_insert(void);
void tup_insert_into_the_line(int itx);
void tup_append(void);
void tup_delete(unsigned long long curr_idx);
void tup_write(const char *path);
void tup_quit(void);
char *tup_read_file(const char *path);
int tup_load_content(const char *file_content, unsigned long long file_size);

#ifdef TUP_IMPLEMENTATION
void tup_command_dispatcher(void)
{
    int i = 0;
    char *command = (char *)calloc(UNIVERSAL_SIZE, 0);
    assert(command != NULL);

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
                    char* path = strtok(&command[++i], " ");
                    path[strcspn(path, "\n")] = '\0';
                    if (strlen(path) == 0) {
                        path = (char *)default_path;
                    }
                    tup_write(path);
                }; break;
                case 'd': {
                    tup_delete(curr_idx);
                }; break;
                case 'q': {
                    tup_quit();
                }; break;
                default: {
                    fprintf(stdout, "?: %s", command);
                };
            };
        } else if ((curr_idx = atoi(command)) > 0) { 
            printf("it is a number: %llu\n", curr_idx);
            tup_insert_into_the_line(curr_idx);
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
    assert(line != NULL);

    Item_t *itm = NULL;

    do {
        line = fgets(line, UNIVERSAL_SIZE, stdin);
        assert(line != NULL);

        itm = (Item_t *)malloc(sizeof(Item_t));
        assert(itm!= NULL);
        itm->idx   = idx++;
        itm->cntnt = strdup(line);

        arrput(arr_items, itm);
    } while (strnstr(line, ".", 1) == NULL);
    tup_command_dispatcher();
}

void tup_insert_into_the_line(int curr_idx)
{
    char *line = (char *)calloc(UNIVERSAL_SIZE, 0);
    assert(line != NULL);

    Item_t *itm = NULL;

    do {
        line = fgets(line, UNIVERSAL_SIZE, stdin);
        assert(line != NULL);

        itm = (Item_t *)malloc(sizeof(Item_t));
        assert(itm!= NULL);

        itm->idx   = curr_idx;
        itm->cntnt = strdup(line);

        arrins(arr_items, curr_idx, itm);
    } while (strnstr(line, ".", 1) == NULL);
    tup_command_dispatcher();
}


void tup_append(void)
{
    // TODO: implement this.
    tup_insert();
}

void tup_delete(unsigned long long curr_idx)
{
    arrdel(arr_items, curr_idx);
    printf("line: [%llu] has been deleted\n", curr_idx);
}

void tup_write(const char *path)
{
    FILE *f_strem = fopen(path, "w");
    assert(f_strem != NULL);

    for (int i = 0; i < arrlen(arr_items); ++i) {
        char *line = arr_items[i]->cntnt;
        printf("%s ", line);
        assert(fwrite(line, sizeof(char), strlen(line), f_strem) != 0);
    }
    arrfree(arr_items);
    fprintf(stdout, "Buffer was written into: %s\n", path);
}

void tup_quit(void)
{
    int hm_len = arrlen(arr_items);
    if (hm_len > 0) {
        printf("are you sure?(y/n), the buffer is not empty: %d\n", hm_len);
        char response = fgetc(stdin);
        if (response == 'y' || response == 'Y') {
            fprintf(stdout, "ok.\n");
            exit(1);
        } else {
            tup_command_dispatcher();
        }
    } else {
        fprintf(stdout, "lines in the buffer: %d\n", hm_len);
    }
    arrfree(arr_items);
    exit(0);
}

char *tup_read_file(const char *path)
{
    FILE *file = fopen(path, "ab+");
    assert(file != NULL);

    assert(fseek(file, 0, SEEK_END) != -1);
    unsigned long long file_size = ftell(file) + 1;
    assert(fseek(file, 0, SEEK_SET) != -1);

    char *file_content = (char *)calloc(file_size, sizeof(char));
    assert(file_content != NULL);

    fread(file_content, sizeof(char), file_size, file);

    assert(tup_load_content(file_content, file_size) == 1);

    fprintf(stdout, "Content from file %s has been loaded\n", path);
    return file_content;
}

int tup_load_content(const char *file_content, unsigned long long file_size)
{
    (void)file_size;
    char *line = (char *)calloc(UNIVERSAL_SIZE, 0);
    assert(line != NULL);

    unsigned long long i, j;
    for (i = 0, j = 0; i < file_size; ++i, ++j) {
        line[j] = file_content[i];
        if (line[j] == 10) {
            line[++j] = '\0';

            Item_t *itm = (Item_t *)malloc(sizeof(Item_t));
            assert(itm!= NULL);

            itm->idx = idx++;
            itm->cntnt = line;

            arrput(arr_items, itm);

            j = -1;
        }
    }

    return 1;
}

#endif // TUP_IMPLEMENTATION
