#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define MAX_APPS 10
#define MAX_APP_NAME 50
#define FILENAME "appYangLagiJalan.txt"
#define FILENAME_KONFIG "appKonfigYangJalan.txt"

// Variabel global untuk menyimpan PID aplikasi yang sedang berjalan
pid_t running_apps[MAX_APPS];
int num_running_apps = 0;

// Fungsi untuk menambahkan PID aplikasi yang sedang berjalan
void addRunningApp(pid_t pid) {
    if (num_running_apps < MAX_APPS) {
        running_apps[num_running_apps++] = pid;
        printf("Num run apps: %d\n", num_running_apps);
    }
}

// Fungsi untuk membaca data dari file teks saat program dimulai
void manaYangJalan() {
    FILE *file = fopen(FILENAME, "r");
    if (file != NULL) {
        fscanf(file, "%d", &num_running_apps);
        for (int i = 0; i < num_running_apps; i++) {
            fscanf(file, "%d", &running_apps[i]);
        }
        fclose(file);
    }
}

// Fungsi untuk menyimpan data ke file teks saat program dihentikan
void simpanYangJalan() {
    FILE *file = fopen(FILENAME, "w");
    if (file != NULL) {
        fprintf(file, "%d\n", num_running_apps);
        for (int i = 0; i < num_running_apps; i++) {
            fprintf(file, "%d\n", running_apps[i]);
        }
        fclose(file);
    }
}

void simpanYangJalanKonfig() {
    FILE *file = fopen(FILENAME_KONFIG, "w");
    if (file != NULL) {
        fprintf(file, "%d\n", num_running_apps);
        for (int i = 0; i < num_running_apps; i++) {
            fprintf(file, "%d\n", running_apps[i]);
        }
        fclose(file);
    }
}

// Fungsi untuk membuka aplikasi
void bukaAplikasi(char *apps[], int jumlahApp[]) {
    int i, j;
    pid_t pid;

    for (i = 0; i < MAX_APPS && apps[i] != NULL; i++) {
        for (j = 0; j < jumlahApp[i]; j++) {
            pid = fork();

            if (pid == 0) {
                execlp(apps[i], apps[i], NULL);
                exit(0);
            } else {
                addRunningApp(pid); // Tambahkan PID aplikasi yang baru dimulai
            }
        }
    }
}

void bunuhDariKonfig() {
    FILE *file = fopen(FILENAME_KONFIG, "r");
    if (file != NULL) {
        int jumlahApp;
        fscanf(file, "%d", &jumlahApp);
        printf("%d", jumlahApp);
        for (int i = 0; i < jumlahApp; i++) {
            int pid;
            fscanf(file, "%d", &pid);
            printf(" = %d, ", pid);
            if (kill(pid, SIGTERM) == -1) {
                perror("Error killing process");
            }
        }
        fclose(file);
    }
}

// Fungsi untuk mematikan aplikasi sesuai dengan PID yang diberikan
void bunuhBiasa() {
    FILE *file = fopen(FILENAME, "r");
    if (file != NULL) {
        int jumlahApp;
        fscanf(file, "%d", &jumlahApp);
        printf("%d", jumlahApp);
        for (int i = 0; i < jumlahApp; i++) {
            int pid;
            fscanf(file, "%d", &pid);
            printf(" = %d, ", pid);
            if (kill(pid, SIGTERM) == -1) {
                perror("Error killing process");
            }
        }
        fclose(file);
    }
    FILE *file = fopen(FILENAME_KONFIG, "r");
    if (file != NULL) {
        int jumlahApp;
        fscanf(file, "%d", &jumlahApp);
        printf("%d", jumlahApp);
        for (int i = 0; i < jumlahApp; i++) {
            int pid;
            fscanf(file, "%d", &pid);
            printf(" = %d, ", pid);
            if (kill(pid, SIGTERM) == -1) {
                perror("Error killing process");
            }
        }
        fclose(file);
    }
}

// Fungsi untuk membaca file konfigurasi
void bacaKonfignya(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Cannot open file %s\n", filename);
        exit(1);
    }

    char line[MAX_APP_NAME];
    char *app;
    int num;

    // Parsing setiap baris dalam file konfigurasi
    while (fgets(line, sizeof(line), file)) {
        // Mengambil nama aplikasi dan jumlahnya
        app = strtok(line, " ");
        num = atoi(strtok(NULL, " \n"));

        // Menjalankan aplikasi sesuai dengan konfigurasi
        for (int i = 0; i < num; i++) {
            pid_t pid = fork();
            if (pid == 0) {
                execlp(app, app, NULL);
                exit(0);
            } else {
                addRunningApp(pid);
            }
        }
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    manaYangJalan();

    if (argc < 2) {
        printf("Usage: %s -o <app1> <num1> <app2> <num2> ... <appN> <numN> or %s -f <filename> or %s -k\n", argv[0], argv[0], argv[0]);
        exit(1);
    }

    if (strcmp(argv[1], "-o") == 0) {
        char *apps[MAX_APPS];
        int jumlahApp[MAX_APPS];

        int i, j, appIndex = 0;
        for (i = 2; i < argc; i += 2) {
            apps[appIndex] = argv[i];
            jumlahApp[appIndex] = atoi(argv[i + 1]);
            appIndex++;
        }
        apps[appIndex] = NULL;

        bukaAplikasi(apps, jumlahApp);

        simpanYangJalan(); 
    } else if (strcmp(argv[1], "-f") == 0) {
        // Membaca file konfigurasi
        if (argc != 3) {
            printf("Usage: %s -f <filename>\n", argv[0]);
            exit(1);
        }
        bacaKonfignya(argv[2]);

        simpanYangJalanKonfig(); 
    } else if (strcmp(argv[1], "-k") == 0) {
        if (argc == 3) {
            bunuhDariKonfig();
            remove(FILENAME_KONFIG);
        } else {
            bunuhBiasa();
            remove(FILENAME);
        }
    } else {
        printf("Invalid option\n");
        exit(1);
    }

    return 0;
}
