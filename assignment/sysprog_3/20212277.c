#include "mystdio.h"
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_PROC 5

int main(int argc, char *argv[]) {
    int fd[MAX_PROC][2];
    pid_t pid;
    pid_t parr[MAX_PROC];
    char line[256];
    
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        my_printf("File not found\n");
        return -1;
    }
    // 프로세스 개수만큼 파이프 생성
    for (int i = 0; i < MAX_PROC; i++) {
        if (pipe(fd[i]) < 0) {
            my_printf("Pipe error\n");
            return -1;
        }
    }
    
    // 자식 프로세스 생성
    for (int i = 0; i < MAX_PROC; i++) {
        pid = fork();
        if (pid < 0) {
            my_printf("Fail to fork\n");
            return -1;
        } else if (pid == 0) { // 자식 프로세스
            for (int j = 0; j < MAX_PROC; j++) { // 사용하지 않는 파이프 닫음
                if (j != i) {
                    close(fd[j][0]);
                    close(fd[j][1]);
                }
            }
            close(fd[i][1]); // 쓰기 파이프 닫고, 읽기 파이프에서 읽음
            while (read(fd[i][0],&line,sizeof(line)) > 0) {
                my_printf("%d: %s", getpid(), line);
            }
            close(fd[i][0]); // 읽기 파이프 닫기
            exit(0);
        }else{
            parr[i] = pid; // 자식 프로세스 아이디 저장
        }
    }
    
    // 부모 프로세스
    // 파일에서 한 줄씩 읽어 파이프에 쓰기
    int proc_index = 0, last_proc_index = 0;
    char c;
    int i = 0;
    while ((c = fgetc(fp)) != EOF) {
        line[i++] = c;
        if(c == '\n'){
            line[i] = '\0';
            write(fd[proc_index][1], line, sizeof(line)); // 한 줄을 파이프에 쓰기
            last_proc_index = proc_index;
            proc_index = (proc_index + 1) % MAX_PROC; // 다음 프로세스로 인덱스 업데이트
            sleep(1); // 데이터를 쓰고, 자식 프로세스가 파이프를 읽을 동안 대기
            i = 0;
        }
    }
    
    // 파일 읽기가 끝남
    for (int i = 0; i < MAX_PROC; i++) {
        close(fd[i][1]); // 쓰기 파이프 닫기
    }
    
    for (int i = last_proc_index; i >= 0; i--) {
        int pid = waitpid(parr[i], NULL, 0);
        if(i == last_proc_index){
            my_printf("%d: Read all data\n",pid);
        }
        my_printf("%d: I'm exiting...\n",pid);
        
    }
   fclose(fp); // 파일 닫기

    return 0;
}

