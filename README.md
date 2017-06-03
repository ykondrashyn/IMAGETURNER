# IMAGETURNER
IMAGETURNER is test project aimed to develop a command line tool for image processing. 
* The only supported format is currently PGM
https://en.wikipedia.org/wiki/Netpbm_format
### Features
* Rotating image on arbitrary angle via rotation matrix
* Histogram generation

## Installation
    $ git clone https://github.com/ykondrashyn/IMAGETURNER.git
    $ cd IMAGETURNER
    $ make
    
## Usage
    ./simpleocr -i tux.pgm -r 30 -o output.pgm #rotate image by 30 degrees
    ./simpleocr -n -i tux.pgm #show info
    
