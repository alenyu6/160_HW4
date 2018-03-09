#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int MAX = 20000;

typedef struct Tweeter {
    char * name;
    int tweetCount;
} Tweeter;

int getFieldPos(char * fieldName, size_t length ,char * header) {

    int pos = -1;
    int count = 0;
    char * token = strtok(header,",");
    while(token != NULL) {
        //printf("%s %s\n", token, fieldName);
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

int findTweeter( Tweeter ** list, char * name, int size) {

    if( size <= 0 )
        return -1;
    size_t nameLen = strlen(name);
    for(int i = 0; i < size; i++) {
       Tweeter * t = list[i];
       if( strncmp(name, t->name, nameLen) == 0 ) return i;
    }
    return -1;

}

//Naive n^2 sort, file size not large enough to matter
void sort(Tweeter *** list) {

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
    char * field = "\"name\"";
    int namePos = getFieldPos(field, strlen(field) ,line);
    if(namePos < 0) {
        printf("Invalid file format\n\0");
        exit(0);
    }

    int nextPos = 0;
    Tweeter * tweeterCount[MAX];

    printf("%d\n", namePos);
    while(fgets(line, 1024, ofstream)) {
        char * name = getField(namePos, line);
        int found = findTweeter(tweeterCount, name, nextPos);
        if(found >= 0) {
            tweeterCount[found]->tweetCount++;
        }
        else{
            Tweeter * tweeter = (Tweeter *)malloc(sizeof(Tweeter));
            size_t nameLen = strlen(name);
            tweeter->name = (char *)malloc( sizeof(char) * (nameLen+1) ); //Last char for null termination
            strncpy(tweeter->name,name,nameLen); //safe version of strcpy, using size_t of name
            tweeter->name[nameLen] = 0; //Need to properly terminate the string
            tweeter->tweetCount = 1;
            tweeterCount[nextPos++] = tweeter;
        }
    }

    printf("%d\n", nextPos);

    for( int i = 0; i < 10; i++ ) {
        Tweeter * t = tweeterCount[i];
        printf("%s : %d\n", t->name, t->tweetCount );
    }

    //Properly free memory after printing
    for( int i = 0; i < nextPos; i++) {
        Tweeter * t = tweeterCount[i];
        free(t->name); //free malloc'd mem for tweeter name;
        free(t); //free tweeter struct
    }

    printf("%s\n", "Finished");

}
