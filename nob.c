#define NOB_IMPLEMENTATION
#include "nob.h"

#define FLAGS "-Wall", "-Wextra", "-Iinclude"
#define BUILD_FOLDER "build/"
#define SRC_FOLDER "src/"

int build_table_library(Cmd *cmd)
{
    cmd_append(cmd, "cc", FLAGS, "-c", SRC_FOLDER "table.c", "-o", BUILD_FOLDER "table.o");
    if (!nob_cmd_run(cmd))
        return 0;
    cmd_append(cmd, "ar", "rcs", BUILD_FOLDER "libtable.a", BUILD_FOLDER "table.o");
    if (!nob_cmd_run(cmd))
        return 0;
    return 1;
}

int main(int argc, char **argv)
{
    GO_REBUILD_URSELF(argc, argv);

    if (!mkdir_if_not_exists(BUILD_FOLDER))
        return 1;

    Cmd cmd = {0};
    if (!build_table_library(&cmd))
        return 1;

    cmd_append(&cmd, "cc", FLAGS, "-o", BUILD_FOLDER "main", SRC_FOLDER "main.c",
               "-L" BUILD_FOLDER,
               "-ltable");
    if (!nob_cmd_run(&cmd))
        return 1;

    return 0;
}
