#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Tweeter {
    char * name;
    int tweetCount;
};

int getFieldPos(char * fieldName, char * header) {

    int pos = -1;
    int count = 0;
    char * token = strtok(header,",");
    while(token != NULL) {
        printf("%s %s\n", token, fieldName);
        //printf("Compare: %d\n", strcmp(token, fieldName ));
        if( strcmp(token, fieldName ) == 0)
            pos = count;
        token = strtok(NULL, ",");
        count++;
    }
    return pos;

}

char * getField(int pos, char * line) {
    int count = 0;
    char * token = strtok(line, ",");
    while(token != NULL) {
        token = strtok(NULL, ",");
        count++;
        if(count == pos) return token;
    }
    return NULL;
}

int main(int argc, char ** argv) {

    char * fileName = argv[1];
    FILE * ofstream = fopen(fileName, "r");

    if(ofstream == NULL) {
        printf("File does not exist\n\0");
        exit(0);
    }
    char line[1024];
    fgets(line, 1024, ofstream);
    int namePos = getFieldPos("\"name\"", line);
    printf("%d\n", namePos);
    while(fgets(line, 1024, ofstream)) {
        printf("%s\n", getField(namePos, line));
    }

    //printf("%s", "Hello World\n\0");

}
