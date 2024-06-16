#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIPARIS_DOSYA_ADI "siparisler.txt"
#define YEMEKLISTESI_DOSYA_ADI "yemeklistesi.txt"

enum Y_DURUM {
    Y_MEVCUT_DEGIL,
    Y_MEVCUT,
};

enum S_DURUM {
    S_BEKLEMEDE,
    S_ONAYLANDI,
    S_HAZIR,
    S_REDDEDILDI,
};

typedef struct {
    char name[32];
    int fiyat;
    int sure;
    enum Y_DURUM durum;
} Yemek;

typedef struct {
    int id;
    char user[32];
    char yemek[32];
    time_t zaman;
    int sure;
    int fiyat;
    enum S_DURUM durum;
} Siparis;


static inline int get_yemek_line(FILE* yemekfile, Yemek* yemek) {
    return fscanf(yemekfile, "%[^,], %d, %d, %d\n", yemek->name, &yemek->fiyat, &yemek->sure, &yemek->durum);
}

static inline int get_siparis_line(FILE* siparis_file, Siparis* siparis) {
    return fscanf(siparis_file, 
            "%d, %[^,], %[^,], %ld, %d, %d, %d\n",
            &siparis->id, siparis->user, siparis->yemek, 
            &siparis->zaman, &siparis->sure, &siparis->fiyat,
            &siparis->durum);
}


void siparis_ver(char* kullaniciID){
    FILE* yemekfile = fopen(YEMEKLISTESI_DOSYA_ADI, "r");
    FILE* siparis_file = fopen(SIPARIS_DOSYA_ADI, "a+");

    Yemek yemek;

    printf("\nYemek Menusu:\n------------------\n");
    int i = 0;
    while (get_yemek_line(yemekfile, &yemek)==4) {
        if (yemek.durum == Y_MEVCUT) {
            i++;
            printf(
                "(%d) %s %dTL, %ddk\n",
                i, yemek.name, yemek.fiyat, yemek.sure
            );
        }
    }
    printf("(-1) Cikis\n");
    printf("Yemek Seciniz: ");
    int secim;
    scanf("%d", &secim);
    if (secim == -1) {
        printf("Islem iptal edildi.\n");
        return;
    }

    rewind(yemekfile);
    for (int j = 0; j < secim; j++) {
        get_yemek_line(yemekfile, &yemek);
        if (yemek.durum == Y_MEVCUT_DEGIL) {
            j--;
        }
    }


    fprintf(siparis_file,
            "%d, %s, %s, %ld, %d, %d, %d\n", 
            rand(), kullaniciID, yemek.name, time(NULL),
            yemek.sure, yemek.fiyat, 0);

    fclose(yemekfile);
    fclose(siparis_file);

    printf("%s siparisiniz verildi.\n", yemek.name);
}

void siparislerim(char* kullaniciID){
    FILE* siparis_file = fopen(SIPARIS_DOSYA_ADI, "r");

    Siparis siparis;

    printf("\nSiparislerim:\n");
    int siparis_sayisi = 0;
    while(get_siparis_line(siparis_file, &siparis) == 7){
        if (strcmp(kullaniciID, siparis.user) == 0){
            siparis_sayisi++;

            printf("%s - ", siparis.yemek);

            switch (siparis.durum) {
                case S_BEKLEMEDE:
                    printf("Onay Bekliyor.");
                    break;
                case S_ONAYLANDI:
                    printf("Onaylandi.");
                    break;
                case S_HAZIR:
                    printf("Hazir.");
                    break;
                case S_REDDEDILDI:
                    printf("Reddedildi.");
                    break;
                default:
                    printf("Invalid State");
                    break;
            }
            printf("\n");
        }
    }
    if (siparis_sayisi == 0) {
        printf("\tGun icinde verilen siparisiniz bulunmamaktadir.\n\tKullanici adinizi kontrol ediniz.\n");
    }

    fclose(siparis_file);
}

int main(){
    srand(time(NULL));

    char kullaniciID[32];
    printf("Kullanici Adiniz: ");
    scanf("%s", kullaniciID);
    printf("Hos geldiniz, %s.\n", kullaniciID);


    while(1){
        printf("\nAna Menu\n------------------\n(1)  Siparis Ver\n(2)  Siparislerim\n(-1) Cikis\nİslem Seciniz: ");
        int islem;
        scanf("%d", &islem);

        switch (islem) {
            case 1:
                siparis_ver(kullaniciID);
                break;
            case 2:
                siparislerim(kullaniciID);
                break;
            case -1:
                return 0;
                break;
        }
    }
}
