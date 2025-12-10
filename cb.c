#include "cb.h"

void build() {
    command_t * cmd = command_init(CC);
    command_add_source_file(cmd, "src/main.c");
    command_add_source_file(cmd, "src/player.c");
    command_add_source_file(cmd, "src/anim.c");
    command_add_include_dir(cmd, "include");
    command_add_dynamic_library(cmd, "SDL3");
    command_add_dynamic_library(cmd, "m");
    command_set_output_file(cmd, "build/main");
    command_execute(cmd);
    command_deinit(cmd);
}

void run() {
    command_t * cmd = command_init("/home/onraj/Documents/Projects/2D-Game-SDL3/build/main");
    command_execute(cmd);
    command_deinit(cmd);
}

int main(int argc, char ** argv) {
    cb_rebuild_on_change(__FILE__, argv);
    parse_arguments(argc, argv);
    build();
    char * r = argv[1];
    if(strcmp(r, "run") == 0) {
        run();
    }
    return 0;
}
