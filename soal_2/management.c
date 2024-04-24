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
#include <sys/wait.h>
#include <signal.h>
#include <ctype.h>
#include <pwd.h>

void buatLog(char *username, char *filename, char *action)
{
    pid_t pid = fork();

    if (pid == 0)
    {
        FILE *f = fopen("/home/rafaelega24/SISOP/modul2/2/history.log", "a");
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        fprintf(f, "[%s][%02d:%02d:%02d] - %s - %s\n", username, tm.tm_hour, tm.tm_min, tm.tm_sec, filename, action);
        fclose(f);
    }
    else
    {
        int stat;
        waitpid(pid, &stat, 0);
    }
}

void donlot()
{
    pid_t pid = fork();

    if (pid == 0)
    {
        FILE *out = fopen("/home/rafaelega24/SISOP/modul2/2/log.txt", "w");
        if (out == NULL)
        {
            perror("Gabisa buka log");
            exit(EXIT_FAILURE);
        }
        dup2(fileno(out), 1);
        dup2(fileno(out), 2);
        fclose(out);
        char *donlot[] = {"wget", "https://drive.google.com/uc?export=download&id=1rUIZmp10lXLtCIH3LAZJzRPeRks3Crup", "-O", "/home/rafaelega24/SISOP/modul2/2/library.zip", NULL};
        execvp("wget", donlot);
    }
    else
    {
        int stat;
        waitpid(pid, &stat, 0);
    }
}

void unjip()
{
    pid_t pid = fork();

    if (pid == 0)
    {
        FILE *out = fopen("/home/rafaelega24/SISOP/modul2/2/log.txt", "w");
        if (out == NULL)
        {
            perror("Gabisa buka log");
            exit(EXIT_FAILURE);
        }
        dup2(fileno(out), 1);
        dup2(fileno(out), 2);
        fclose(out);

        char *unjip[] = {"unzip", "/home/rafaelega24/SISOP/modul2/2/library.zip", "-d", "/home/rafaelega24/SISOP/modul2/2", NULL};
        execvp("unzip", unjip);
    }
    else
    {
        int stat;
        waitpid(pid, &stat, 0);
    }
}

void dekrip(char *direktori)
{
    struct dirent *entry;
    DIR *dir = opendir(direktori);

    pid_t pid = fork();

    if (pid == 0)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
            {
                char ch, text[100];
                int key = 19;
                char oldFileName[500], newFileName[500];
                sprintf(oldFileName, "%s/%s", direktori, entry->d_name);
                strcpy(text, entry->d_name);

                if (text[0] < '0' || text[0] > '9')
                {
                    for (int i = 0; text[i] != '\0'; ++i)
                    {

                        ch = text[i];
                        // Check for valid characters.
                        if (isalnum(ch))
                        {
                            // Lowercase characters.
                            if (islower(ch))
                            {
                                ch = (ch - 'a' - key + 26) % 26 + 'a';
                            }
                            // Uppercase characters.
                            if (isupper(ch))
                            {
                                ch = (ch - 'A' - key + 26) % 26 + 'A';
                            }
                        }
                        // Adding decoded character back.
                        text[i] = ch;

                        sprintf(newFileName, "%s/%s", direktori, text);
                    }
                    rename(oldFileName, newFileName);
                }
            }
        }
        closedir(dir);
    }
    else
    {
        int stat;
        waitpid(pid, &stat, 0);
    }
}

void rinemDanDilet(char *direktori)
{
    struct dirent *entry;
    DIR *dir = opendir(direktori);

    pid_t pid = fork();

    if (pid == 0)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
            {
                char filenya[100];
                char namaLama[500], namaBaru[500];
                strcpy(filenya, entry->d_name);

                if (strstr(filenya, "d3Let3") != NULL)
                {
                    sprintf(namaLama, "%s/%s", direktori, entry->d_name);
                    remove(namaLama);
                    buatLog(getpwuid(getuid())->pw_name, entry->d_name, "Successfully deleted.");
                }
                else if (strstr(filenya, "r3N4mE") != NULL)
                {
                    sprintf(namaLama, "%s/%s", direktori, entry->d_name);
                    if (strstr(filenya, ".ts") != NULL)
                    {
                        sprintf(namaBaru, "%s/helper.ts", direktori);
                        rename(namaLama, namaBaru);
                    }
                    else if (strstr(filenya, ".py") != NULL)
                    {
                        sprintf(namaBaru, "%s/calculator.py", direktori);
                        rename(namaLama, namaBaru);
                    }
                    else if (strstr(filenya, ".go") != NULL)
                    {
                        sprintf(namaBaru, "%s/server.go", direktori);
                        rename(namaLama, namaBaru);
                    }
                    else
                    {
                        sprintf(namaBaru, "%s/renamed.file", direktori);
                        rename(namaLama, namaBaru);
                    }
                    buatLog(getpwuid(getuid())->pw_name, entry->d_name, "Successfully renamed.");
                }else{
                    continue;
                }
            }
        }
        closedir(dir);
    }
    else
    {
        int stat;
        waitpid(pid, &stat, 0);
    }
}

void buatFolder(char *direktori)
{
    pid_t pid = fork();

    if (pid == 0)
    {
        char *buat[] = {"mkdir", "-p", direktori, NULL};
        execvp("mkdir", buat);
    }
    else
    {
        int stat;
        waitpid(pid, &stat, 0);
    }
}
void pindahin(char *direktoriAsal, char *direktoriTujuan, char *action){
    struct dirent *entry;
    DIR *dir = opendir(direktoriAsal);

    pid_t pid = fork();

        if (pid == 0) {
            while ((entry = readdir(dir)) != NULL) {
                if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                    char filenya[500], dirFile[500];
                    char dirTujuan[500];
                    strcpy(dirTujuan, direktoriTujuan);
                    strcat(dirTujuan, "/");

                    strcpy(filenya, entry->d_name);
                    strcpy(dirFile, direktoriAsal);
                    strcat(dirFile, "/");
                    strcat(dirFile, filenya);

                    if (strstr(filenya, "m0V3") != NULL){
                        buatLog(getpwuid(getuid())->pw_name, filenya, action);
                        FILE *out = fopen("/home/rafaelega24/SISOP/modul2/2/log.txt", "w");
                        if (out == NULL) {
                            perror("Gabisa buka log");
                            exit(EXIT_FAILURE);
                        }
                        dup2(fileno(out), 1);
                        dup2(fileno(out), 2);
                        fclose(out);

                        char *pindah[] = {"mv", dirFile, dirTujuan, NULL};
                        execvp("mv", pindah);
                }else{
                    continue;
                }                     
                }
            }
            closedir(dir);
        }else if(pid < 0){
            exit(EXIT_FAILURE); 
        }else{
            wait(NULL);
        }
}

int mode, stop = 0;
void jenisMode(int sig)
{
    pid_t pid = fork();

    if (pid == 0)
    {
        if (sig == SIGRTMIN)
        {
            mode = 0;
        }
        else if (sig == SIGUSR1)
        {
            mode = 1;
        }
        else if (sig == SIGUSR2)
        {
            mode = 2;
        }
        else if (sig == SIGTERM)
        {
            stop = 1;
            exit(EXIT_SUCCESS);
        }
    }
    else
    {
        int stat;
        waitpid(pid, &stat, 0);
    }
}

int main(int argc, char *argv[])
{
    pid_t pid, sid;
    char libDir[] = "/home/rafaelega24/SISOP/modul2/2/library";
    char bekapDir[] = "/home/rafaelega24/SISOP/modul2/2/library/backup";

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

    if ((chdir("/")) < 0)
    {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    signal(SIGRTMIN, jenisMode);
    signal(SIGUSR1, jenisMode);
    signal(SIGUSR2, jenisMode);
    signal(SIGTERM, jenisMode);

    if (argc == 3 && strcmp(argv[1], "-m") == 0)
    {
        if (strcmp(argv[2], "backup") == 0)
        {
            mode = 1;
        }
        else if (strcmp(argv[2], "restore") == 0)
        {
            mode = 2;
        }
    }
            donlot();
            unjip();
            dekrip(libDir);
            rinemDanDilet(libDir);
            buatFolder(bekapDir);

    while (!stop){
        if (mode == 1)
        {
            pindahin(libDir, bekapDir, "Successfully moved to backup.");
        }
        else if (mode == 2)
        {
            pindahin(bekapDir, libDir, "Successfully restored from backup.");
        }
        sleep(1);
    }   
}
