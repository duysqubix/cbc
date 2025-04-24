#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include "defs.h"
#include "log.h"

static struct option long_options[] ={
    {"debug", required_argument, 0, 'd'},
    {"step", no_argument, 0, 's'},
    {0, 0, 0, 0}
};


// default options
bool DEBUG_STEP_MODE = false;


static void parse_flags(int *argc, char ***argv){
    int c;
    int option_index = 0;
    while((c = getopt_long(*argc, *argv, "d:", long_options, &option_index)) != -1){
        switch (c){
            case 'd':

                if(strcmp(optarg, "trace") == 0){ log_set_level(LOG_TRACE); }
                if(strcmp(optarg, "debug") == 0){ log_set_level(LOG_DEBUG); }
                if(strcmp(optarg, "info") == 0) { log_set_level(LOG_INFO); }
                if(strcmp(optarg, "error") == 0){ log_set_level(LOG_ERROR); }
                if(strcmp(optarg, "fatal") == 0){ log_set_level(LOG_FATAL); }

                break;
            case 's':
                DEBUG_STEP_MODE = true;
                break;
        }
    }
    *argc -= optind;
    *argv += optind;
}

int main(int argc, char *argv[]) {
    log_set_level(LOG_WARN);

    parse_flags(&argc, &argv);

    log_info("Step: %s", DEBUG_STEP_MODE ? "true" : "false");
    
    gameboy_init(argv[0]);

    return gameboy_loop();
    // int r = gameboy_loop();
    return 0;
} 