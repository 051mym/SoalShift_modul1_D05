#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <time.h>

using namespace std;

int main() {
    pid_t pid, sid;
    //1 fork parent
    pid = fork();
    //error handling
    if (pid < 0) exit(EXIT_FAILURE);
    //1 stop parent = orphan process
    if (pid > 0) exit(EXIT_SUCCESS);
    //2 ubah mode file agar dapat di r/w
    umask(0);
    //3 buat session id unik dari kernel agar dpt berjalan
    sid = setsid();
    //error handling
    if (sid < 0) exit(EXIT_FAILURE);
    //4 ubah dir kerja ke tempat yang pasti selalu ada
    if ((chdir("/")) < 0) exit(EXIT_FAILURE);
    //5 tutup file descriptor std
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    //main program
    string pathEnak="/home/yusuf/Documents/makanan/makan_enak.txt";
    int inc=1;
    //6 loop utama jika diperlukan program berjalan kontinyu
    while(true)
    {
        //membandingkan range 0-30
        struct stat statEnak;
        stat(pathEnak.c_str(), &statEnak);
        time_t timeProgram = time(NULL);
        time_t timeEnak = statEnak.st_atime;

        //jika perbedaan detik program dengan detik akses terakhir file <= 30
        if (difftime(timeProgram,timeEnak)<=30) {
            //generate file diet
            string pathSehat="/home/yusuf/Documents/makanan/makan_sehat";
            pathSehat += to_string(inc) + ".txt";
            FILE *sehat=fopen((char*)pathSehat.c_str(),"w");
            fclose(sehat);
            inc++;
        }
        //setiap 5 detik delay = berjalan setiap 5 detik
        sleep(5);
    }
    exit(EXIT_SUCCESS);
}
