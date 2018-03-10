#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t MAX_LINES = 20000;
size_t MAX_TWEETERS = 6300;
size_t LINE_LENGTH = 375;

typedef struct Tweeter {
    char * name;
    int tweetCount;
} Tweeter;

int getFieldPos(char * fieldName, size_t length ,char * header) {

    int pos = -1;
    int count = 0;
    char * token = strtok(header,",\n"); //In case name is in last column
    while(token != NULL) {
        char * trimmed;
        int end = strlen(token)-1;
        //Mutates string to remove quotes
        if( token[0] == '"' && token[end] == '"' ) {
            trimmed = token + 1;
            trimmed[end - 1] = 0;
        }
        else trimmed = token;
        if( strncmp(trimmed, fieldName, length ) == 0)
            return count;
        token = strtok(NULL, ",\n");
        count++;
    }
    return pos;

}

char * getField(int pos, char * line) {
    int count = 0;
    char * token = strtok(line, ",\n");
    if( pos == 0 ) { //If field is first return token;
        return token;
    }
    while(token != NULL) {
        token = strtok(NULL, ",\n");
        count++;
        if(count == pos) return token;
    }
    return NULL;
}

int findTweeter( Tweeter * list, char * name, int size) {

    if( size <= 0 )
        return -1;
    size_t nameLen = strlen(name);
    for(int i = 0; i < size; i++) {
       Tweeter t = list[i];
       if( strncmp(name, t.name, nameLen) == 0 ) return i;
    }
    return -1;

}

void sort(Tweeter * list, int size) {
    for(int i = 1; i < size; i++){
        for(int j = i; j > 0; j--){
            if( list[j-1].tweetCount < list[j].tweetCount) {
                Tweeter toSwap = list[j-1];
                list[j-1] = list[j];
                list[j] = toSwap;
            }
        }
    }
}

int main(int argc, char ** argv) {

    char * fileName = argv[1];
    FILE * ofstream = fopen(fileName, "r");

    if(ofstream == NULL) {
        printf("File does not exist\n");
        exit(0);
    }

    char line[1024];
    fgets(line, 1024, ofstream);

    if( strlen(line) > LINE_LENGTH ) {
        printf("Invalid file format - Exceeded line length\n");
        exit(0);
    }

    char * field = "name";
    int namePos = getFieldPos(field, strlen(field) ,line);
    if(namePos < 0) {
        printf("Invalid file format\n");
        exit(0);
    }

    int nextPos = 0;
    unsigned int numLines = 1;
    Tweeter tweeterCount[MAX_TWEETERS];

    while( fgets(line, 1024 , ofstream) ) {

        if(numLines > MAX_LINES) {
            printf("Invalid file format - Exceeded number of lines read\n");
            exit(0);
        }
        if( strlen(line) > LINE_LENGTH ) {
            printf("Invalid file format - Exceeded line length\n");
            exit(0);
        }

        char * name = getField(namePos, line);

        if( name == NULL || strlen(name) > LINE_LENGTH ) {
            printf("Invalid file format - Exceeded name length\n");
            exit(0);
        }

        int found = findTweeter(tweeterCount, name, nextPos);
        if(found >= 0) {
            tweeterCount[found].tweetCount++;
        }
        else{
            Tweeter tweeter;
            size_t nameLen = strlen(name);
            tweeter.name = (char *)malloc( sizeof(char) * (nameLen+1) ); //Last char for null termination
            strncpy(tweeter.name,name,nameLen); //safe version of strcpy, using size_t of name
            tweeter.name[nameLen] = 0; //Need to properly terminate the string
            tweeter.tweetCount = 1;
            tweeterCount[nextPos++] = tweeter;
        }
        numLines++;
    }

    //Insertion O(n^2) sort
    if(nextPos > 0) { //Header with no tweets
        sort(tweeterCount, nextPos); 

        int printCount = (nextPos > 10) ? 10 : nextPos; //Limit to print 10

        for( int i = 0; i < printCount; i++ ) {
            Tweeter t = tweeterCount[i];
            printf("%s : %d\n", t.name, t.tweetCount );
        }
    }

    //Properly free memory after printing
    for( int i = 0; i < nextPos; i++) {
        Tweeter t = tweeterCount[i];
        free(t.name); //free malloc'd mem for tweeter name;
    }

    //printf("%s\n", "Finished");

}
