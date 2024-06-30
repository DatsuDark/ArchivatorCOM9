#include <stdio.h>
#include <string.h>

#include "functions/parametersReader.h"

// countParams() count the number of parameters like "-c" or "--add"
int countParams(int argc, char* argv[]) {
    int paramsCount = 0;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            paramsCount++;
        }
    }
    return paramsCount;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        puts("no parameters");
        return 0;
    }

    // available parameters store
    const char* paramsStore[] = {
            "-c", "--create",
            "-x", "--extract",
            "-l", "--list",
            "-a", "--add",
            "-d", "--delete",
            "-f", "--file",
            "-o", "--output",
            "-v", "--verbose",
            "-s", "--size",
            "-h", "--help",
    };
    const int paramsStoreLength = 20; // store length

    int paramsCount = countParams(argc, argv); // countParams() count the number of parameters like "-c" or "--add"

    // parameterReader() returns array of structures each of which stores key(parameter name) and value(parameter value)
    // array[0].name = "-x"
    // array[0].value = "\path_to.zip"
    Param* enteredParams = parametersReader(paramsCount, argc, argv, paramsStoreLength, paramsStore);

    // uncomment this if you need to see the result of the parameterReader()
    /*for (int i = 0; i < paramsCount; i++) {
        printf("%s: %s\n", enteredParams[i].name, enteredParams[i].value);
    }*/

    // freeing memory
    free(enteredParams);
    return 0;
}
