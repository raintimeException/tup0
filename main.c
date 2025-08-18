#define TUP_IMPLEMENTATION
#include "tup0.h"

int main(int argc, char* const *argv)
{
    (void)argc;
    (void)argv;
    tup_help(help_Level);
    if (argc == 1) {
        tup_command_dispatcher();
    } else if (argc > 1) {
        // TODO: read different files and give the possibility of selection between them
    } else {
        TUP_UNREACHABLE("main");
    }
    return 0;
}
