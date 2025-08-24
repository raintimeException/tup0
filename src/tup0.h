#include <stdlib.h>
#include <stdio.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define PROMPT_SIGN     "*"
#define UNIVERSAL_SIZE  8192
#define DEFAULT_PATH    "default_path"
#define PEDANTIC
#undef  PEDANTIC

#define TUP_TODO(message) do { fprintf(stderr, "%s:%d: TODO: %s\n", __FILE__, __LINE__, message); abort(); } while(0)
#define TUP_UNREACHABLE(message) do { fprintf(stderr, "%s:%d: UNREACHABLE: %s\n", __FILE__, __LINE__, message); abort(); } while(0)

typedef struct {
    char *data;
    int start;
    int end;
    int idx;
} Line_t;

typedef struct {
    const char *f_in;
    const char *f_out;

    Line_t **lines;

    int curs_idx;
    char command[UNIVERSAL_SIZE];
} Editor_t;

typedef enum {
    MIN,
    MAX,
    EXTREME,
} Help_Level;
Help_Level help_Level = MIN;
static int desperation_level = 0;

static const char *exit_message = "GOOD LUCK MATE";

void tup_command_dispatcher(Editor_t *editor);
void tup_help(void);
void tup_check_desperation_level(void);
void tup_insert(Editor_t *editor);
void tup_insert_into_curs_line(Editor_t *editor);
void tup_append_line(Editor_t *editor);
char *_tup_remove_new_line_char(char *data);
void tup_delete_line(Editor_t *editor);
int _tup_editor_set_f_out(Editor_t *editor, char *path);
void tup_write_out(Editor_t *editor);
int _tup_editor_set_f_in(Editor_t *editor, char *path);
int _tup_editor_load_content(Editor_t *editor, const char *content, int content_size);
void tup_read_in(Editor_t *editor);
void tup_quit(Editor_t *editor);


#ifdef TUP_IMPLEMENTATION

void tup_command_dispatcher(Editor_t *editor)
{
    while (1) {
        if (!fgets(editor->command, UNIVERSAL_SIZE, stdin)) {
            fprintf(stderr, "Could not read the command: %s", editor->command);
        }
        if (strnstr(editor->command, ":", 1)) {
            char cmd = (editor->command)[1];
            switch (cmd) {
                case 'h': {
                    tup_help();
                }; break;
                case 'i': {
                    tup_insert(editor);
                }; break;
                case 'a': {
                    tup_append_line(editor);
                }; break;
                case 'w': {
                    char *command = &(editor->command)[3];
                    char *path = strtok(command, " ");
                    _tup_editor_set_f_out(editor, path);
                    tup_write_out(editor);
                }; break;
                case 'd': {
                    tup_delete_line(editor);
                }; break;
                case 'q': {
                    tup_quit(editor);
                }; break;
                default: {
                    fprintf(stdout, "?: %s", editor->command);
                };
            };
        } else if ((editor->curs_idx = atoi(editor->command)) > 0) { 
            fprintf(stdout, "curs_idx %d, content: %s\n", editor->curs_idx, editor->lines[editor->curs_idx - 1]->data);
        } else {
            tup_help();
        }
        memset(editor->command, 0, UNIVERSAL_SIZE);
    }
}

void tup_help(void)
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
    fprintf(stdout, PROMPT_SIGN);
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

void tup_insert(Editor_t *editor)
{
    char *data = NULL;
    Line_t *line = NULL;

    while (1) {
        data = (char *)calloc(UNIVERSAL_SIZE, 0);
        assert(data != NULL);
        fprintf(stdout, "INFO: insert '.' to stop insertion\n");

        data = fgets(data, UNIVERSAL_SIZE, stdin);

        line = (Line_t *)malloc(sizeof(Line_t));
        assert(line != NULL);

        line->data = strdup(data);
        line->start = 0;
        line->end = strlen(data);

        if (strnstr(line->data, ".", 1) != NULL) {
            free(line);
            goto exit;
        }
        arrput(editor->lines, line);
        for (int i = 0; i < arrlen(editor->lines); ++i) {
            printf("%s", (editor->lines)[i]->data);
        }
        editor->curs_idx = arrlen(editor->lines);
        line = NULL;
        data = NULL;
    }
exit:
    tup_command_dispatcher(editor);
}

void tup_insert_into_curs_line(Editor_t *editor)
{
    char *data = NULL;
    Line_t *line = NULL;

    while (1) {
        data = (char *)calloc(UNIVERSAL_SIZE, 0);
        data = fgets(data, UNIVERSAL_SIZE, stdin);
        assert(data != NULL);

        line = (Line_t *)malloc(sizeof(Line_t));
        assert(line != NULL);

        line->data = strdup(data);
        line->start = 0;
        line->end = strlen(data);

        if (strnstr(line->data, ".", 1) != NULL) {
            free(line);
            goto exit;
        }
        arrins(editor->lines, editor->curs_idx, line);
    }
exit:
    tup_command_dispatcher(editor);
}

void tup_append_line(Editor_t *editor)
{
    Line_t *line = editor->lines[editor->curs_idx - 1];
    char *data = line->data;

    char *data_to_append = (char *)calloc(UNIVERSAL_SIZE*sizeof(char), 0);
    assert(data_to_append != NULL);
    int n = UNIVERSAL_SIZE - strlen(data);

    data = _tup_remove_new_line_char(data);
    fprintf(stdout, "%s", data);

    while (1) {
        data_to_append = fgets(data_to_append, UNIVERSAL_SIZE, stdin);
        assert(data_to_append != NULL);
        if (strnstr(data_to_append, ".", 1)) goto exit;


        line->data = strncat(data, data_to_append, n);
        line->start = 0;
        line->end = strlen(data);

        for (int i = 0; i < arrlen(editor->lines); ++i) {
            printf("%s", (editor->lines)[i]->data);
        }
    }
exit:
    tup_insert(editor);
}

char *_tup_remove_new_line_char(char *data)
{
    for (unsigned long i = 0; i < strlen(data); ++i) {
        if (data[i] == '\n') data[i] = 0;
    }
    return data;
}

void tup_delete_line(Editor_t *editor)
{
    arrdel(editor->lines, editor->curs_idx - 1);
    fprintf(stdout, "[INFO]: line [%d] has been deleted\n", editor->curs_idx);
}

int _tup_editor_set_f_out(Editor_t *editor, char *path)
{
    path[strcspn(path, "\n")] = '\0';
    if (strlen(path) == 0) {
        path = DEFAULT_PATH;
    }

    editor->f_out = path;
    return 1;
}

void tup_write_out(Editor_t *editor)
{
    FILE *f_strem = fopen(editor->f_out, "w");
    assert(f_strem != NULL);

    char *data = (char *)calloc(UNIVERSAL_SIZE, 0);
    assert(data != NULL);

    for (int i = 0; i < arrlen(editor->lines); ++i) {
        data = editor->lines[i]->data;
        fprintf(stdout, "%s", data);
        assert(fwrite(data, sizeof(char), strlen(data), f_strem) != 0);
    }
    free(data);
    arrfree(editor->lines);
    fprintf(stdout, "[INFO]: Buffer was written into: %s\n", editor->f_out);
}

void tup_quit(Editor_t *editor)
{
#ifdef PEDANTIC
    int remain_inside = arrlen(editor->lines);
    if (remain_inside > 0) {
        fprintf("[WARN]: buffer is not empty: %d\n", hm_len);
        fprintf("[WARN]: quit(y/n):\n");
        char response = fgetc(stdin);
        if (response == 'y' || response == 'Y') {
            fprintf(stdout, "[INFO]: ok.\n");
            exit(1);
        } else {
            tup_command_dispatcher(editor);
        }
    } else {
        fprintf(stdout, "[INFO]: lines in the buffer: %d\n", hm_len);
    }
#else
    arrfree(editor->lines);
    exit(0);
#endif
    fprintf(stdout, "[INFO]: %s\n", exit_message);
}

int _tup_editor_set_f_in(Editor_t *editor, char *path)
{
    editor->f_in = path;
    printf("%s\n", editor->f_in);
    printf("%s\n", path);
    return 1;
}

void tup_read_in(Editor_t *editor)
{
    FILE *file = fopen(editor->f_in, "ab+");
    assert(file != NULL);

    assert(fseek(file, 0, SEEK_END) != -1);
    int content_size = ftell(file) + 1;
    assert(fseek(file, 0, SEEK_SET) != -1);

    char *content = (char *)calloc(content_size, sizeof(char));
    assert(content != NULL);

    fread(content, sizeof(char), content_size, file);

    if (!_tup_editor_load_content(editor, content, content_size)) {
        fprintf(stderr, "[ERROR]: Could not load content from file [%s] \n", editor->f_in);
        exit(1);
    }
    fprintf(stdout, "[INFO]: Content from file [%s] has been loaded\n", editor->f_in);
}

int _tup_editor_load_content(Editor_t *editor, const char *content, int content_size)
{
    char *data = (char *)calloc(UNIVERSAL_SIZE, 0);
    assert(data != NULL);

    Line_t *line = NULL;
    int i, j;
    for (i = 0, j = 0; i < content_size; ++i, ++j) {
        data[j] = content[i];
        if (data[j] == 10) {
            data[++j] = '\0';

            line = (Line_t *)malloc(sizeof(Line_t));
            assert(line != NULL);

            line->data = data;
            line->start = 0;
            line->end = strlen(data);

            arrput(editor->lines, line);
            j = -1;
        }
    }
    return 1;
}

#endif // TUP_IMPLEMENTATION
