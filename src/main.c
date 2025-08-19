#define TUP_IMPLEMENTATION
#include "tup0.h"

int main(int argc, char* const *argv)
{
    tup_help(help_Level);
    if (argc == 1) {
        tup_command_dispatcher();
    } else if (argc == 2) {
        tup_read_file(*++argv);
    } else {
        TUP_UNREACHABLE("main");
    }

    return 0;
}
