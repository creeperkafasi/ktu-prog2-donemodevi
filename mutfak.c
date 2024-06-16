/*
 * Deniz Tunç			434377
 * Emre Camuzoğlu		439553
 * Ramazan karabacak	439705
*/

#define PRINT_DEBUG (1)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>

#define QUEUE_MAX_LENGTH 128
#define SIPARIS_DOSYA_ADI "siparisler.txt"
#define KOPYA_DOSYA_ADI "siparisler_kopya_mutfak.txt"
#define SURE_CARP 1
#define ASCI_SAYISI 3
#define ASCI_SLEEP_OPTIMIZASYON 1

#if PRINT_DEBUG == 1
#define DEBUG(a) { a; fflush(stdout);}
#else
#define DEBUG(a)
#endif

// (Deniz): Windows ve Linux sleep fonksiyonlari farkli oldugu icin bunu yazmaya mecbur kaldim
#ifdef linux
#include <unistd.h>
#define SLEEP(seconds) sleep(seconds)
#elif _WIN32
#include <windows.h>
#define SLEEP(seconds) Sleep(seconds * 1000)
#endif

typedef struct {
    int id;
    int zaman;
    int sure;
} Siparis;

typedef struct {
    Siparis siparis;
    int bitis_zamani;
} Asci;

Asci ascilar[ASCI_SAYISI];

Siparis queue[QUEUE_MAX_LENGTH];
size_t queue_len = 0;

void pop_queue(size_t idx){
    queue_len--;
    for (int i = idx; i < queue_len; i++) {
        queue[i] = queue[i+1];
    }
}

void siparis_satir(FILE* siparisler_file, char* c, int* id, time_t* zaman, int* sure, int* fiyat, int* durum){
        fscanf(siparisler_file, "%d, %*[^,], %*[^,], %ld, %d, %d, %d\n", id, zaman, sure, fiyat, durum);
}

void siparisleri_oku(){
    fflush(stdout);
    FILE* siparisler_file = fopen(SIPARIS_DOSYA_ADI, "r");
    if (siparisler_file == NULL) {
        perror("DOSYA ACILAMADI");
        fflush(stderr);
    }

    char c;
    while ((c = getc(siparisler_file)) != EOF) {
        fseek(siparisler_file, -1, SEEK_CUR);

        int id;
        time_t zaman;
        int sure, fiyat, durum;
        siparis_satir(siparisler_file, &c, &id, &zaman, &sure, &fiyat, &durum);

        if (durum != 1) {
            continue;
        }

        int check = 1;
        for (int i = 0; i < queue_len; i++) {
            if (queue[i].id == id) {
                check = 0;
                break;
            }
        }
        for (int i = 0; i < ASCI_SAYISI; i++){
            if (ascilar[i].siparis.id == id){
                check = 0;
                break;
            }
        }
        if (!check)    continue;

        queue[queue_len].id = id;
        queue[queue_len].sure = sure;
        queue[queue_len].zaman = zaman;
        queue_len++;
    }
    fclose(siparisler_file);
}


int priority_job_index(time_t current_time){
    if (queue_len == 0) return -1;

    int job_index = 0;
    for (int j = 0; j < queue_len; j++) {
        Siparis j1 = queue[job_index];
        Siparis j2 = queue[j];
        int left1 = j1.zaman + j1.sure * SURE_CARP - current_time;
        int left2 = j2.zaman + j2.sure * SURE_CARP - current_time;

        if (left2 < left1) {
            job_index = j;
        }
    }
    return job_index;
}

void durumu_guncelle(int id, int durum){
    if (id == 0) return;


    FILE* siparisler_file = fopen(SIPARIS_DOSYA_ADI, "r");
    if (siparisler_file == NULL) {
        perror("HATA: Siparis dosyasi acilamadi");
        return;
    }

    FILE* copy_file = fopen(KOPYA_DOSYA_ADI, "w");
    if (copy_file == NULL){
        perror("HATA: Siparis kopya dosyasi acilamadi");
        return;
    }

    char ch;
    int cid, sure, fiyat, cdurum;
    long int zaman;
    char user[64];
    char dish[64];
    while (fgetc(siparisler_file) != EOF){
        fseek(siparisler_file, -1, SEEK_CUR);
        fscanf(siparisler_file, "%d, %[^,], %[^,], %ld, %d, %d, %d\n", &cid, user, dish, &zaman, &sure, &fiyat, &cdurum);

        if(cid == id) fprintf(copy_file, "%d, %s, %s, %ld, %d, %d, %d\n", id, user, dish, zaman, sure, fiyat, durum);
        else fprintf(copy_file, "%d, %s, %s, %ld, %d, %d, %d\n", cid, user, dish, zaman, sure, fiyat, cdurum);
    }
    fclose(siparisler_file);
    fclose(copy_file);

    copy_file = fopen(KOPYA_DOSYA_ADI, "r");

    siparisler_file = fopen(SIPARIS_DOSYA_ADI, "w");
    if(siparisler_file == NULL) {
        perror("DOSYA ACILAMADI");
    }
    while((ch = getc(copy_file)) != EOF ){
        fprintf(siparisler_file, "%c", ch);
    }

    fclose(copy_file);
    fclose(siparisler_file);

    remove(KOPYA_DOSYA_ADI);
}

int main(){
    // init
    for (int a = 0; a < ASCI_SAYISI; a++) {
        ascilar[a].bitis_zamani = 0;
    }

    for (int i = 1;;i++) {
        // queue guncelle
        DEBUG(printf("\n\nSiparis listesi taraniyor...\n"));
        siparisleri_oku();

        DEBUG(
            printf("Sirada %ld yemek var\n", queue_len);
            /*for(int y = 0; y < queue_len; y++)
                printf("queue[%d]: id%d\n", y, queue[y].id);*/
        )

        time_t current_time = time(NULL);

        // Asci atamasi
        int bosta_asci_var = 0;
        for (int a = 0; a < ASCI_SAYISI; a++) {
            Asci* asci = &ascilar[a];

            if (asci->siparis.id == 0){
                DEBUG(
                    printf("A%.2d - bosta\n", a));
                    }
            else{
                DEBUG(printf("A%.2d - id%d (%ldsn)\n", a, asci->siparis.id, asci->bitis_zamani - current_time));}
            
            if (asci->bitis_zamani <= current_time) {
                // Siparis durumunu guncelle
                durumu_guncelle(asci->siparis.id, 2);

                // Asciya yeni gorevini ver
                int idx = priority_job_index(current_time);
                if (idx == -1) {
                    bosta_asci_var = 1;
                    asci->bitis_zamani = 0;
                    asci->siparis.id = 0;
                    continue;
                }
                Siparis s = queue[idx];
                ascilar[a].siparis = s;
                ascilar[a].bitis_zamani = current_time + s.sure * SURE_CARP;

                pop_queue(idx);

                DEBUG(printf("\t-> Asci %d, queue[%d]==id%d %dsn aldi\n", a, idx, asci->siparis.id, s.sure * SURE_CARP));
            }
        }

        // Tum ascilar dolu ise uyku moduna gec
        if (!bosta_asci_var && ASCI_SLEEP_OPTIMIZASYON) {
            int ilk_bitiren = 0;
            for (int a = 0; a < ASCI_SAYISI; a++) {
                if (ascilar[a].bitis_zamani < ascilar[ilk_bitiren].bitis_zamani) {
                    ilk_bitiren = a;
                }
            }
            DEBUG(
                printf("Tum ascilar dolu, %ld saniye uykuya geciliyor!\n", (ascilar[ilk_bitiren].bitis_zamani - current_time));
                fflush(stdout);
            )
            SLEEP((ascilar[ilk_bitiren].bitis_zamani - current_time));
        }
        else SLEEP(1);
    }
}
