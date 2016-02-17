#include <stdlib.h>
#include <stdio.h>

#define MAXFILENAME 15
#define MAXSTRLEN 100

typedef struct netpbmheader {
    char type[3];
    unsigned int width;
    unsigned int height;
    unsigned int depth;
} netpbmheader;

int main(int argc, char *argv[]) {

  FILE *fp;
  long fpsize;
  long rd;
  char *buffer;
  char tmp[MAXSTRLEN];
  char filename[MAXFILENAME];
  netpbmheader imageheader;

  // Basic sanity check
  if (argv[1])
    strcpy(filename, argv[1]);
  else
    return 1;

  printf("filename 1: %s\n", filename);

  // Open file for reading
  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return 1;
  }

  // Get filesize
  fseek(fp, 0, SEEK_END);
  fpsize = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  fgets(tmp, MAXSTRLEN, fp);
  sscanf(tmp, "%s", imageheader.type);
  fgets(tmp, MAXSTRLEN, fp);
  sscanf(tmp, "%d %d", &imageheader.width, &imageheader.height);
  fgets(tmp, MAXSTRLEN, fp);
  sscanf(tmp, "%d", &imageheader.depth);
  
  buffer = (char*)malloc(sizeof(char)*fpsize);
  if (buffer == NULL) {
    printf("Memory error");
    return 2;
  }
  
  rd = fread(buffer, 1, fpsize, fp);
  if( (rd + sizeof(netpbmheader) -1)  != fpsize) {
    printf("Reading error\n");
    return 3;
  }


  printf("type: %s\n", imageheader.type); 
  printf("width: %d\n", imageheader.width); 
  printf("height: %d\n", imageheader.height); 
  printf("depth: %d\n", imageheader.depth); 
  printf("File size: %ld\n", fpsize); 




  return 0;

}

