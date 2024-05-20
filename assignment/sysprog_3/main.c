#include "mystdio.h"

char filePath[20];

void test1(){
    FILE *f = fopen(filePath,"r"); // 읽기 모드 - 입력 버퍼
    if(f == NULL) my_printf("Fail to open file\n");
    
    char c[50] ={0,},c2[50] ={0,},c3[50] ={0,};
   
    int counts = fread(&c,10, 1, f); // 10 byte 만큼, 1번
    my_printf("1. result: %s, count: %d\n",c,counts);
    
    fseek(f, 0, SEEK_SET); // 파일 포인터를 다시 처음 위치로 옮김
    
    counts = fread(&c2, 10, 3,f); // 10 byte 만큼, 3번
    my_printf("2. result: %s, count: %d\n",c2,counts);
    
    fseek(f, 0, SEEK_SET);
    
    counts = fread(&c3, 1,20,f); // 1바이트씩, 20번
    my_printf("3. result: %s, count: %d\n",c3,counts);
    
    if(fclose(f) == -1) my_printf("Fail to close file\n");

}

void test2(){
    FILE *f = fopen(filePath,"r+"); // 읽기,쓰기 모드 - 둘 다
    if(f == NULL) my_printf("Fail to open file\n");
    
    char *str = "Hello, Nice to meet you!";
    char c[100] = {0,};
    
    int counts = fwrite(str, strlen(str),1, f); // 쓰기
    
    fseek(f,0,SEEK_SET); // 쓴 내용을 읽기 위해 파일 포인터 위치 이동
    
    if (fread(c,strlen(str),1,f) == -1) { // 읽기
        my_printf("Fail to read\n");
        return;
    }
    
    my_printf("result: %s count: %d\n",c,counts);

    if(fclose(f) == -1) my_printf("Fail to close file\n");
}


void test3(){
    FILE *f = fopen(filePath,"w"); // 쓰기 모드 + 파일 존재하지 않을 경우 새로 생성 - 출력 버퍼
    if(f == NULL) my_printf("Fail to open file\n");
    
    char *str = "Hello, Nice to meet you!";
    
    int counts = fwrite(str, strlen(str),1, f); // 쓰기

    if(fclose(f) == -1) my_printf("Fail to close file\n");
}


void test4(){
    FILE *f = fopen(filePath,"w+"); // 읽기,쓰기 모드 + 파일이 존재하지 않을 경우 새로 생성 - 둘 다
    if(f == NULL) my_printf("Fail to open file\n");
    
    char *str = "Hello, Nice to meet you!";
    char c[100] = {0,};
    
    int counts = fwrite(str, strlen(str),1, f); // 쓰기
    
    fseek(f,0,SEEK_SET); // 쓴 내용을 읽기 위해 파일 포인터 위치 이동

    if (fread(c,strlen(str),1,f) == -1) { // 읽기
        my_printf("Fail to read\n");
        return;
    }
    
    my_printf("result: %s count: %d\n",c,counts);


    if(fclose(f) == -1) my_printf("Fail to close file\n");
}

void test5(){
    FILE *f = fopen(filePath,"a"); // 이어서 쓰기 모드 + 파일이 존재하지 않을 경우 새로 생성 - 출력 버퍼
    if(f == NULL) my_printf("Fail to open file\n");

    char *str = " Nice to meet you!";
    
    int counts = fwrite(str, strlen(str),1, f); // 쓰기

    if(fclose(f) == -1) my_printf("Fail to close file\n");
}

void test6(){
    FILE *f = fopen(filePath,"a+"); // 읽기 + 이어서 쓰기 모드 + 파일이 존재하지 않을 경우 새로 생성
    if(f == NULL) my_printf("Fail to open file\n");
    
    char *str = "Hello!";
    char c[BUFSIZE] = {0,};
    
    int counts = fwrite(str, strlen(str),1, f); // 쓰기
  
    fseek(f,0,SEEK_SET); // 쓴 내용을 읽기 위해 파일 포인터 위치 이동
   
    if (fread(c, BUFSIZE,1,f) == -1) { // 읽기
        my_printf("Fail to read\n");
        return;
    }
    
    my_printf("result: %s count: %d\n",c,counts);

    if(fclose(f) == -1) my_printf("Fail to close file\n");
}

void test_fseek(){
    FILE *f = fopen(filePath,"a"); // 이어서 쓰기 모드 + 파일이 존재하지 않을 경우 새로 생성 - 둘 다
    if(f == NULL) my_printf("Fail to open file\n");

    my_printf("%d ", lseek(f->fd,0,SEEK_CUR));
    
    if(feof(f))
        my_printf("It's end of file\n");
    
    fseek(f, 0, SEEK_SET);
    
    my_printf("%d ", lseek(f->fd,0,SEEK_CUR));
    
    if(fclose(f) == -1) my_printf("Fail to close file\n");
}

int main(int argc, char *argv[]){
    strcpy(filePath, argv[1]);
    test6();
  
    return 0;
}
