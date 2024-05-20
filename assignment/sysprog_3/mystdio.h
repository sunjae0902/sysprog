#ifndef _MY_STDIO_H
#define _MY_STDIO_H

#define BUFSIZE 1024
#define EOF (-1)

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define R (O_RDONLY)
#define R_PLUS (O_RDWR)
#define W (O_WRONLY | O_CREAT | O_TRUNC)
#define W_PLUS (O_RDWR | O_CREAT | O_TRUNC)
#define A (O_WRONLY | O_APPEND | O_CREAT)
#define A_PLUS (O_RDWR | O_CREAT | O_APPEND)
#define FILE_MOD (S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IWOTH | S_IROTH) // 파일 생성 시 부여하는 권한

#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/stat.h>

typedef struct myFile {
    int fd;
    int pos; // 버퍼 내의 현재 위치를 가리킴.
    int size;
    int mode; // 1 - 6으로 정의
    char *buffer;
} FILE;

FILE *fopen(const char *pathname, const char *mode);
int fread(void *ptr, int size, int nmemb, FILE *stream);
int fwrite(const void *ptr, int size, int nmemb, FILE *stream);
int fflush(FILE *stream);
int fseek(FILE *stream, int offset, int whence);
int feof(FILE *stream);
int fclose(FILE *stream);
int fgetc(FILE *stream);
int fputc(int character, FILE *stream);
void my_printf(const char *format, ...);


int int_to_string(int num, char *buffer) {
    int i = 0;
    int is_negative = 0;
    
    // 음수인 경우 처리
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }
    
    // 숫자를 역순으로 문자열에 저장
    do {
        buffer[i++] = num % 10 + '0';
        num /= 10;
    } while (num != 0);
    
    // 음수 처리
    if (is_negative)
        buffer[i++] = '-';
    
    // 문자열 역순으로 되어있으므로 뒤집기
    int j = 0;
    int k = i - 1;
    while (j < k) {
        char temp = buffer[j];
        buffer[j++] = buffer[k];
        buffer[k--] = temp;
    }
    
    // 널 종료 문자 추가
    buffer[i] = '\0';
    
    return i;
}
void my_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    
    while (*format != '\0') {
        if (*format == '%') {
            format++;
            if (*format == 'd') {
                // 정수 출력 처리
                int num = va_arg(args, int);
                char buffer[12]; // 정수 최대 자릿수 + 부호 + 널 종료 문자('\0')
                int_to_string(num, buffer);
                int i = 0;
                while (buffer[i] != '\0') {
                    write(1, &buffer[i], 1);
                    i++;
                }
            } else if (*format == 'c') {
                // 문자 출력 처리
                char ch = (char)va_arg(args, int);
                write(1, &ch, 1);
            } else if (*format == 's') {
                // 문자열 출력 처리
                char *str = va_arg(args, char*);
                int i = 0;
                while (str[i] != '\0') {
                    write(1, &str[i], 1);
                    i++;
                }
            }
        } else {
            // 일반 문자 출력
            write(1, format, 1);
        }
        format++;
    }
    
    va_end(args);
}


FILE *fopen(const char *pathname, const char *mode){
    FILE *file = malloc(sizeof(FILE));
    int tmpMode;
    
    if(strcmp("r", mode) == 0) tmpMode = 1;
    else if (strcmp("r+", mode) == 0) tmpMode = 2;
    else if (strcmp("w",  mode) == 0) tmpMode = 3;
    else if (strcmp("w+", mode) == 0) tmpMode = 4;
    else if (strcmp("a",  mode) == 0) tmpMode = 5;
    else if (strcmp("a+",  mode) == 0) tmpMode = 6;
    else return NULL;
    
    switch(tmpMode){
        case 1:
            if((file->fd = open(pathname, R)) < 0) return NULL;
            file->mode = R;
            break;
        case 2:
            if((file->fd = open(pathname, R_PLUS)) < 0) return NULL;
            file->mode = R_PLUS;
            break;
        case 3:
            if((file->fd = open(pathname, W, FILE_MOD)) < 0) return NULL;
            file->mode = W;
            break;
        case 4:
            if((file->fd = open(pathname, W_PLUS, FILE_MOD)) < 0) return NULL;
            file->mode = W_PLUS;
            break;
        case 5:
            if((file->fd = open(pathname, A, FILE_MOD)) < 0) return NULL;
            file->mode = A;
            break;
        case 6:
            if((file->fd = open(pathname, A_PLUS, FILE_MOD)) < 0) return NULL;
            file->mode = A_PLUS;
            break;
        default:
            return NULL;
    }
    
    file->buffer = (char *)malloc(BUFSIZE); // 동적 할당하여 버퍼 초기화
    if (file->buffer == NULL) {
        my_printf("Fail to allocate buffer\n");
        free(file);
        return NULL;
    }
    return file;
}

int fread(void *ptr, int size, int nmemb, FILE *stream){
    if(stream->mode & O_WRONLY){
        my_printf("Error: Permission denied\n");
        return 0;
    }

    int total_bytes_read = 0; // 읽어 온 바이트 수
    int bytes_to_read = size * nmemb; // 읽어야 할 데이터 크기
    int rest = 0;
    int offset = 0;
  
    int cur_size = stream->pos; // 버퍼에 남은 데이터 크기
   
    while(bytes_to_read > 0){ // 읽어야 할 데이터가 버퍼에 남은 크기보다 더 크다 -> 더 읽어와
        rest = cur_size - offset; // 버퍼에 남은 데이터 크기
        if(bytes_to_read >= rest){ // 추가로 읽어와야 함
            memcpy(ptr + total_bytes_read, stream->buffer + offset, rest); // 버퍼에 남은거 다 써줌
            total_bytes_read += rest;
            cur_size = read(stream->fd, stream->buffer, BUFSIZE); // 최대 BUFSIZE만큼
            if (cur_size > 0) {
                offset = 0; // 버퍼 시작 위치 초기화
                bytes_to_read -= rest;
            }
            else {
                return total_bytes_read / size;
            }
        }
        else{
            memcpy(ptr + total_bytes_read, stream->buffer + offset, bytes_to_read);
            total_bytes_read += bytes_to_read;
            offset += bytes_to_read;
            bytes_to_read = 0;
        }
    }
    return total_bytes_read / size;
}

int fwrite(const void *ptr, int size, int nmemb, FILE *stream){
    size_t bytes_written = 0; // 쓰여진 바이트 수
    
    if(stream->mode & O_RDONLY){
        my_printf("Error: Permission denied\n");
        return 0;
    }
    
    size_t bytes_to_copy = size * nmemb; // 쓸 데이터 크기
    size_t remaining_space = BUFSIZE - stream->pos; // 버퍼의 최대 사이즈 - 현재 파일 포인터 위치 (쓸수있는 남은 데이터크기)
    size_t bytes_copied = bytes_to_copy < remaining_space ? bytes_to_copy : remaining_space; // 최대 1024만큼 쓸수있다
    memcpy(stream->buffer + stream->pos, ptr, bytes_copied);

    stream->pos += bytes_copied; // 업데이트
    bytes_written += bytes_copied;
    if (stream->pos == BUFSIZE){ // 버퍼만큼 읽었다면, 파일에 쓰기
        if (write(stream->fd, stream->buffer, BUFSIZE) == -1) {
            my_printf("Error: Failed to write to file\n");
            return 0;
        }
        stream->pos = 0; // 버퍼 초기화
    }
    return bytes_written / size;
    
}


int fflush(FILE *stream){
    if(stream->mode == R){
        my_printf("Error: Not allows for READ-ONLY filestream\n");
        return EOF;
    }
    
    off_t curOffset = lseek(stream->fd, 0, SEEK_CUR); // 파일 포인터 이전 위치 보관

    if(curOffset == (off_t) -1) return EOF;
    if(write(stream->fd, stream->buffer, stream->pos) != stream->pos) return EOF; // 버퍼에 남은 데이터를 쓴다
    stream->pos = 0; //버퍼 초기화
    if(lseek(stream->fd, curOffset, SEEK_SET) == (off_t)(-1)) return EOF;
    
    return 0;
}

int fseek(FILE *stream, int offset, int whence){
    off_t newOffset = (lseek(stream->fd, (off_t) offset, whence) < 0) ? -1 : 0; // 성공 시 0, 그렇지 않으면 -1반환
    return newOffset;
}

int feof(FILE *stream){
    char buffer;
    ssize_t result = read(stream->fd, &buffer, 1);
    if (result == 0) {
        return EOF;
    } else if (result == -1) {
        return 0;
    } else {
        if (lseek(stream->fd, -1, SEEK_CUR) == -1) {
            return 0;
        }
        return 0;
    }
}

int fclose(FILE *stream){
    if(stream->mode != R){
        if(stream->pos > 0){ // 버퍼에 데이터가 있는 경ㅇ
            if(fflush(stream) == EOF) return EOF;
        }
    }
    if(close(stream->fd) < 0) return EOF;
    free(stream->buffer);
    free(stream);
    return 0;
    
}

int fgetc(FILE *stream){
    if(stream->mode & O_WRONLY){
        // my_printf("Error: Permission denied\n");
        return EOF;
    }
    else{
        if(lseek(stream->fd, 0, SEEK_CUR) == (off_t)(-1)) return EOF;
        char c;
        if (read(stream->fd, &c, sizeof(char)) == 1) {
            return (int)c;
        }
        return EOF;
    }
    
}

int fputc(int character, FILE *stream){
    if(stream->mode & O_RDONLY){
        // my_printf("Error: Permission denied\n");
        return EOF;
    }
    char ch = (char)character;
    if (write(stream->fd, &ch, 1) == 1) {
        return character;
    } else {
        return EOF;
    }
}

#endif
