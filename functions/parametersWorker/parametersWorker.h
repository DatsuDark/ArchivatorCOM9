// Вопрос: проверки ошибок должен создать я и тогда в визуализаторе будет большое количество функций под каждую ошибку
// Или я должен просто отправлять определённый код ошибки и уже в визуализаторе будет происходить проверка, какой код пришёл и тогда там просто будет одна большая функция
// поменять типы char на int
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char* name;
    char* value;
} Param;

int checkingTheFileCreate (char* value, int codeError, int resultOfTheCheck){
    char *extension = strrchr(value, '.');
    if (extension != NULL) {
        if (strcmp(extension, ".zip") == 0) {
            if (fopen(value, "rb") == NULL){
                resultOfTheCheck = 1;
                return resultOfTheCheck;
            } else {
                codeError = 0;//"Какой-то номер ошибки (такой архив уже существует)";
                return codeError;
            }
        } else if (strcmp(extension, ".tar") == 0) {
            if (fopen(value, "rb") == NULL){
                resultOfTheCheck = 2;
                return resultOfTheCheck;
            }else {
                codeError = 0;//"Какой-то номер ошибки (такой архив уже существует)";
                return codeError;
            }
        } else {
            codeError = 0;//"Какой-то номер ошибки (архивы с таким расширением не поддерживаются)";
            return codeError;
        }
    } else {
        codeError = 0; //"Какой-то номер ошибки (неверно указано имя архива)";
        return codeError;
    }
}

int  checkingTheFileExtract (char* value, int codeError, int resultOfTheCheck){
    char *extension = strrchr(value, '.');
    if (extension != NULL) {
        if (strcmp(extension, ".zip") == 0) {
            if (fopen(value, "rb") == NULL){
                codeError = 0;//"Какой-то номер ошибки (такого архива не существует)";
                return codeError;
            } else {
                resultOfTheCheck = 1;
                return resultOfTheCheck;
            }
        } else if (strcmp(extension, ".tar") == 0) {
            if (fopen(value, "rb") == NULL){
                codeError = 0;//"Какой-то номер ошибки (такого архива не существует)";
                return codeError;
            }else {
                resultOfTheCheck = 2;
                return resultOfTheCheck;
            }
        } else {
            codeError = 0;//"Какой-то номер ошибки (архивы с таким расширением не поддерживаются)";
            return codeError;
        }
    } else {
        codeError = 0;//"Какой-то номер ошибки (неверно указано имя архива)";
        return codeError;
    }
}

int pathExists(char* value) {
    FILE *file = fopen(value, "rb");
    if (file) {
        fclose(file);
        return 1;
    } else {
        return 0;
    }
}

void parametersWorker(Param* dataParams, int count) {

    int codeError;
    int resultOfTheCheck;
    char* pathToSave[100];

    for (int i = 0; i < count; ++i) {
        if (strcmp(dataParams[i].name, "-c") == 0) {
            for (int j = i; j < count; ++j){
                if (strcmp(dataParams[j].name, "-o") == 0) {
                    dataParams[i].value = strcat(dataParams[j].value, dataParams[i].value);
                    if (pathExists(dataParams[i].value)){
                        checkingTheFileCreate(dataParams[i].value, codeError, resultOfTheCheck);
                        if (resultOfTheCheck == 1) {
                            //fileManager(отдаём какие-то данные, нужные для создания архива .zip);
                        } else if(resultOfTheCheck == 2){
                            //fileManager(отдаём какие-то данные, нужные для создания архива .tar);
                        } else {
                            // вызываем функцию из визуализатора, для выведения информации об ошибке
                        }
                    } else {
                        //Тут должен быть блок которые требует, чтобы файл менеджер создал не только архив, но и папки, которых не хватает для корректного пути
                    }
                } else {
                    checkingTheFileCreate(dataParams[i].value, codeError, resultOfTheCheck);
                    if (resultOfTheCheck == 1) {
                        //fileManager(отдаём какие-то данные, нужные для создания архива .zip);
                    } else if(resultOfTheCheck == 2) {
                        //fileManager(отдаём какие-то данные, нужные для создания архива .tar);
                    } else {
                        // вызываем функцию из визуализатора, для выведения информации об ошибке
                    }
                }
            }
        }else if (strcmp(dataParams[i].name, "-x") == 0) {
            for (int j = i; j < count; ++j){
                if (strcmp(dataParams[j].name, "-f") == 0) {
                    dataParams[i].value = dataParams[j].value;
                    if (pathExists(dataParams[i].value)) {
                        checkingTheFileExtract(dataParams[j].value, codeError, resultOfTheCheck);
                        if (resultOfTheCheck == 1) {
                            for (int k = i; k < count; ++k) {
                                if (strcmp(dataParams[k].name, "-o") == 0) {
                                    pathToSave[100] = dataParams[k].value;
                                    //вызываем какую то функцию для отправки пути сохранения извлечённых данных из архива
                                }
                            }
                            //fileManager(отдаём какие-то данные, нужные для извлечения архива .zip);
                        } else if(resultOfTheCheck == 2) {
                            for (int k = i; k < count; ++k) {
                                if (strcmp(dataParams[k].name, "-o") == 0) {
                                    pathToSave[100] = dataParams[k].value;
                                    //вызываем какую то функцию для отправки пути сохранения извлечённых данных из архива
                                }
                            }
                            //fileManager(отдаём какие-то данные, нужные для извлечения архива .tar);
                        } else {
                            // вызываем функцию из визуализатора, для выведения информации об ошибке
                        }
                    } else {
                        codeError = 0; //"Какой то номер ошибки, указанного пути не существует";
                        // тут должен быть вызов функции из визуализатора
                    }
                } else {
                    codeError = 0;//"Тут должен быть какой то номер ошибки, нам не пришёл путь, откуда нужно извлекать архив";
                    // тут должен быть вызов функции из визуализатора
                }
            }
        }else if (strcmp(dataParams[i].name, "-l") == 0) {
            for (int j = i; j < count; ++j){
                if(strcmp(dataParams[j].name, "-f") == 0){
                    dataParams[i].value = dataParams[j].value;
                    //отправляю данные в файл менеджер, чтобы он вытащил названеия файлов из архива и передал их визуализатору
                } else {
                    codeError = 0;//"Тут должен быть какой то номер ошибки, нам не пришёл путь, откуда нужно извлекать архив";
                    // тут должен быть вызов функции из визуализатора
                }
            }
        }else if (strcmp(dataParams[i].name, "-a") == 0) {
            for (int j = i; j < count; ++j){
                if (strcmp(dataParams[j].value, "-f")){
                    dataParams[i].value = dataParams[j].value;
                    
                } else {
                    codeError = 0;//"Тут должен быть какой то номер ошибки, нам не пришёл путь, откуда нужно извлекать архив";
                    // тут должен быть вызов функции из визуализатора
                }
            }
        }else if (strcmp(dataParams[i].name, "-d") == 0) {

        }else if (strcmp(dataParams[i].name, "-f") == 0) {
            
        }else if (strcmp(dataParams[i].name, "-o") == 0) {

        }else if (strcmp(dataParams[i].name, "-v") == 0) {
            //слабо представляю как это вообще должно работать, предпологаю, что я должен отправить данную информацию или в кодер или в визуализатор
        }else if (strcmp(dataParams[i].name, "-s") == 0) {
            // надо уточнить, приходят ли мне файлы в каком порядке, отсартированные или нет, просто если да, то по идее я должен выдавать ошибку, так как не может придти только команда -s
        }else if (strcmp(dataParams[i].name, "-h") == 0) {
            //helper(); //функция для вывода информации пользователю о всех возможных командах в архиваторе
        }
    }
    return;
}
