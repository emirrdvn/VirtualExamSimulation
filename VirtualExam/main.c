#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// SINAV URETECI
void cevap_anahtari_uret(char cevap_anahtari[ ], int S)
{
    srand(time(NULL));
    cevap_anahtari[S];
    char siklar[5]={'A','B','C','D','E'};
    for(int i=0;i<S;i++)
        cevap_anahtari[i]=siklar[rand()%5]; //cevap_anahtari dizisine rastgele sik atandý
}
void cevap_anahtari_yazdir(char cevap_anahtari[ ], int S)
{
    printf("\nCEVAP ANAHTARI\n");
    printf("_____________________________________________________________________________\n\n");
    for(int i=0;i<S;i++)
        printf("%03d : %c | ",i+1,cevap_anahtari[i]);
    printf("\n_____________________________________________________________________________\n");
}
void sinavi_uygula(char ogrenci_cevaplari[ ][100], char cevap_anahtari[ ], int N, int S, double B, double D)
{
    double bos_ihtimal,dogru_ihtimal;
    char siklar[6]={'A','B','C','D','E'};
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<S;j++)
        {
            bos_ihtimal = (double)rand()/RAND_MAX;
            if(bos_ihtimal<B)       //BOS SIKKIN ATANMASI
            ogrenci_cevaplari[i][j]='X';
            else
            {
                dogru_ihtimal = (double)rand()/RAND_MAX;
                if(dogru_ihtimal<=D)         // DOGRU SIKKIN ATANMASI
                    ogrenci_cevaplari[i][j]=cevap_anahtari[j];
                else    //YANLIS SIKKIN ATANMASI
                {
                    int poz;
                    char siklar2[5]={'A','B','C','D','E'};
                    for(int i=0;i<5;i++)
                    {
                        if(siklar[i]==cevap_anahtari[j])
                            poz=i;  //Dogru sikkin pozisyonunu belirledik
                    }
                    for(int i=poz;i<5;i++)
                        siklar2[i]=siklar2[i+1]; //Dogru sikki sildik
                    int y=rand()%4;
                    ogrenci_cevaplari[i][j]=siklar2[y]; //Dogru olmayan rastgele sik atandi
                }
            }
        }
    }
}
void ogrenci_cevabini_yazdir(char ogrenci_cevaplari[ ][100], int ogrenci_ID, int S)
{
    for(int i=0;i<ogrenci_ID;i++)
    {
        printf("%03d id'li ogrencinin cevaplari:\n\n",i+1);
        for(int j=0;j<S;j++)
            printf("%03d : %c | ",j+1,ogrenci_cevaplari[i][j]);
        printf("\n-----------------------------------------------------------------------------\n");
    }
}
void ogrencileri_puanla(char ogrenci_cevaplari[ ][100], char cevap_anahtari[ ], double HBN[ ], int N, int S)
{
    for(int i=0;i<N;i++)
    {
        double sonuc,dogru=0.0,yanlis=0.0;
        for(int j=0;j<S;j++)
        {
            if(ogrenci_cevaplari[i][j]==cevap_anahtari[j])
                dogru++;
            else if(ogrenci_cevaplari[i][j]=='X')
                ;
            else
                yanlis++;
        }
        sonuc=(100.0/S)*dogru-(100.0/S)*(yanlis/4.0);
        if(sonuc<0)
            HBN[i]=0;
        else
            HBN[i]=sonuc;
    }
}
//  SINAV DERLEYICI
double sinif_ortalamasi_hesapla(double HBN[ ], int N)
{
    double toplam=0;
    for(int i=0;i<N;i++)
        toplam+= HBN[i];
    return toplam/N;
}
double standart_sapma_hesapla(double ortalama, double HBN[ ], int N)
{
    double sonuc;
    for(int i=0;i<N;i++)
        sonuc+=pow((HBN[i]-ortalama),2);
    sonuc=sqrt(sonuc/N);
    return sonuc;
}
void T_skoru_hesapla(double ortalama, double HBN[ ], int N, double std, double T_skoru[ ])
{
    for(int i=0;i<N;i++)
    {
        if(std==0.0)
            T_skoru[i]=60;
        else
            T_skoru[i]= 60 + 10*((HBN[i]-ortalama)/std);
    }
}
int sinif_duzeyi_hesapla(double T_skoru[ ],double ortalama)
{
    char *basari_durumlari[8]={"Ustun basari","Mukemmel","Cok Iyi","Iyi","Ortanin ustu","Orta","Zayif","Kotu"};
    double duzen[8]={20,10,7.5,5,5,5,5}; // Agirlikli basari puani ortalamalari arasindaki puan farklari (100-80),(80-70),(70-62.5)...
    double y=0.0;
    int sinif_indis=0;
    for(int i=0;i<7;i++)
    {
        if(ortalama>(100-duzen[i]-y)&&ortalama<=(100-y))
        {
            printf("Sinif Duzeyi:  %s\n",basari_durumlari[i]);
            sinif_indis=i;
            break;
        }
        else if(ortalama<=42.5)
        {
            printf("Sinif Duzeyi:  %s\n",basari_durumlari[7]);
            sinif_indis=7;
            break;
        }
        y += duzen[i];
    }
    return sinif_indis; //bu deger harf notu hesapla fonksiyonunda kullanilacak.
}
void harf_notu_hesapla(double T_skoru[],int sinif_indis,int N,char *harf_notlari[])
{
    char *harf_dizisi[9]={"FF","FD","DD","DC","CC","CB","BB","BA","AA"};
    for(int i=0;i<N;i++) //Ogrenci dongusu:
    {
       for(int j=0;j<8;j++) //Basari duzeyi dongusu. Burasi sadece basari duzeyini belirlemek icin donuyor.
       {
           int z=0;
           if(sinif_indis==j) //Basari duzeyi belirlendi.
           {
               for(int k=0;k<9;k++) //Harf notu dongusu:
               {
                   if(T_skoru[i]<(32+2*j)) //FF araligi.(32+2*j'den kucuk degerler esas kosul algoritmasina girmedigi icin ek kosul kullandik.)
                   {
                       harf_notlari[i]=harf_dizisi[0];
                       break;
                   }
                   else if(T_skoru[i]>=(32+2*j+z)&&T_skoru[i]<(37+2*j+z)) //Esas kosul algoritmasi.(j'ler basari duzeyleri arasindaki farki(2),
                                                                            //z'ler ise harf notlari arasindaki farki(5) temsil ediyor.
                   {
                       harf_notlari[i]=harf_dizisi[k+1];
                       break;
                   }
                   else if(T_skoru[i]>=(67+2*j)) //AA araligi.(67+2*j'den buyuk degerler esas kosul algoritmasina girmedigi icin ek kosul kullandik.)
                   {
                       harf_notlari[i]=harf_dizisi[8];
                       break;
                   }
                   z+=5; // harf notunda FF'den AA'ya ilerledikce araliklar 5'er 5'er artiyor
               }
           }
       }
    }
}
void ogrenci_sonucu_yazdir(double HBN[ ], double T_skoru[ ],char *harf_notlari[ ],int N)
{
    for(int i=0;i<N;i++)
        printf("%03d id'li ogrencinin;  HBN:%.2lf | T skoru:%.2lf | Harf notu:%s\n",i+1,HBN[i],T_skoru[i],harf_notlari[i]);
}
int main()
{
    int S,N,ogrenci_ID,sinif_indis;
    double D,B;
    printf("Soru sayisini giriniz(max 100): ");scanf("%d",&S);
    printf("Ogrenci sayisini giriniz(max 100): ");scanf("%d",&N);
    printf("Bos birakma ihtimalini giriniz(0-1): ");scanf("%lf",&B);
    printf("Dogru yapma ihtimalini giriniz(0-1): ");scanf("%lf",&D);
    char cevap_anahtari[S],ogrenci_cevaplari[N][100],*harf_notlari[100];
    double HBN[N],ortalama,std,T_skoru[N];
    cevap_anahtari_uret(cevap_anahtari,S);
    cevap_anahtari_yazdir(cevap_anahtari,S);
    sinavi_uygula(ogrenci_cevaplari,cevap_anahtari,N,S,B,D);
    ogrenci_ID=N;
    ogrenci_cevabini_yazdir(ogrenci_cevaplari,ogrenci_ID,S);
    ogrencileri_puanla(ogrenci_cevaplari,cevap_anahtari,HBN,N,S);
    ortalama=sinif_ortalamasi_hesapla(HBN,N);
    printf("\nSinif ortalamasi:  %.2lf ",ortalama);
    std=standart_sapma_hesapla(ortalama,HBN,N);
    printf("\nStandart sapma degeri:  %.5lf \n",std);
    sinif_indis=sinif_duzeyi_hesapla(T_skoru,ortalama);
    printf("-----------------------------------------------------------------------------\n");
    T_skoru_hesapla(ortalama,HBN,N,std,T_skoru);
    printf("\n");
    harf_notu_hesapla(T_skoru,sinif_indis,N,harf_notlari);
    ogrenci_sonucu_yazdir(HBN,T_skoru,harf_notlari,N);
    return 0;
}
