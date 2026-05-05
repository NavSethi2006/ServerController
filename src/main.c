#include <stdio.h>
#include "utils/logger.h"
#include "utils/file_handle.h"
#include "utils/configs.h"
#include "server.h"

int main() {

    create_log();  

    LOG("Creating Server", INFO);
    LOG("TESTING INFO", INFO);
    LOG("TESTING FAILED", FAILED);
    LOG("TESTING SUCCESS", SUCCESS);

    FILE* file = setup_config();
    
    load_all();

    setup_server();

}

