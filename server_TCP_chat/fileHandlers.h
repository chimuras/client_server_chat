#ifndef FILEHANDLERS_H
#define FILEHANDLERS_H

int checkFileExist(unsigned char* filePath);
void findBounds(int* start, int* end, char searchChar, unsigned char* info);
unsigned char* getSubString(int start, int end, unsigned char* string);
unsigned char* combine_chars(unsigned char *s1, unsigned char *s2);


#endif /* FILEHANDLERS_H */

