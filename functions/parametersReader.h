#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// equalizeParams() equalizes parameters ("--create" becomes "-c")
char* equalizeParams(char* param, const int paramsStoreLength, const char* paramsStore[]) {
    for (int i = 1; i < paramsStoreLength; i += 2) {
        if (strcmp(param, paramsStore[i]) == 0) {
            param = paramsStore[i - 1];
            break;
        }
    }
    return param;
}

// checkRequiredParams() checks for existence of required parameter "-f" when finding "-a"
int checkRequiredParams(int CLArgsCount, char* CLArgs[]) {
    for (int i = 1; i < CLArgsCount; i++) {
        if (strcmp(CLArgs[i], "-a") == 0) {
            for (int j = 0; j < CLArgsCount; j++) {
                if (strcmp(CLArgs[j], "-f") == 0) {
                    return 1;
                }
            }
            if (i == CLArgsCount - 1) {
                return 0;
            }
        }
    }
    return 1;
}

int isOneUniqueParam(int paramsCount, int CLArgsCount, char* CLArgs[]) {
    char* uniqueParams[] = {"-h","-f","-o","-v","-s"};
    int uniqueParamsLength = 5;

    char usedParams[paramsCount][3];
    int usedParamsIndex = 0;

    for (int i = 1; i < CLArgsCount; i++) {
        if (CLArgs[i][0] == '-') {
            for (int j = 0; j < uniqueParamsLength; j++) {
                if (strcmp(CLArgs[i], uniqueParams[j]) == 0) {
                    strcpy(usedParams[usedParamsIndex], CLArgs[i]);
                    usedParamsIndex++;
                    if (usedParamsIndex == paramsCount) {
                        i = CLArgsCount;
                        break;
                    }
                }
            }
        }
    }

    for (int i = 0; i < paramsCount - 1; i++) {
        for (int j = i + 1; j < paramsCount; j++) {
            if (strcmp(usedParams[i], usedParams[j]) == 0 && usedParams[i][0] == '-' && usedParams[j][0] == '-') {
                return 0;
            }
        }
    }

    return 1;
}

// isCorrectParams() compares parametersStore to user-entered parameters
int isCorrectParams(int paramsCount, int CLArgsCount, char* CLArgs[], const int paramsStoreLength, const char* paramsStore[]) {
    for (int i = 1; i < CLArgsCount; i++) {
        if (CLArgs[i][0] == '-') {
            for (int j = 0; j < paramsStoreLength; j += 2) {
                if (strcmp(CLArgs[i], paramsStore[j]) == 0) {
                    break;
                }
                if (j == 18) {
                    return 0;
                }
            }
        }
    }

    if (!checkRequiredParams(CLArgsCount, CLArgs) || !isOneUniqueParam(paramsCount, CLArgsCount, CLArgs)) {
        return 0;
    }

    return 1;
}

// parameter structure
typedef struct {
    char* name;
    char* value;
} Param;

// enterParam() writes the parameter name and value
Param enterParam(char* name, char* value) {
    Param param;
    param.name = name;
    param.value = value;
    return param;
}

// main function (CL - Command Line)
Param* parametersReader(int paramsCount, int CLArgsCount, char* CLArgs[], const int paramsStoreLength, const char* paramsStore[]) {
    // equalizeParams() equalizes parameters ("--create" becomes "-c")
    for (int i = 1; i < CLArgsCount; i++) {
        if (strncmp(CLArgs[i], "--", 2) == 0) {
            CLArgs[i] = equalizeParams(CLArgs[i], paramsStoreLength, paramsStore);
        }
    }

    // isCorrectParams() compares parametersStore to user-entered parameters
    if(!isCorrectParams(paramsCount, CLArgsCount, CLArgs, paramsStoreLength, paramsStore)) {
        puts("incorrect parameters");
        return 0;
    }

    // memory allocation for array of structures
    Param* enteredParams = (Param*)malloc(paramsCount * sizeof(Param));
    if (enteredParams == NULL) {
        return 0;
    }

    // cycle to fill enteredParams array
    for (int i = 0, j = 1; i < paramsCount; i++, j += 2) {
        // this "if" is needed to assign value to parameters that initially do not have one (like -h and -v)
        if (strcmp(CLArgs[j], "-h") == 0 || strcmp(CLArgs[j], "-v") == 0) {
            // enterParam() writes the parameter name and value
            enteredParams[i] = enterParam(CLArgs[j], "true");
            j--;
        } else {
            enteredParams[i] = enterParam(CLArgs[j], CLArgs[j + 1]);
        }
    }

    return enteredParams;
}


