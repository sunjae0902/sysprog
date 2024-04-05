#include "functions.h"
#include <stdio.h>
#include <stdlib.h>

void storeInputData(char **input, int size, int bits) { // bit (8,32,64) 단위로 쪼개어 2차원 문자열 포인터에 저장
    int len = (size % bits == 0) ? size / bits : size / bits+1;
    FILE *f = fopen("input","rb");
    if(f == NULL){
        printf("Fail to open file");
    }
    {
        fseek(f, -(bits+1), SEEK_END);
        for(int i = 0; i < len ; i++){
            if(i > 0)
                fseek(f, -2 * bits, SEEK_CUR);
            for (int j = 0; j < bits; j++) {
                input[i][j] = fgetc(f);
            }
        }
        fclose(f);
    }
}

void binaryToSignedChar(const char **input, int bits){ // 메모리에 저장된 비트열을 signed char로 변환 (-128~127)
    int numBytes = (bits + 7) / 8;
    int i, j, k;
    for (i = 0; i < numBytes; ++i) {
        signed char byte = 0;
        for (j = 0; j < 8; ++j) {
            k = i * 8 + j;
            if (k < bits) {
                byte <<= 1;
                byte |= (input[k / 8][k % 8] - '0');
            } else {
                byte <<= 1;
            }
        }
        printf("%d ",byte);
    }
}

void binaryToASCII(const char **input, int bits){  // 메모리에 저장된 비트열을 signed char로 변환하여 그에 해당하는 ascii code 출력
    int numBytes = (bits + 7) / 8;
    int i, j, k;
    for (i = 0; i < numBytes; ++i) {
        signed char byte = 0;
        for (j = 0; j < 8; ++j) {
            k = i * 8 + j;
            if (k < bits) {
                byte <<= 1;
                byte |= (input[k / 8][k % 8] - '0');
            } else {
                byte <<= 1;
            }
        }
        printf("%c ",((int)byte < 0 || (int)byte > 127) ? '.' : byte);
    }
}

void binaryToUnsignedChar(const char **input, int bits){  // 메모리에 저장된 비트열을 unsigned char로 변환
    int numBytes = (bits + 7) / 8;
    int i, j, k;
    for (i = 0; i < numBytes; ++i) {
        unsigned char byte = 0;
        for (j = 0; j < 8; ++j) {
            k = i * 8 + j;
            if (k < bits) {
                byte <<= 1;
                byte |= (input[k / 8][k % 8] - '0');
            } else {
                byte <<= 1;
            }
        }
        printf("%d ",byte);
    }
}

void binaryToSignedInt(const char **input, int bits){  // 메모리에 저장된 비트열을 signed int로 변환
    int numBytes = (bits + 31) / 32;
    int i, j, k, r = 0;
    
    for (i = 0; i < numBytes; ++i) {
        signed int byte = 0;
        for (j = 0; j < 32; ++j) {
            k = i * 32 + j;
            if (k < bits) {
                byte <<= 1;
                byte |= (input[k / 32][k % 32] - '0');
            } else {
                byte <<= 1;
            }
        }
        printf("%d ",byte);
    }
}

void binaryToUnsignedInt(const char **input, int bits){  // 메모리에 저장된 비트열을 unsigned int로 변환
    int numBytes = (bits + 31) / 32;
    int i, j, k;
    for (i = 0; i < numBytes; ++i) {
        unsigned int byte = 0;
        for (j = 0; j < 32; ++j) {
            k = i * 32 + j;
            if (k < bits) {
                byte <<= 1;
                byte |= (input[k / 32][k % 32] - '0');
            } else {
                byte <<= 1;
            }
        }
        printf("%u ",byte);
    }
}

void binaryToFloat(const char **input, int bits){  // 메모리에 저장된 비트열을 float로 변환
    int numBytes = (bits + 31) / 32;
    int i, j, k;
    for (i = 0; i < numBytes; ++i) {
        int byte = 0;
        for (j = 0; j < 32; ++j) {
            k = i * 32 + j;
            if (k < bits) {
                byte <<= 1;
                byte |= (input[k / 32][k % 32] - '0');
            } else {
                byte <<= 1;
            }
        }
        printf("%.4f ",(float)byte);
    }
}

void binaryToDouble(const char **input, int bits){  // 메모리에 저장된 비트열을 double로 변환
    int numBytes = (bits + 63) / 64;
    
    int i, j, k;
    for (i = 0; i < numBytes; ++i) {
        long long byte = 0;
        for (j = 0; j < 64; ++j) {
            k = i * 64 + j;
            if (k < bits) {
                byte <<= 1;
                byte |= (input[k / 64][k % 64] - '0');
            } else {
                byte <<= 1;
            }
        }
        printf("%.4lf ",(double)byte);
    }
}

int main (void){
    FILE *f = fopen("input","rb");
    int curpos = ftell(f);
    fseek(f, 0L, SEEK_END);
    int bufSize = ftell(f);
    
    char **input = (char **)malloc((bufSize - 1) / 8 * sizeof(char *));
    
    if (input == NULL) {
        printf("Memory allocation failed.\n");
        return -1;
    }
    for (int i = 0; i < (bufSize - 1) / 8; i++) {
        input[i] = (char *)malloc(8 * sizeof(char));
        if (input[i] == NULL) {
            printf("Memory allocation failed.\n");
            return -1;
        }
    }
    printf("BUFSIZE: %d\n\n",bufSize);
    
    fseek(f, curpos, SEEK_SET);
    if(f != NULL){
        
        char c;
        int i = 1;
        
        printf("input: ");
        while((c = (char) fgetc(f)) != EOF){
            printf("%c",c);
        }
        for(; i <= 7; i++){
            printf("\n%d. ",i);
            switch(i){
                case 1:
                    printf("signed char: ");
                    storeInputData(input, bufSize-1, 8); // 8비트 단위로 쪼개어 저장
                    binaryToSignedChar((const char **)input, bufSize-1);
                    break;
                case 2:
                    printf("ASCII codes: ");
                    binaryToASCII((const char **)input, bufSize-1);
                    break;
                case 3:
                    printf("unsigned char: ");
                    binaryToUnsignedChar((const char **)input, bufSize-1);
                    break;
                case 4:
                    printf("signed int: ");
                    storeInputData(input, bufSize-1, 32); // 32비트 단위로 쪼개어 저장
                    binaryToSignedInt((const char **)input, bufSize-1);
                    break;
                case 5:
                    printf("unsigned int: ");
                    binaryToUnsignedInt((const char **)input, bufSize-1);
                    break;
                case 6:
                    printf("float: ");
                    binaryToFloat((const char **)input, bufSize-1);
                    break;
                case 7:
                    printf("double: ");
                    storeInputData(input, bufSize-1, 64); // 64비트 단위로 쪼개어 저장
                    binaryToDouble((const char **)input, bufSize-1);
                    break;
            }
        }
    }
    else{
        printf("Fail to open file.\n");
    }
    for (int i = 0; i < (bufSize - 1) / 8; i++) { // 메모리 해제
        free(input[i]);
    }
    free(input);
    fclose(f);
    
    return 0;
}


