#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "turner.h"
#include <math.h>

#define DEFAULT_MAX_GRAY 255

struct netpbm {
    char type[3];
    unsigned int intCol;
    unsigned int intRow;
    unsigned int depth;
    uint8_t **matrix;
};

static void writeHeaderPGM(imagePGM img, FILE *pgmFile);
static void loadDataPGM(imagePGM img, FILE *pgmFile);
static void skipCommentsPGM(FILE *pgmFile);
static void swap(uint8_t *array, unsigned int pos1, unsigned int pos2);

/*
 * FILE-RELATED FUNCTIONS
 */
imagePGM openPGM(const char *filename) {
    unsigned int col, row, depth, line;
    FILE *pgmFile;
    imagePGM img;
    char type[3];

    // Open file for reading
    pgmFile = fopen(filename, "rb");
    if (!pgmFile) {
        fprintf(stderr, "Error opening file\n");
        exit(EXIT_FAILURE);
    }

    // Get image type
    fgets(type, sizeof (type), pgmFile);
    if ((strcmp(type, "P5") != 0)) {
        fprintf(stderr, "Error: Unsupported image.");
        exit(EXIT_FAILURE);
    }

    // According to the standard PGM file may contain comments, skipping them
    skipCommentsPGM(pgmFile);
    
    // Reading image properties
    fscanf(pgmFile, "%d %d", &col, &row);
    
    // Creating an image instance
    setSizePGM(img, col, row);
    setFormatPGM(img, type);
    skipCommentsPGM(pgmFile);
    fscanf(pgmFile, "%d", &depth);
    setMaxIntensityPGM(img, depth);
    img->matrix = (uint8_t **) malloc(sizeof (uint8_t *) * img->intRow);
    if (!img->matrix) {
        free(img->matrix);
        fprintf(stderr, "Error allocation memory\n");
        exit(EXIT_FAILURE);
    }
    for (line = 0; line < row; line++) {
        img->matrix[line] = (uint8_t *) malloc((sizeof (uint8_t)) * col);
        if (!img->matrix[line]) {
            while (line >= 0) {
                // Free all used memory on error
                free(img->matrix[--line]);
            }
            free(img->matrix);
            fprintf(stderr, "Error allocation memory");
            exit(EXIT_FAILURE);
        }
        //This actually loads image data to matrix 
        fread(img->matrix[line], sizeof (uint8_t), col, pgmFile);
        //memset(img->matrix[line], 200, col);
    }

    //loadDataPGM(img, pgmFile);
    fclose(pgmFile);
    return img;
}

void savePGM(const char *filename, imagePGM img) {
    FILE *pgmFile;
    int i;
    int j;
    pgmFile = fopen(filename, "wb");
    if (!pgmFile) {
        fprintf(stderr, "Error saving image\n");
        exit(EXIT_FAILURE);
    }
    // Write image info/comments
    writeHeaderPGM(img, pgmFile);
    // Write matrix
    for (i = 0; i < getRowsPGM(img); i++)
        fwrite(img->matrix[i], sizeof (uint8_t), img->intCol, pgmFile);
    fclose(pgmFile);
}

static void writeHeaderPGM(imagePGM img, FILE *pgmFile) {
    fprintf(pgmFile, "%s\n", getFormatPGM(img));
    fprintf(pgmFile, "%d %d\n", getColsPGM(img), getRowsPGM(img));
    fprintf(pgmFile, "%d\n", getMaxIntensityPGM(img));
}

static void loadDataPGM(imagePGM img, FILE *pgmFile) {
    /* Filling the matrix
     * 
     * 
     */
    int line;

    img->matrix = (uint8_t **) malloc(sizeof (uint8_t *) * img->intRow);
    if (!img->matrix) {
        free(img->matrix);
        fprintf(stderr, "Error allocation memory\n");
        exit(EXIT_FAILURE);
    }

    for (line = 0; line < img->intRow; line++) {
        img->matrix[line] = (uint8_t *) malloc((sizeof (uint8_t)) * img->intCol);
        if (!img->matrix[line]) {
            while (line >= 0) {
                // Free all used memory on error
                free(img->matrix[--line]);
            }
            free(img->matrix);
            fprintf(stderr, "Error allocation memory");
            exit(EXIT_FAILURE);
        }
        //This actually loads image data to matrix 
        fread(img->matrix[line], sizeof (uint8_t), img->intCol, pgmFile);
        //memset(img->matrix[line], 200, col);
    }
}

static void skipCommentsPGM(FILE *pgmFile) {
    int marker;
    char comment_line[COMMENT_MAX_LINE];

    while ((marker = fgetc(pgmFile)) != EOF && isspace(marker));
    if (marker == '#') {
        fgets(comment_line, sizeof (comment_line), pgmFile);
        skipCommentsPGM(pgmFile);
    } else {
        fseek(pgmFile, -1, SEEK_CUR);
    }
}

static void swap(uint8_t *array, unsigned int pos1, unsigned int pos2) {
    uint8_t temp = array[pos1];
    array[pos1] = array[pos2];
    array[pos2] = temp;
}

imagePGM newPGM(unsigned int intCol, unsigned int intRow) {
    imagePGM newImg;
    int line;
    newImg = (imagePGM) malloc(sizeof (struct netpbm));
    if (!newImg) {
        fprintf(stderr, "Error allocation memory\n");
        exit(EXIT_FAILURE);
    }
    newImg->matrix = (uint8_t **) malloc(sizeof (unsigned int *) * intRow);
    if (!newImg->matrix) {
        free(newImg);
        fprintf(stderr, "Error allocation memory\n");
        exit(EXIT_FAILURE);
    }
    for (line = 0; line < intRow; line++) {
        newImg->matrix[line] = (uint8_t *) malloc(sizeof (unsigned int) * intCol);
        if (!newImg->matrix[line]) {

            while (line >= 0) {
                free(newImg->matrix[--line]);
            }
            free(newImg->matrix);
            free(newImg);

            fprintf(stderr, "Error allocation memory");
            exit(EXIT_FAILURE);
        }
        memset(newImg->matrix[line], 0, intCol);
    }

    newImg->intCol = intCol;
    newImg->intRow = intRow;
    setMaxIntensityPGM(newImg, DEFAULT_MAX_GRAY);
    return newImg;
}

imagePGM loadPGM(const char *chrFilename) {
    imagePGM img = openPGM(chrFilename);
    return img;
}

void freePGM(imagePGM img) {
    int line;
    for (line = 0; line < img->intRow; line++) {
        free(img->matrix[line]);
    }
    free(img->matrix);
    free(img);
}

/*
 * IMAGE-RELATED FUNCTIONS
 */
void setSizePGM(imagePGM img, unsigned int intCols, unsigned int IntRow) {
    img->intCol = intCols;
    img->intRow = IntRow;
}

void setFormatPGM(imagePGM img, char *chrType) {
    strcpy(img->type, chrType);
}

char *getFormatPGM(imagePGM img) {
    return img->type;
}

unsigned int getMaxIntensityPGM(imagePGM img) {
    return img->depth;
}

void setMaxIntensityPGM(imagePGM img, unsigned int intMaxGray) {
    img->depth = intMaxGray;
}

int getRowsPGM(imagePGM img) {
    return img->intRow;
}

int getColsPGM(imagePGM img) {
    return img->intCol;
}

void setPixelIntensityPGM(imagePGM img, unsigned int col, unsigned int line, unsigned int intensity) {
    img->matrix[line][col] = intensity;
}

unsigned int getPixelIntensityPGM(imagePGM img, unsigned int col, unsigned int row) {

    return img->matrix[row][col];
}

imagePGM getClonePGM(imagePGM img) {
    int line;
    imagePGM newImg = newPGM(img->intCol, img->intRow);

    setFormatPGM(newImg, getFormatPGM(img));

    setMaxIntensityPGM(newImg, getMaxIntensityPGM(img));

    for (line = 0; line < img->intRow; line++) {

        memcpy(newImg->matrix[line], img->matrix[line], sizeof (unsigned int) * img->intCol);
    }

    return newImg;
}

imagePGM rotatePGM90(imagePGM img) {
    //Writing cols in rows and rows in cols
    imagePGM newImg = newPGM(img->intRow, img->intCol);
    unsigned int col, row;
    setFormatPGM(newImg, getFormatPGM(img));
    setMaxIntensityPGM(newImg, getMaxIntensityPGM(img));

    for (row = 0; row < img->intCol; row++) {
        for (col = 0; col < img->intRow; col++) {
            setPixelIntensityPGM(newImg, col, row, getPixelIntensityPGM(img, row, col));
            
        }
    }

    return newImg;
}

imagePGM rotatePGM(imagePGM img, int angle_deg) {
    //
    imagePGM newImg = newPGM(img->intCol, img->intRow);
    double angle_rad = (double) angle_deg / 180 * M_PI;
    printf ("%f", angle_rad);
    unsigned int col, row, x, y;
    setFormatPGM(newImg, getFormatPGM(img));
    setMaxIntensityPGM(newImg, getMaxIntensityPGM(img));

    for (row = 0; row < img->intRow; row++) {
        for (col = 0; col < img->intCol; col++) {
            y = row*cos(angle_rad)-col*sin(angle_rad);
            x = row*sin(angle_rad)+col*cos(angle_rad);

            if (x >= newImg->intCol)
                continue;
            if (y >= newImg->intRow)
                continue;
            setPixelIntensityPGM(newImg, col, row, getPixelIntensityPGM(img, x, y));
        }
    }

    return newImg;
}
