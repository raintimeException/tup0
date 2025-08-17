#define TUP_IMPLEMENTATION
#include "tup0.h"

int main(int argc, char* const *argv)
{
    if (argc == 1) {
        tup_command_dispatcher();
    } else if (argc == 2) {
        if (!nob_read_entire_file(argv[1], &lines)) {
            nob_log(NOB_ERROR, "Could not read the file: %s", argv[1]);
        }
    } else {
        NOB_TODO("Implement selection of different files");
    }
    return 0;
}
