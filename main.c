#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "turner.h"

#include <ctype.h>
#include <unistd.h>

void printHelp() {
    fprintf (stderr, "Usage: simpleocr [-i filename] [OPTIONS] [-o filename]\n");
    fprintf (stderr, "This tool is a test task. It's intended to process a PGM image\n\n");
    fprintf (stderr, "Options are: \n");
    fprintf (stderr, " -h             display this help and exit\n");
    fprintf (stderr, " -i filename    load an input file\n");
    fprintf (stderr, " -n             show information regarding the input file\n");
    fprintf (stderr, " -r             rotate the image on arbitrary angle\n");
    fprintf (stderr, " -o filename    specify the output file\n");
}

int main(int argc, char *argv[]) {
    imagePGM srcImg;
    imagePGM dstImg;
    char *srcFilename;
    char *dstFilename;
    int FileLoad = 0, FileLoadFilename = 0, FileSave = 0, FileSaveFilename = 0, flagInfo = 0, FileInfoOptions = 0;
    int FileImage = 0, FileImageOptions = 0, FileImageRotate = 0, FileImageRotateOptions = 0, rotAngle = 0;
    int c;
    int index;

    while ((c = getopt(argc, argv, "i:o:hnr:")) != -1) {
        switch (c) {
            case 'h':
                printHelp();
                exit(EXIT_SUCCESS);
            case 'n':
                flagInfo = 1;
                break;
            case 'i':
                FileLoad = 1;
                FileLoadFilename = 1;
                srcFilename = optarg;
                break;
            case 'o':
                FileSave = 1;
                FileSaveFilename = 1;
                dstFilename = optarg;
                break;
            case 'r':
                FileImage = 1;
                FileImageOptions = 1;
                rotAngle = atoi(optarg);
                break;
            case '?':
                break;
                /*
                if (optopt == 'i')
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                else if (optopt == 'o')
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                else if (optopt == 'r')
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint(optopt))
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
                return 1;
                */
            default:
                fprintf(stderr, "Wrong arguments. try simpleocr -h for help.");
                exit(EXIT_FAILURE);
        }
    }
    
    ///printf("src = %s\n", dstFilename);
    
    if (FileLoad && FileLoadFilename) {
        srcImg = loadPGM(srcFilename);
        if (flagInfo) {
            printf ("Properties:\n");
            printf ("format: %s\n", getFormatPGM(srcImg));
            printf ("depth: %d\n", getMaxIntensityPGM(srcImg));
            printf ("width: %d\n", getColsPGM(srcImg));
            printf ("height: %d\n", getRowsPGM(srcImg));
        }
    }
    
    if (FileSave && FileSaveFilename) {
        if (FileImage) {
            dstImg = rotatePGM(srcImg, rotAngle);
        }
        savePGM(dstFilename, dstImg);
        freePGM(dstImg);
        printf ("Done\n");
    }
    
    return 0;
}