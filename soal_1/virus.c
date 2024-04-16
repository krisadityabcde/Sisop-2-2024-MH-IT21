#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <syslog.h>


void buatlog(const char *diredit, char *namafile) {
    FILE *log = fopen("/home/rafaelega24/SISOP/modul2/1/virus.log", "a");
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    char waktu[100];
    strftime(waktu, sizeof(waktu), "[%d-%m-%Y][%H:%M:%S]", timeinfo);
    fprintf(log, "%s Suspicious string at %s successfully replaced!\n", waktu, namafile);
    fclose(log);
}

char *editKonten(char *str, char *sebelum, char *setelah) {
    char *pos, *temp;
    int index, sebelum_len;
    int i, count = 0;

    sebelum_len = strlen(sebelum);

    for (i = 0; str[i] != '\0'; i++) {
        if (strstr(&str[i], sebelum) == &str[i]) {
            count++;
            i += sebelum_len - 1;
        }
    }

    temp = (char *)malloc(i + count * (strlen(setelah) - sebelum_len) + 1);

    i = 0;
    while (*str) {
        if ((pos = strstr(str, sebelum)) == str) {
            strcpy(&temp[i], setelah);
            i += strlen(setelah);
            str += sebelum_len;
        } else {
            temp[i++] = *str++;
        }
    }

    temp[i] = '\0';
    return temp;
}

void editfile(const char *diredit) {
    struct dirent *entry;
    struct stat stat;
    char file_path[1024];
    char buffer[1024];
    char *kontenUdahDiEdit;

    DIR *dir = opendir(diredit);
    if (!dir) return;

    chdir(diredit);
    while((entry = readdir(dir))){
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        if(lstat(entry->d_name, &stat) == -1) continue;

        if(S_ISDIR(stat.st_mode)){
            editfile(entry->d_name);
        }else if(S_ISREG(stat.st_mode)){
            sprintf(file_path, "%s/%s", diredit, entry->d_name);

            FILE *awal = fopen(file_path, "r");
            if (awal != NULL){
                fread(buffer, 1, sizeof(buffer), awal);
                fclose(awal);

                kontenUdahDiEdit = buffer;
                kontenUdahDiEdit = editKonten(kontenUdahDiEdit, "m4LwAr3", "[MALWARE]");
                kontenUdahDiEdit = editKonten(kontenUdahDiEdit, "5pYw4R3", "[SPYWARE]");
                kontenUdahDiEdit = editKonten(kontenUdahDiEdit, "R4nS0mWaR3", "[RANSOMWARE]");
            
              if (strcmp(buffer, kontenUdahDiEdit) != 0) {
                    awal = fopen(file_path, "w");
                    fputs(kontenUdahDiEdit, awal);
                    fclose(awal);
                    buatlog(diredit, entry->d_name);
                    sleep(15);
              }
            }
        }
    }
  closedir(dir);
}

int main(int argc, char *argv[]){

  pid_t pid, sid;

  pid = fork();

  if (pid < 0)
  {
  exit(EXIT_FAILURE);
  }

  if (pid > 0)
  {
  exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();
  if (sid < 0)
  {
  exit(EXIT_FAILURE);
  }

  if (chdir("/") < 0)
  {
  exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO); 
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while (1){
  sleep(15);
  editfile(argv[1]);
  }

  return 0;
}
