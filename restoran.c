#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#define SIZE 64
#define YEMEK_DOSYA_ADI "yemeklistesi.txt"
#define SIPARIS_DOSYA_ADI "siparisler.txt"
#define KOPYA_DOSYA "kopya_restoran.txt"
#define TOP_RANK_SIZE 5

char c;
char kelime_tutucu[SIZE];
int l, counter;

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
  int id;
  char user[SIZE];
  char sip_yemekadi[SIZE];
  time_t zaman;
  int sure;
  int fiyat;
  enum S_DURUM durum;  
} Siparis;

typedef struct {
    char ismi[SIZE];
    int fiyati;
    int suresi;
    enum Y_DURUM durumu;
} Yemek;

void yemek_listesi_oku(FILE* y){
    rewind(y);
    printf("\n");
    int line1 = 1;
    Yemek yemekbilgi;
    while(fscanf(y, "%[^,], %d, %d, %d\n", yemekbilgi.ismi, 
        &yemekbilgi.fiyati, &yemekbilgi.suresi, &yemekbilgi.durumu) > 0){

        if (yemekbilgi.durumu == Y_MEVCUT){
            printf("(%d) %s %dTl %dDk mevcut\n", 
            line1, yemekbilgi.ismi, yemekbilgi.fiyati, yemekbilgi.suresi);
        }

        if(yemekbilgi.durumu == Y_MEVCUT_DEGIL){
            printf("(%d) %s %dTl %dDk mevcut degil\n", 
            line1, yemekbilgi.ismi, yemekbilgi.fiyati, yemekbilgi.suresi);

        }
        line1++;
        if(c == '\n')
            continue;
    }
    printf("\n\n");
}

void siparis_oku(FILE* fps){
    rewind(fps);
    printf("\n");
    Siparis sip_bilgi;
    int line2 = 1;
    while((c = fgetc(fps)) != EOF){    
        fseek(fps, -1, SEEK_CUR);
        fscanf(fps, "%d, %[^,], %[^,], %*ld, %*d, %d, %d\n", &sip_bilgi.id, sip_bilgi.user, 
        sip_bilgi.sip_yemekadi, &sip_bilgi.fiyat, &sip_bilgi.durum);

        switch (sip_bilgi.durum)
        {
        case S_BEKLEMEDE:
            printf("(%d) id : %d  %s | %s %dTl onay bekliyor...\n", 
            line2, sip_bilgi.id, sip_bilgi.user, sip_bilgi.sip_yemekadi, sip_bilgi.fiyat);
            break;
        case S_ONAYLANDI:
            printf("(%d) id : %d  %s | %s %dTl onaylandi!.\n", 
            line2, sip_bilgi.id, sip_bilgi.user, sip_bilgi.sip_yemekadi, sip_bilgi.fiyat);
            break;
        case S_HAZIR:
            printf("(%d) id : %d  %s | %s %dTl hazir!\n", 
            line2, sip_bilgi.id, sip_bilgi.user, sip_bilgi.sip_yemekadi, sip_bilgi.fiyat);
            break;
        case S_REDDEDILDI:
            printf("(%d) id : %d  %s | %s %dTl reddedildi.\n", 
            line2, sip_bilgi.id, sip_bilgi.user, sip_bilgi.sip_yemekadi, sip_bilgi.fiyat);
            break;
        }
        line2++;

    }

    printf("\n\n");
}
//yemek duzenleme
int bilgileri_duzenle(int x){
    int r;
    scanf("%d", &r);
    //-1 degılse girilen degeri dondur
    if(r != -1)
        return r;
    return x;
}

int kazanc(FILE* kazanc_oku){
    l = 0;
    counter = 0;
    Siparis siparis_struct;
    while((c = fgetc(kazanc_oku)) != EOF){//değişti (emre)
        fseek(kazanc_oku, -1, SEEK_CUR);

        fscanf(kazanc_oku, "%*d, %*[^,], %*[^,], %*ld, %*d, %d, %d\n", 
        &siparis_struct.fiyat, &siparis_struct.durum);
        //hazır olan siparisler
        if(siparis_struct.durum == 2){  
            l += siparis_struct.fiyat;
        }
        counter = 0;
        continue;
    }
    return l;
}

void guncelleme(){
    int s;
    s = -1;
    printf("\nGuncelleme\n");
    printf("---------------------------\n\n(1)Yemek Ekleme\n(2)Yemek Silme\n(3)Yemek Bilgisi Duzenleme\n(-1)Cikis\n\nYapmak istediginiz islem icin uygun sayi giriniz : ");
    scanf("%d", &s);

    switch(s){
    case 1: ;
        FILE* yemeklistesi_ekle = fopen(YEMEK_DOSYA_ADI, "a+");
        if(yemeklistesi_ekle == NULL){
            perror("Dosya acilamadi");
            return;
        }

        yemek_listesi_oku(yemeklistesi_ekle);
        rewind(yemeklistesi_ekle);

        Yemek yemek;
        printf("Eklenecek yemek adi giriniz : ");
        scanf("\n%[^\n]", yemek.ismi);

        printf("Eklenecek yemek fiyati girin : ");
        scanf("%d", &yemek.fiyati);

        printf("Eklenecek yemek hazirlanma suresi girin : ");
        scanf("%d", &yemek.suresi);

        printf("Eklenecek yemek durumu girin (0 veya 1 | 0 = Mevcut degil, 1 = Mevcut):");
        scanf("%d", &yemek.durumu);

        fprintf(yemeklistesi_ekle, "%s, %d, %d, %d\n", 
			yemek.ismi, yemek.fiyati, yemek.suresi, yemek.durumu);

        rewind(yemeklistesi_ekle);
        printf("\nYemek Listesi :\n");
        yemek_listesi_oku(yemeklistesi_ekle);

        fclose(yemeklistesi_ekle);

        break;
    case 2: ;
        FILE* yemeklistesi_sil = fopen(YEMEK_DOSYA_ADI, "r");
        if (yemeklistesi_sil == NULL) {
            perror("Dosya acilamadi");
            return;
        }
        FILE* yemeklistesi_sil_kopya = fopen(KOPYA_DOSYA, "w+");
        if (yemeklistesi_sil_kopya == NULL) {
            fclose(yemeklistesi_sil);
            perror("Dosya acilamadi");
            return;
        }

        l = 0;

        while(fgets(kelime_tutucu, 64, yemeklistesi_sil)){
            fprintf(yemeklistesi_sil_kopya, "%s", kelime_tutucu);
            l++;
        }

        //dosya kopyalanmadıysa
        if(l == 0){
            printf("Bu liste bos!!!\n");
            fclose(yemeklistesi_sil_kopya);
            remove(KOPYA_DOSYA);
            fclose(yemeklistesi_sil);
            break;
        }

        fclose(yemeklistesi_sil);

        printf("\nYemek Listesi :\n");
        yemek_listesi_oku(yemeklistesi_sil_kopya);

        printf("Silmek istediginiz yemegin bulundugu satiri giriniz : ");
        scanf("%d", &l);

        FILE* yemeklistesi_yeni;
        yemeklistesi_yeni = fopen(YEMEK_DOSYA_ADI, "w+");

        rewind(yemeklistesi_sil_kopya);

        kelime_tutucu[0] = 0;
        counter = 0;
        int found = 0;
        //secilen satırı atlar
        while(fgets(kelime_tutucu, 64, yemeklistesi_sil_kopya)){
            counter++;
            if(l == counter) continue;
            fprintf(yemeklistesi_yeni, "%s", kelime_tutucu);
        }

        printf("\nYemek Listesi :\n");
        yemek_listesi_oku(yemeklistesi_yeni);

        fclose(yemeklistesi_sil_kopya);
        remove(KOPYA_DOSYA);
        fclose(yemeklistesi_yeni);
        kelime_tutucu[0] = 0;
        break;
    case 3: 
        printf("Yemek Listesi: ");

        FILE* yemeklistesi_duzenle = fopen(YEMEK_DOSYA_ADI, "r");
        if(yemeklistesi_duzenle == NULL){
            perror("Dosya acilamadi ");
            return;
        }
        FILE* yemeklistesi_duzenle_kopya = fopen(KOPYA_DOSYA, "w+");
        if(yemeklistesi_duzenle_kopya == NULL){
            perror("Dosya acilamadi ");
            return;
        }

        yemek_listesi_oku(yemeklistesi_duzenle);
        rewind(yemeklistesi_duzenle);

        Yemek duzenle;
        printf("Duzenlemek istediginiz yemegin bulundugu satiri giriniz (Cikis icin -1):");
        scanf("%d", &l);
        if (l == -1) {
            printf("Isleminiz iplat edildi.\n");
            fclose(yemeklistesi_duzenle);
            fclose(yemeklistesi_duzenle_kopya);
            remove(KOPYA_DOSYA);
            return;
        }

        int sayac = 1;
        kelime_tutucu[0] = 0;
        //kopyalama, yemek bilgisi tutma (değişti : emre)[fonskiyon yap]
        while ((c = fgetc(yemeklistesi_duzenle)) != EOF){
            fseek(yemeklistesi_duzenle, -1, SEEK_CUR);
            //duzenlencek satir
            if(sayac == l){
                fscanf(yemeklistesi_duzenle, "%[^,], %d, %d, %d\n", 
                    duzenle.ismi, &duzenle.fiyati, &duzenle.suresi,
                    &duzenle.durumu);

                printf("Secilen yemek : %s\n", duzenle.ismi);
                printf("Yeni yemek ismi giriniz (duzenlememek icin (0)): ");
                scanf("\n%[^\n]", kelime_tutucu);

                //duzenlenmeicekse
                if(strcmp(kelime_tutucu, "0") != 0){
                    sprintf(duzenle.ismi, "%s", kelime_tutucu);
                }

                printf("Yeni fiyat bilgisi giriniz(duzenlememek icin (-1)): ");
                duzenle.fiyati = bilgileri_duzenle(duzenle.fiyati);

                printf("Yeni sure bilgisi giriniz(duzenlememek icin (-1)):");
                duzenle.suresi = bilgileri_duzenle(duzenle.suresi);

                printf("Yeni durum bilgisi giriniz(duzenlememek icin (-1)):");
                duzenle.durumu = bilgileri_duzenle(duzenle.durumu);

                printf("%s, %d, %d, %d\n", duzenle.ismi, duzenle.fiyati, 
                    duzenle.suresi, duzenle.durumu);
                fprintf(yemeklistesi_duzenle_kopya, "%s, %d, %d,%d\n",
                        duzenle.ismi, duzenle.fiyati,
                        duzenle.suresi, duzenle.durumu);
                sayac++;
                continue;
            }
            //kopyalama
            fgets(kelime_tutucu, 64, yemeklistesi_duzenle);
            fprintf(yemeklistesi_duzenle_kopya, "%s", kelime_tutucu);
            sayac++;
        }

        rewind(yemeklistesi_duzenle);

        fclose(yemeklistesi_duzenle);

        yemeklistesi_duzenle = fopen(YEMEK_DOSYA_ADI, "w+");
        rewind(yemeklistesi_duzenle_kopya);

        while(fgets(kelime_tutucu, 64, yemeklistesi_duzenle_kopya)){
            fprintf(yemeklistesi_duzenle, "%s", kelime_tutucu);
        }


        rewind(yemeklistesi_duzenle);
        yemek_listesi_oku(yemeklistesi_duzenle);
        fclose(yemeklistesi_duzenle);
        fclose(yemeklistesi_duzenle_kopya);
        remove(KOPYA_DOSYA);
        break;
    default:
        if(s != -1)
            printf("Uygun bir sayi giriniz.\n");
    }
}

void onay(){
    printf("\nSiparis Onay/Ret\n---------------------------\n");

    FILE* siparis_dosya = fopen(SIPARIS_DOSYA_ADI, "r");
    FILE* kopya_dosya = fopen(KOPYA_DOSYA, "w+");


    int id, sure, fiyat, durum;
    char yemek[32], kullanici[32];
    time_t zaman;

    // Siparisleri yazdır

    int lines_read;
    for (lines_read = 0;
        fscanf(siparis_dosya, "%d, %[^,], %[^,], %ld, %d, %d, %d\n", 
            &id, kullanici, yemek, &zaman, &sure, &fiyat, &durum) != -1;)
    {
        if (durum == 0) {
            printf("(%d) - %010d %s %s \n",
                    lines_read + 1, id, kullanici, yemek);
            lines_read++;
        }
    }

    if(lines_read == 0){
        printf("Onaylanmasi beklenen siparis bulunmamaktadir.\n");
        fclose(kopya_dosya);
        remove(KOPYA_DOSYA);
        fclose(siparis_dosya);
        return;
    }

    // Degistirilecek satiri sec
    int change_line;
    printf("Satir giriniz: ");
    scanf("%d", &change_line);

    // Satirlari kopya dosyaya aktar
    int counter = 0;
    rewind(siparis_dosya);
    int done = 0;
    while (fscanf(siparis_dosya, "%d, %[^,], %[^,], %ld, %d, %d, %d\n", &id, kullanici, yemek, &zaman, &sure, &fiyat, &durum) > 0){

        // Eger degistirilecek satiri daha yazdirmadiysak
        if(!done){
            if (durum == 0) counter++;
            if (counter == change_line) {
                // Yeni durumu kullanicidan iste
                done = 1;
                printf("Onay (1), Ret (-1) %d: ", id);
                scanf("%d", &durum);
                if (durum == -1) durum = S_REDDEDILDI;
            }
        }

        fprintf(kopya_dosya, "%d, %s, %s, %ld, %d, %d, %d\n",
            id, kullanici, yemek, zaman, sure, fiyat, durum);
    }

    // Kopya dosyayi siparis dosyasina aktar
    fclose(kopya_dosya);
    kopya_dosya = fopen(KOPYA_DOSYA, "r");
    fclose(siparis_dosya);
    siparis_dosya = fopen(SIPARIS_DOSYA_ADI, "w");

    char line_buffer[100];
    while (fscanf(kopya_dosya, "%[^\n]\n", line_buffer) > 0){
        fprintf(siparis_dosya, "%s\n", line_buffer);
    }

    fclose(kopya_dosya);
    remove(KOPYA_DOSYA);
    fclose(siparis_dosya);

    printf("Siparis durumu basariyla guncellendi.\n\n");
}

void rapor(){
    time_t t = time(NULL);
    struct tm date_ = *localtime(&t);
    int j = -1;
    printf("\nRapor Islemleri\n---------------------------\n\n");
    printf("(1)Gunluk rapor cikarma\n(2)Rapor goruntuleme\n(-1)Cikis\n\nUygun bir sayi giriniz : ");
    scanf("%d", &j);

    switch(j){
    case -1:
        break;
    case 1:
        kelime_tutucu[0] = 0;
        char dosya_adi[SIZE];
        dosya_adi[0] = 0;

        //rapor olusturulması istenen tarihi alır
        sprintf(dosya_adi, "arsiv/%02d.%02d.%02d.txt", date_.tm_mday, date_.tm_mon + 1, (date_.tm_year + 1900) % 2000);
        FILE* dosya_ac, * siparis_cek, * siparis_cek_kopya;
        dosya_ac = fopen(dosya_adi, "w");
        fclose(dosya_ac);

        siparis_cek = fopen(SIPARIS_DOSYA_ADI, "r");
        siparis_cek_kopya = fopen(KOPYA_DOSYA, "w+");
        dosya_ac = fopen(dosya_adi, "a");
        if(siparis_cek_kopya == NULL){
            printf("acılmadı\n");
            break;
        }

        kelime_tutucu[0] = 0;
        counter = 0;
        Siparis siparis;
        //rapor olusturur
        while((c = fgetc(siparis_cek)) != EOF){//değişti : emre
            fseek(siparis_cek, -1, SEEK_CUR);

            fscanf(siparis_cek, "%d, %[^,], %[^,], %ld, %d, %d, %d\n", &siparis.id, siparis.user, siparis.sip_yemekadi, &siparis.zaman, &siparis.sure, &siparis.fiyat, &siparis.durum);

            FILE* dest;
            //1 ve 0lar siparise
            if((siparis.durum == S_BEKLEMEDE) || (siparis.durum == S_ONAYLANDI)){
                dest = siparis_cek_kopya;
            }

            //2 ve 3ler rapora
            if((siparis.durum == S_HAZIR) || (siparis.durum == S_REDDEDILDI)){
                dest = dosya_ac;
            }
            fprintf(dest, "%d, %s, %s, %ld, %d, %d, %d\n", siparis.id, siparis.user, siparis.sip_yemekadi, siparis.zaman, siparis.sure, siparis.fiyat, siparis.durum);

        }
        fclose(siparis_cek);


        siparis_cek = fopen(SIPARIS_DOSYA_ADI, "w");
        rewind(siparis_cek_kopya);

        kelime_tutucu[0] = 0;
        //kopyadan orjinal siparis dosyasına atar
        while(fgets(kelime_tutucu, 64, siparis_cek_kopya)){
            fprintf(siparis_cek, "%s", kelime_tutucu);
        }

        fclose(siparis_cek);
        fclose(siparis_cek_kopya);
        fclose(dosya_ac);
        remove(KOPYA_DOSYA);

        printf("\nBugunun raporu basariyla cikartildi.\n\n");
        break;
    case 2://(!)
        printf("Goruntulemek istediginiz raporu gg.aa.yy seklinde giriniz : ");
        int tarih[3];
        char dosya_ismi[32];
        scanf("%d.%d.%d", &tarih[0], &tarih[1], &tarih[2]);
        sprintf(dosya_ismi, "arsiv/%02d.%02d.%02d.txt", tarih[0], tarih[1], tarih[2]);


        FILE* dosya = fopen(dosya_ismi, "r");
        if (dosya == NULL){
            perror("Dosya acilamadi");
            return;
        }

        siparis_oku(dosya);
        fclose(dosya);
        break;
    default:
        printf("Uygun sayi giriniz.");
        break;
    }
}

// (Deniz)
// Burada siralanan structlar, analiz fonksiyonunun
// en cok tuketilenler bolumunde acilan structlar olmalidir.
// Turlerden bagimsiz sirlama fonksiyonu yapabilmek ve
// offsetlerle ugrasmamak adina kontrol edilen her
// elemanin ilk sizeof(int) kadarki byte'ini siralanan deger
// olarak aliyorum
// Bu fonksiyonu aciklamasi gerekecek olan arkadaslarima
// sabir ve pointer bilgisi diliyorum :)
void bubble_sort(void* array, size_t len, size_t struct_size){
    for (int i = 0; i < len; i++) {
        void* indexi = array + (struct_size * i);
        for (int j = i + 1; j < len; j++) {
            void* indexj = array + (struct_size * j);
            int controli, controlj;
            memcpy(&controli, indexi, sizeof(int));
            memcpy(&controlj, indexj, sizeof(int));
            if(controli  < controlj){
                void* temp = malloc(struct_size);
                memcpy(temp, indexi, struct_size); // temp = i
                memcpy(indexi, indexj, struct_size); // i = j
                memcpy(indexj, temp, struct_size); // j = temp
                free(temp);
            }
        }
    }
}

void analiz(){
    int i = -1;
    printf("\nAnaliz Islemleri\n---------------------------\n");
    printf("\n(1)Gunluk kazanc\n(2)Aylik kazanc\n(3)Donemsel kazanc\n(4)En cok tuketim\n(-1)Cikis\n\nUygun bir sayi giriniz : ");
    scanf("%d", &i);
    char dosya_ismi[32];
    int gun, ay, yil;

    switch(i){
    case -1:
        break;
    case 1:
        printf("Gunluk analiz yapilmasini istediginiz tarihi giriniz (gg.aa.yy) : ");
        scanf("%d.%d.%d", &gun, &ay, &yil);
        sprintf(dosya_ismi, "arsiv/%02d.%02d.%02d.txt", gun, ay, yil);

        FILE* dosya;
        dosya = fopen(dosya_ismi, "r");

        int gunluk_kazanc = kazanc(dosya);

        //siparis_oku(dosya);
        printf("\n\nSecili gunun kazanci : %d TL\n\n", gunluk_kazanc);
        fclose(dosya);
        break;
    case 2:
        printf("Analiz yapilmasi istenen ayi giriniz (aa.yy): ");
        scanf("%d.%d", &ay, &yil);

        if((ay < 1) || (ay > 12)){
            printf("Uygun bir sayi giriniz.\n");
            return;
        }

        int aylik_kazanc = 0;
        for(int gun = 1; gun <= 31; gun++){
            sprintf(dosya_ismi, "arsiv/%02d.%02d.%02d.txt", gun, ay, yil);
            FILE* dosya = fopen(dosya_ismi, "r");
            if(dosya == NULL){
                continue;
            }
            aylik_kazanc += kazanc(dosya);
            fclose(dosya);
        }

        printf("\nToplam aylik kazanc : %d TL\n", aylik_kazanc);

        break;
    case 3:
        printf("Analiz yapilmasini istediginiz tarihi araligini giriniz (gg.aa.yy - gg.aa.yy):");
        int g1, a1, y1, g2, a2, y2;
        scanf("%d.%d.%d - %d.%d.%d", &g1, &a1, &y1, &g2, &a2, &y2);

        for (int yil = y1; yil <= y2; yil++){
            int a_start = (yil == y1) ? a1 : 1;
            int a_end = (yil == y2) ? a2 : 12;

            for (int ay = a_start; ay <= a_end; ay++) {
                int g_start = ((ay == a1) && (yil == y1)) ? g1 : 1;
                int g_end = ((ay == a2) && (yil == y2)) ? g2 : 31;

                for (int gun = g_start; gun <= g_end; gun++) {
                    sprintf(dosya_ismi, "arsiv/%02d.%02d.%02d.txt", gun, ay, yil);
                    printf("%s\n", dosya_ismi);
                    FILE* dosya = fopen(dosya_ismi, "r");
                    if(dosya == NULL){
                        continue;
                    }
                    aylik_kazanc += kazanc(dosya);
                    fclose(dosya);
                }
            }
        }
        printf("\n%d.%d.%d - %d.%d.%d arasi toplam kazanc : %d TL\n",g1, a1, y1, g2, a2, y2, aylik_kazanc);
        break;
    case 4:
        printf("\nTum zamanlarin siparis lider tablosu\n-------------------\n");
        typedef struct {
            int count;
            char username[32];
        } user;
        user* users = calloc(1024, sizeof(user));
        int users_len = 0;

        typedef struct {
            int count;
            char name[32];
        } yemek;
        yemek* yemekler = calloc(128, sizeof(yemek));
        int yemekler_len = 0;

        long int toplam_hazirlama_suresi;

        typedef struct {
            int count;
            int tarih[3];
        } gun;
        gun* gunler = calloc(512, sizeof(gun));
        int gunler_len = 0;

        Siparis sip;

        DIR *arsiv_dir = opendir("arsiv");
        if(arsiv_dir == NULL){
            perror("Klasor acilamadi");
            return;
        }
        struct dirent *dent;
        while((dent = readdir(arsiv_dir)) != NULL){
            if (dent->d_type != DT_REG) {
                continue;
            }

            //printf("%s açiliyor\n", dent->d_name);
            sprintf(dosya_ismi, "arsiv/%s", dent->d_name);
            FILE* dosya = fopen(dosya_ismi, "r");
            if (dosya == NULL) {
                printf("%s", dosya_ismi);
                perror("Dosya acilamadi");
                continue;
            }

            while(  fscanf(dosya,
                    "%d, %[^,], %[^,], %ld, %d, %d, %d\n",
                    &sip.id, sip.user, sip.sip_yemekadi, &sip.zaman,
                    &sip.sure, &sip.fiyat, &sip.durum) > 0  )
            {
                // Reddedilen siparisleri atla
                if (sip.durum == 3) {
                    continue;
                }
                // Kullanici siparis sayisi
                int bulundu = 0;
                for (int i = 0; i < users_len; i++) {
                    if (strcmp(users[i].username, sip.user) == 0) {
                        users[i].count++;
                        bulundu = 1;
                        break;
                    }
                }
                if (!bulundu) {
                    strcpy(users[users_len].username, sip.user);
                    users[users_len].count = 1;
                    users_len++;
                }

                // Yemek siparis sayisi
                bulundu = 0;
                for (int i = 0; i < yemekler_len; i++) {
                    if (strcmp(yemekler[i].name, sip.sip_yemekadi) == 0) {
                        yemekler[i].count++;
                        bulundu = 1;
                        break;
                    }
                }
                if (!bulundu) {
                    strcpy(yemekler[yemekler_len].name, sip.sip_yemekadi);
                    yemekler[yemekler_len].count = 1;
                    yemekler_len++;
                }

                // Gunluk siparis sayisi
                int gun, ay, yil;
                sscanf(dent->d_name, "%d.%d.%d", &gun, &ay, &yil);
                bulundu = 0;
                for (int i = 0; i < gunler_len; i++) {
                    int *tarih = gunler[i].tarih;
                    if ((gun == tarih[0]) &&
                        (ay == tarih[1]) &&
                        (yil == tarih[2]))
                    {
                        gunler[i].count++;
                        bulundu = 1;
                        break;
                    }
                }
                if (!bulundu) {
                    int *tarih = gunler[gunler_len].tarih;
                    gunler[gunler_len].tarih[0] = gun;
                    gunler[gunler_len].tarih[1] = ay;
                    gunler[gunler_len].tarih[2] = yil;
                    gunler[gunler_len].count = 1;
                    gunler_len++;
                }

                // Toplam hazirlanma suresi
                toplam_hazirlama_suresi += sip.sure;
            }
            fclose(dosya);
        }

        bubble_sort(yemekler, yemekler_len, sizeof(yemek));
        bubble_sort(users, users_len, sizeof(user));
        bubble_sort(gunler, gunler_len, sizeof(gun));

        printf("\n-> En cok siparis veren kullanicilar:\n");
        for (int i = 0; i < TOP_RANK_SIZE; i++) {
            if (i >= users_len) {
                printf("%d#                 -\n", i + 1);
            }
            else printf("%d# %4d siparis ile - %s\n",
                    i + 1,
                    users[i].count,
                    users[i].username);
        }
        printf("\n-> En cok siparis verilen yemekler:\n");
        for (int i = 0; i < TOP_RANK_SIZE; i++) {
            if (i >= yemekler_len) {
                printf("%d#                 -\n", i + 1);
            }
            else printf("%d# %4d siparis ile - %s\n",
                    i + 1,
                    yemekler[i].count,
                    yemekler[i].name);
        }
        printf("\n-> En cok siparis verilen gunler:\n");
        for (int i = 0; i < TOP_RANK_SIZE; i++) {
            int* tarih = gunler[i].tarih;
            if (i >= gunler_len) {
                printf("%d#                 -\n", i + 1);
            }
            else printf("%d# %4d siparis ile - %d.%d.%d\n",
                    i + 1,
                    gunler[i].count,
                    tarih[0], tarih[1], tarih[2]);
        }
        printf("\n-> Ascilarin toplam calisma suresi: %lddk",
                toplam_hazirlama_suresi);
        printf("\n\n");
        break;
    default:
        printf("Uygun bir sayi giriniz.\n");
        break;
    }
}

int main()
{
    int secim;
    do{
        secim = -1;
        printf("Ana Menu\n");
        printf("---------------------------\n\n(1)Yemek Listesi Guncelleme\n(2)Siparis Onay Ret\n(3)Rapor Islemleri\n(4)Analiz Islemleri\n(-1)Cikis\n\nBir sayi giriniz: ");
        scanf("%d", &secim);

        switch(secim){
        case 1:
            guncelleme();
            break;
        case 2:
            onay();
            break;
        case 3:
            rapor();
            break;
        case 4:
            analiz();
            break;
        case -1:
            break;
        default:
            printf("Uygun sayi giriniz!!\n");
        }
    }while(secim != -1);
return 0;
}
