#define TUP_IMPLEMENTATION
#include "tup0.h"

int main(int argc, char* const *argv)
{
    Editor_t editor = {0};

    editor.help_level = MIN;
    tup_help(&editor);

    if (argc == 1) {
        tup_command_dispatcher(&editor);
    } else if (argc == 2) {
        if (!_tup_editor_set_f_in(&editor, *++argv)) {
            fprintf(stderr, "[ERROR]: Could not read the file: %s\n", *argv);
            exit(1);
        }
        tup_read_in(&editor);
    } else {
        fprintf(stderr, "[ERROR]: something happend in main()\n");
        exit(1);
    }

    return 0;
}
