#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/wait.h> 

#define LOG_PATH "./logs/"
#define LOG_FORMAT "[%02d:%02d:%04d]-[%02d:%02d:%02d]-%d-%s_%s\n"

volatile sig_atomic_t running = 1;

void signal_handler(int sig) {
    running = 0;
}

void log_activity(const char *user, const char *process, int pid, const char *status) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char filename[256];
    sprintf(filename, "%s%s.log", LOG_PATH, user);
    FILE *file = fopen(filename, "a");
    if (file) {
        fprintf(file, "[%02d:%02d:%04d]-[%02d:%02d:%02d]-%d-%s_%s\n",
                tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
                tm.tm_hour, tm.tm_min, tm.tm_sec, 
                getpid(), process, status);
        fclose(file);
    } else {
        perror("Error opening log file");
    }
}

void monitor_user(const char *user) {
    pid_t pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);
    if (setsid() < 0) exit(EXIT_FAILURE);

    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    umask(0);
    chdir("/");

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (running) {
        pid_t child_pid = fork();
        if (child_pid == 0) {
            const char *args[] = {"ps", "-u", user, NULL};
            execvp("ps", (char *const *)args);
            exit(EXIT_FAILURE);
        } else {
            int status;
            waitpid(child_pid, &status, 0);
        }
        sleep(1);
    }
}

void control_user(const char *user, int enable) {
    while (running) {
        pid_t child_pid = fork();
        if (child_pid == 0) {
            const char *args[] = {"pkill", "-u", user, NULL};
            execvp("pkill", (char *const *)args);
            exit(EXIT_FAILURE)
        } else {
            int status;
            waitpid(child_pid, &status, 0); 
        }
        sleep(1);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <option> <user>\n", argv[0]);
        printf("Options:\n");
        printf("  -m : Start monitoring user's processes\n");
        printf("  -s : Stop monitoring user's processes\n");
        printf("  -c : Control user's processes\n");
        printf("  -a : Release control on user's processes\n");
        return 1;
    }

    char *option = argv[1];
    char *user = argv[2];

    if (strcmp(option, "-m") == 0) {
        monitor_user(user);
    } else if (strcmp(option, "-s") == 0) {
        printf("Monitoring stopped for user %s\n", user);
    } else if (strcmp(option, "-c") == 0) {
        control_user(user, 1);
    } else if (strcmp(option, "-a") == 0) {
        control_user(user, 0);
    } else {
        printf("Invalid option\n");
        return 1;
    }

    return 0;
}
