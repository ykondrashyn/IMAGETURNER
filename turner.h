#ifndef _TURNER_
#define _TURNER_
typedef struct netpbm *imagePGM;
#define COMMENT_MAX_LINE 1024
#endif


imagePGM openPGM(const char *filename);

void savePGM(const char *filename, imagePGM img);

char *PGMsetFilenameExtension(char *filename);


imagePGM newPGM(unsigned int intCol, unsigned int intRow);

imagePGM loadPGM(const char *chrFilename);

void freePGM(imagePGM img);

void setSizePGM(imagePGM img, unsigned int intCols, unsigned int intRow);

void setFormatPGM(imagePGM img, char *MagicNumber);

char *getFormatPGM(imagePGM img);

unsigned int getMaxIntensityPGM(imagePGM img);

void setMaxIntensityPGM(imagePGM img, unsigned int intMaxGray);

int getRowsPGM(imagePGM img);

int getColsPGM(imagePGM img);

void setPixelIntensityPGM(imagePGM img, unsigned int col, unsigned int row, unsigned int intensity);

unsigned int getPixelIntensityPGM(imagePGM img, unsigned int col, unsigned int row);

imagePGM rotatePGM90(imagePGM img);

imagePGM rotatePGM(imagePGM img, int angle_deg);
