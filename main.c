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

void printMap(struct Tweeter* map, int tCount){
  printf("\n - > map:\n");
  for (int i = 0; i<tCount; i++){
    printf("#%d: <%s,%d>\n", i, map[i].name, map[i].tweetCount);
  }
  printf("-----------------\n");
}

int add2Map(struct Tweeter* map, char* username, int twCount){
  if (map == NULL) return -1;

  for (int i = 0; i<twCount-1; i++){
    if( strcmp(username, map[i].name ) == 0){
        //printf(" -> match at %d, %s=%s\n", i, map[i].name, username);
        map[i].tweetCount++;
        //printf("adding=%d <%s,%d>\n", i, map[i].name, map[i].tweetCount);
        return 1;
    }
  }
  map[twCount].name = strdup(username);
  map[twCount].tweetCount = 1;
  //printf("adding=%d <%s,%d>\n", twCount, map[twCount].name, map[twCount].tweetCount);
  return 0;
}



int main(int argc, char ** argv) {

    char * fileName = argv[1];
    FILE * ofstream = fopen(fileName, "r");

    struct Tweeter twVolume[20000];
    int twCount = 0;

    if(ofstream == NULL) {
        printf("File does not exist\n");
        exit(0);
    }

    char line[1024];
    fgets(line, 1024, ofstream);
    int namePos = getFieldPos("\"name\"", line);
    printf("usernames at pos %d\n", namePos);

    while(fgets(line, 1024, ofstream)) {
        //printf("%s\n", getField(namePos, line));
        if (add2Map(twVolume, getField(namePos, line), twCount) == 0){
          twCount++;
        }
    }
    printMap(twVolume, twCount);

    //printf("%s", "Hello World\n\0");

}
