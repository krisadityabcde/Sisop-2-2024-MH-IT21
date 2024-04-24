# Sisop-2-2024-MH-IT21
## Anggota Kelompok:
- Rafael Ega Krisaditya	(5027231025)
- Rama Owarianto Putra Suharjito	(5027231049)
- Danar Bagus Rasendriya	(5027231055)

## Soal 1
### 1a
Program ini diminta untuk dapat menerima input path berupa ‘argv’ untuk mengatur folder dimana file akan dieksekusi
```c
int main(int argc, char *argv[]){
editfile(argv[1]);
}
```
Function `editfile()` adalah function utama yang akan digunakan dalam program ini dengan `argv[1]` nantinya akan berupa input folder path tempat program akan dieksekusi.

### 1b
Program tersebut berfungsi untuk mereplace string dengan ketentuan sebagai berikut:
1. String m4LwAr3 direplace dengan string [MALWARE]
2. String 5pYw4R3 direplace dengan string [SPYWARE]
3. String R4nS0mWaR3 direplace dengan string [RANSOMWARE]

```c
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
```
Awalnya function `editfile()` akan dijalankan secara rekursif setiap kali menemui subdirektori dan ketika menemui sebuah file, barulah isi konten dari file tersebut dimasukkan ke dalam variabel buffer yang nantinya akan di-pass ke function `editKonten()` untuk didekripsi.

```c
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
```
Pada function selanjutnya, satu-persatu karakter yang sudah dimasukkan ke dalam buffer akan didekripsi dengan cara menulis ulang semua karakter sebelum bagian string yang akan diganti, menulis bagian string yang sudah diganti, dan ditutup dengan bagian string setelah bagian string yang sudah diganti sebelumnya. Keseluruhan string yang sudah ditulis ulang kemudian akan dikembalikan ke fungsi awal untuk ditulis ulang ke file sebelumnya.

### 1c
Program harus berjalan secara daemon, dan tidak diperbolehkan menggunakan command `system()` dalam pembuatan program

```c
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
```
Untuk inisialisasi daemon, saya menggunakan template daemon yang disediakan pada modul.

Lalu dapat dilihat pada repo ini juga tidak ada command `system()` yang digunakan.

### 1d
Program akan secara terus menerus berjalan di background dengan jeda 15 detik

```c
  while (1){
  sleep(15);
  editfile(argv[1]);
  }
```
Dengan tambahan while loop program akan berjalan terus-menerus, dan dengan function `sleep(15)` perulangan akan dilakukan setiap interval 15 detik.

### 1e
Catat setiap penghapusan string yang dilakukan oleh program pada sebuah file bernama virus.log dengan format: [dd-mm-YYYY][HH:MM:SS] Suspicious string at <file_name> successfully replaced!

```c
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
```
Untuk membuat log saya menggunakan function tambahan `buatlog()` yang mana function ini akan diinisialisasi setiap kali ada perubahan pada file yang diinspeksi
```c
              if (strcmp(buffer, kontenUdahDiEdit) != 0) {
                    awal = fopen(file_path, "w");
                    fputs(kontenUdahDiEdit, awal);
                    fclose(awal);
                    buatlog(diredit, entry->d_name);
}
```

### Screenshot Hasil Pengerjaan
**Nanti**

### Kendala
Tidak ada kendala

### Revisi
Tidak ada revisi

## Soal 2
### 2a
Atasannya meminta agar program tersebut dapat berjalan secara daemon dan dapat mengunduh serta melakukan unzip terhadap file [berikut](https://drive.google.com/file/d/1rUIZmp10lXLtCIH3LAZJzRPeRks3Crup/view?usp=sharing). Atasannya juga meminta program ini dibuat tanpa menggunakan command system()

```c
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
```

Untuk download dan unzip file, saya membuat function tersendiri untuk setiap tindakan tersebut. Sebenarnya syntax yang digunakan untuk download dan unzip sama seperti biasanya yakni `wget()` dan `unzip()`. Namun karena semua hal ini dilakukan secara daemon, ada beberapa catatan tambahan yang harus diterapkan. Pertama, setiap function akan diinisalisasi sebagai child process menggunakan `fork()`. Kedua, karena akses untuk `stdout` dan `stderr` ditutup pada main function, maka diperlukan sebuah log tambahan untuk menampung `stdout` dan `stderr` yang harusnya dikeluarkan, karena jika tidak ada log yang menampung, syntax yang dipanggil tidak bisa berjalan.

### 2b
Setelah ditelusuri, ternyata hanya 6 file teratas yang nama filenya tidak dienkripsi. Oleh karena itu, bantulah Paul untuk melakukan dekripsi terhadap nama file ke-7 hingga terakhir menggunakan algoritma ROT19

```c
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
```

Function ini akan menelusuri setiap file di dalam direktori library dan akan mengecek setiap nama file yang tidak diawali oleh angka. Kemudian nama file akan didekripsi dan nama file akan di-rename menggunakan function `rename()`.

### 2c
Setelah dekripsi selesai, akan terlihat bahwa setiap file memuat salah satu dari kode berikut: r3N4mE, d3Let3, dan m0V3. Untuk setiap file dengan nama yang memuat kode d3Let3, hapus file tersebut. Sementara itu, untuk setiap file dengan nama yang memuat kode r3N4mE, lakukan hal berikut:
1. Jika ekstensi file adalah “.ts”, rename filenya menjadi “helper.ts”
2. Jika ekstensi file adalah “.py”, rename filenya menjadi “calculator.py”
3. Jika ekstensi file adalah “.go”, rename filenya menjadi “server.go”
4. Jika file tidak memuat salah satu dari ekstensi diatas, rename filenya menjadi “renamed.file”

```c
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
```
Dengan konsep yang sama, setiap file pada direktori library akan ditelusuri nama filenya yang yang mengandung kode **d3Let3** akan dihapus, sementara yang mengandung kode **r3N4mE** akan diubah nama filenya sesuai dengan ketentuannya. Sebagai catatan untuk **renamed.file** hanya akan berjumlah 1 di akhir karena setiap kali ada file yang mendapatkan nama tersebut, file sebelumnya akan di-overwrite dan terganti.  

### 2d
Atasan Paul juga meminta agar program ini dapat membackup dan merestore file. Oleh karena itu, bantulah Paul untuk membuat program ini menjadi 3 mode, yaitu:
1. default: program berjalan seperti biasa untuk me-rename dan menghapus file. Mode ini dieksekusi ketika program dijalankan tanpa argumen tambahan, yaitu dengan command ./management saja
2. backup: program memindahkan file dengan kode m0V3 ke sebuah folder bernama “backup”
3. restore: program mengembalikan file dengan kode m0V3 ke folder sebelum file tersebut dipindahkan

Contoh penggunaan: ./management -m backup

```c
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
```
Pada main function, mode dapat diubah setiap kali program dijalankan.

### 2e
Terkadang, Paul perlu mengganti mode dari program ini tanpa menghentikannya terlebih dahulu. Oleh karena itu, bantulan Paul untuk mengintegrasikan kemampuan untuk mengganti mode ini dengan mengirim sinyal ke daemon, dengan ketentuan:
1. SIGRTMIN untuk mode default
2. SIGUSR1 untuk mode backup
3. SIGUSR2 untuk mode restore

Contoh penggunaan: kill -SIGUSR2 <pid_program>

```c
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
```
Dengan menggunakan `kill`, signal tertentu dapat dikirimkan meskipunprogam berjalan di background. Tentunya PID dari program yang dijalankan harus didapatkan terlebih dahulu menggunakan `ps aux | grep management`.

### 2f
Program yang telah dibuat ini tidak mungkin akan dijalankan secara terus-menerus karena akan membebani sistem. Maka dari itu, bantulah Paul untuk membuat program ini dapat dimatikan dengan aman dan efisien

Dapat dilihat pada kode sebelumnya, ketika ada signal `SIGTERM` maka program akan dimatikan dengan aman.

### 2g
Terakhir, program ini harus berjalan setiap detik dan mampu mencatat setiap peristiwa yang terjadi ke dalam file .log yang bernama “history.log” dengan ketentuan:
1. Format: [nama_user][HH:MM:SS] - <nama_file> - <action>
2. nama_user adalah username yang melakukan action terhadap file
3. Format action untuk setiap kode:
   - kode r3N4mE: Successfully renamed.
   - kode d3Let3: Successfully deleted.
   - mode backup: Successfully moved to backup.
   - mode restore: Successfully restored from backup.
4. Contoh pesan log:
   - [paul][00:00:00] - r3N4mE.ts - Successfully renamed.
   - [paul][00:00:00] - m0V3.xk1 - Successfully restored from backup.

```c
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
```

Function ini adalah function untuk membuat file log. Dan untuk menjalankan program tiap detik saya menggunakan `sleep(1)` di dalam while loop pada main function.

### Screenshot Hasil Pengerjaan
**Nanti**

### Kendala
Karena waktu pengerjaan yang kurang, maka saya hanya bisa mengerjakan hingga function untuk melakukan backup saja, sementara mulai dari restore hingga akhir file belum sempat dikerjakan.

### Revisi
Asisten meminta untuk melengkapi kode hingga selesai.

## Soal 3
### 3a
Namai program tersebut dengan nama admin.c

Sudah dilakukan

### 3b
Program tersebut memiliki fitur menampilkan seluruh proses yang dilakukan oleh seorang user dengan menggunakan command: 

**./admin <user>**

### 3c
Program dapat memantau proses apa saja yang dilakukan oleh user. Fitur ini membuat program berjalan secara daemon dan berjalan terus menerus. Untuk menjalankan fitur ini menggunakan command: 

**./admin -m <user>**

Dan untuk mematikan fitur tersebut menggunakan: 

**./admin -s <user>**

Program akan mencatat seluruh proses yang dijalankan oleh user di file <user>.log dengan format:

**[dd:mm:yyyy]-[hh:mm:ss]_pid-process_nama-process_GAGAL/JALAN**

### 3d
Program dapat menggagalkan proses yang dijalankan user setiap detik secara terus menerus dengan menjalankan: 

**./admin -c user**

sehingga user tidak bisa menjalankan proses yang dia inginkan dengan baik. Fitur ini dapat dimatikan dengan command:

**./admin -a user**

### 3e
Ketika proses yang dijalankan user digagalkan, program juga akan melog dan menset log tersebut sebagai GAGAL. Dan jika di log menggunakan fitur poin c, log akan ditulis dengan JALAN

### Screenshot Hasil Pengerjaan
**Nanti**

### Kendala
Karena waktu pengerjaan yang kurang, awalnya saya hanya bisa mengerjakan poin soal b

### Revisi
Asisten meminta untuk melengkapi kode hingga selesai.

## Soal 4
### 4a
Salomo ingin dapat membuka berbagai macam aplikasi dengan banyak jendela aplikasi dalam satu command. Namai file program tersebut **setup.c**

### 4b
Program dapat membuka berbagai macam aplikasi dan banyak jendela aplikasi sekaligus (bisa membuka 1 atau lebih aplikasi dengan 1 atau lebih window (kecuali aplikasi yang tidak bisa dibuka dengan banyak window seperti discord)) dengan menjalankan: 

./setup -o <app1> <num1> <app2> <num2>...<appN> <numN>

Contoh penggunaannya adalah sebagai berikut: 

**./setup -o firefox 2 wireshark 2**

Program akan membuka 2 jendela aplikasi firefox dan 2 jendela aplikasi wireshark.

### 4c
Program juga dapat membuka aplikasi dengan menggunakan file konfigurasi dengan menggunakan 

**./setup -f file.conf**

Format file konfigurasi dibebaskan, namun pastikan dapat menjalankan fitur dari poin 2.
Contoh isi file.conf:

Firefox 2

Wireshark 3

Ketika menjalankan command contoh, program akan membuka 2 jendela aplikasi firefox dan 3 jendela aplikasi wireshark.

### 4d
Program dapat mematikan semua aplikasi yg dijalankan oleh program tersebut dengan: 

**./setup -k**

### 4e
Program juga dapat mematikan aplikasi yang dijalankan sesuai dengan file konfigurasi. 
Contohnya: 

**./setup -k file.conf**

Command ini hanya mematikan aplikasi yang dijalankan dengan 

**./setup -f file.conf**

### Screenshot Hasil Pengerjaan
**Nanti**

### Kendala
Karena waktu pengerjaan yang kurang, awalnya saya hanya bisa mengerjakan poin soal b, duraan itupun hanya bisa dilakukan tanpa otomatisasi. Maksudnya, untuk menginisialisasi sebuah window harus diubah konfigurasinya 

### Revisi
Asisten meminta untuk melengkapi kode hingga selesai.

