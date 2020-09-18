#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define BAD_NUMBER_ARGS 1

/**
 * Parses the command line.
 *
 * argc: the number of items on the command line (and length of the
 *       argv array) including the executable
 * argv: the array of arguments as strings (char* array)
 * bits: the integer value is set to TRUE if bits output indicated
 *       outherwise FALSE for hex output
 *
 * returns the input file pointer (FILE*)
 **/
FILE *parseCommandLine(int argc, char **argv, int *bits) {
  if (argc > 2) {
    printf("Usage: %s [-b|-bits]\n", argv[0]);
    exit(BAD_NUMBER_ARGS);
  }

  if (argc == 2 &&
      (strcmp(argv[1], "-b") == 0 || strcmp(argv[1], "-bits") == 0)) {
    *bits = TRUE;
  } else {
    *bits = FALSE;
  }

  return stdin;
}

/**
 * Writes data to stdout in hexadecimal.
 *
 * data: an array of no more than 16 characters
 * size: the size of the array
 **/
void printDataAsHex(unsigned char *data, size_t size) {
  for(int i = 0; i < size; i++) {
    if(i % 2 == 0) {
      printf(" "); //Seperate every two bytes
    }	
    printf("%02x", data[i]);

  }

  if(size < 16) {
    for(int i = size; i < 16; i++) {
      printf("  ");
    
      if(i % 2 == 1){
        printf(" "); //Seperate every two bytes
      }	     //Fill up with zeros
    }
  }

}

/**
 * Algorithm to convert and print a decimal number to binary
 **/
void printDecimalInBinary(int decimalNum) {
  char binaryNum[8] = "00000000";
  int powerOfTwoIndex = 0;

  while(decimalNum > 0) { //Decimal to binary algorithm
    if(decimalNum % 2 == 1) {
      binaryNum[7 - powerOfTwoIndex] = '1'; //Place the least sig digit at the end of the array
    }

    decimalNum = decimalNum / 2;
    powerOfTwoIndex++;
  }
  
  printf(" %s", binaryNum);
}

/**
 * Writes data to stdout in binary.
 *
 * data: an array of no more than 8 characters
 * size: the size of the array
 **/
void printDataAsBits(unsigned char *data, size_t size) {
  for(int i = 0; i < size; i++) {
    printDecimalInBinary(data[i]);
  }

  if(size < 6) { //Fill the remaining space with spaces (if required)
    for(int i = size; i < 6; i++){
      printf("         ");
    }
  }
}

/**
 * Writes data to stdout as characters.
 *
 * See myxxd.md for details.
 *
 * data: an array of no more than 16 characters
 * size: the size of the array
 **/
void printDataAsChars(unsigned char *data, size_t size) {

  for(int i = 0; i < size; i++) {

    if(data[i] >= 32 && data[i] <= 126) {
      printf("%c", data[i]);
    }
    else { 
      printf("."); //Add period on the last line
    }

  }

}

/**
 * Reads input and prints in hex
 *
 * input: input stream
 **/
void readAndPrintInputAsHex(FILE *input) {
  unsigned char data[16];
  int numBytesRead = fread(data, 1, 16, input);
  unsigned int offset = 0;
  while (numBytesRead != 0) {
    printf("%08x:", offset);
    offset += numBytesRead;
    printDataAsHex(data, numBytesRead);
    printf("  ");
    printDataAsChars(data, numBytesRead);
    printf("\n");
    numBytesRead = fread(data, 1, 16, input);
  }
}

/**
 * Reads input and prints in bits
 *
 * input: input stream
 **/
void readAndPrintInputAsBits(FILE *input) {
  unsigned char data[6];
  int numBytesRead = fread(data, 1, 6, input);
  unsigned int offset = 0;
  while (numBytesRead != 0) {
    printf("%08x:", offset);
    offset += numBytesRead;
    printDataAsBits(data, numBytesRead);
    printf("  ");
    printDataAsChars(data, numBytesRead);
    printf("\n");
    numBytesRead = fread(data, 1, 6, input);
  }
}

int main(int argc, char **argv) {
  int bits = FALSE;
  FILE *input = parseCommandLine(argc, argv, &bits);

  if (bits == FALSE) {
    readAndPrintInputAsHex(input);
  } else {
    readAndPrintInputAsBits(input);
  }
  return 0;
}
