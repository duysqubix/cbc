#include <sys/stat.h>
#include "utils.h"

int file_exists(const char *filename){
    struct stat sb;

    if(!stat(filename, &sb)){
        if(S_ISREG(sb.st_mode)){
            // it's a file 
            return 1;
        }else if(S_ISDIR(sb.st_mode)){
            // it's a directory
            return 0;
        }
    }
    return 0;
}
