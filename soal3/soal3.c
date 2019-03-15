#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

int main()
{
    pid_t c1, c2, c3;
    int p1, argv[4];
    char tmp[100000];
    c1 = fork();

    if(c1 == 0){
        execlp("unzip","unzip","campur2.zip", NULL);
        exit(1);
    }else{
        int p2;
        while(wait(&p1)>0);

        pipe(argv);
        pipe(argv + 2);
       
        c2 = fork();
        if(c2==0){
            close(argv[2]);
            close(argv[3]);
            close(argv[0]);
            dup2(argv[1],STDOUT_FILENO);
            close(argv[1]);
            execlp("ls","ls","campur2",NULL);
        }else{
            wait(NULL);
            c3 = fork();
            if(c3==0){
                close(argv[2]);
                dup2(argv[3],STDOUT_FILENO);
                close(argv[3]);
                close(argv[1]);
                dup2(argv[0],STDIN_FILENO);
                close(argv[0]);
                
                execlp("grep","grep",".txt$",NULL);
            }else{
                close(argv[3]);
                close(argv[0]);
                close(argv[1]);
                int byte = read(argv[2],tmp,sizeof(tmp));
                FILE* fin;
                fin = fopen("daftar.txt","w+");
                fputs(tmp,fin);
                fclose(fin);
                close(argv[2]);
            }
            
        }
    }
}
