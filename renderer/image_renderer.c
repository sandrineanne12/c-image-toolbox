#include <stdio.h>
#include <stdlib.h>
#include "fgetline.h"
#include <string.h>
#include <ctype.h>
#include <math.h>

#define pi 3.14159265358979323846
typedef struct{
  unsigned char a;
  unsigned char b;
  unsigned char c;
}tusz;

typedef struct{
  int x;
  int y;
}polygon_wspl;
int jaki_kolor(char *kolorek, tusz* farba){
  if(!strcmp(kolorek, "white")){ //int white=0, black=0, red=0, green=0, blue=0, yellow=0;
    farba->a=255;
    farba->b=255;
    farba->c=255;
    return 0;
  }
  if(!strcmp(kolorek, "black")){
    farba->a=0;
    farba->b=0;
    farba->c=0;
    return 0;
  }
  if(!strcmp(kolorek, "red")){
    farba->a=255;
    farba->b=0;
    farba->c=0;
    return 0;
  }
  if(!strcmp(kolorek, "green")){
    farba->a=0;
    farba->b=255;
    farba->c=0;
    return 0;
  }
  if(!strcmp(kolorek, "blue")){
    farba->a=0;
    farba->b=0;
    farba->c=255;
    return 0;
  }
  if(!strcmp(kolorek, "yellow")){
    farba->a=255;
    farba->b=255;
    farba->c=0;
    return 0;
  }
  else return 1;
}

int nadmiarowe_informacje(char* wiersz, int i){
  for(;wiersz[i]!='\0';i++){
    if(!isspace(wiersz[i])) return 1;
  }
  return 0;
}

void rysuj_prostokat(int sr_x, int sr_y ,int a, int b, int kat, tusz* matryca, tusz* farba, int szer, int wys){
  double radiany=-(kat)*pi/180.0;
  double wektor_x, wektor_y;
  double wspolrzedna_x, wspolrzedna_y;
  for(int i=0;i<wys;i++){
    for(int j=0;j<szer;j++){
      wektor_x=sr_x-j;
      wektor_y=sr_y-i;
      wspolrzedna_x=wektor_x*cos(radiany)-wektor_y*sin(radiany);
      wspolrzedna_y=wektor_x*sin(radiany)+wektor_y*cos(radiany);
      if(wspolrzedna_x<=a/2.0&&wspolrzedna_x>=-a/2.0&&wspolrzedna_y<=b/2.0&&wspolrzedna_y>=-b/2.0){
        matryca[i*szer+j].a=farba->a;
        matryca[i*szer+j].b=farba->b;
        matryca[i*szer+j].c=farba->c;
      }
    }
  }
}

void rysuj_elipse(int sr_x, int sr_y ,int a, int b, int kat, tusz* matryca, tusz* farba, int szer, int wys){
  double radiany=-(kat)*pi/180.0;
  double wektor_x, wektor_y;
  double wspolrzedna_x, wspolrzedna_y;
  double os_rx=a/2.0, os_ry=b/2.0;
  for(int i=0;i<wys;i++){
    for(int j=0;j<szer;j++){
      wektor_x=sr_x-j;
      wektor_y=sr_y-i;
      wspolrzedna_x=wektor_x*cos(radiany)-wektor_y*sin(radiany);
      wspolrzedna_y=wektor_x*sin(radiany)+wektor_y*cos(radiany);
      if(wspolrzedna_x*wspolrzedna_x/(os_rx*os_rx)+wspolrzedna_y*wspolrzedna_y/(os_ry*os_ry)<=1){
        matryca[i*szer+j].a=farba->a;
        matryca[i*szer+j].b=farba->b;
        matryca[i*szer+j].c=farba->c;
      }
    }
  }
}
void rysuj_polygon(polygon_wspl* wierzcholki, int ile_wierzcholkow, tusz* matryca, tusz* farba, int szer, int wys){
  //policz wektory miedzy kazdym z punktow, ostatni z pierwszym tez
  int wektor_prostej_x, wektor_prostej_y;
  int wektor_punkt_od_prostej_x, wektor_punkt_od_prostej_y;
  int w_srodku=1;
  
  for(int i=0;i<wys;i++){
    for(int j=0;j<szer;j++){
      w_srodku=1;
      for(int k=0;k<ile_wierzcholkow;k++){
        wektor_prostej_x=wierzcholki[k].x-wierzcholki[(k+1)%ile_wierzcholkow].x;
        wektor_prostej_y=-(wierzcholki[k].y-wierzcholki[(k+1)%ile_wierzcholkow].y); //- bo rosnie w dol
        wektor_punkt_od_prostej_x=wierzcholki[k].x-j;
        wektor_punkt_od_prostej_y=-(wierzcholki[k].y-i);
        if(wektor_prostej_x*wektor_punkt_od_prostej_y-wektor_prostej_y*wektor_punkt_od_prostej_x>0){
          w_srodku=0;
          break;
        }
      }  
      if(w_srodku){
        matryca[i*szer+j].a=farba->a;
        matryca[i*szer+j].b=farba->b;
        matryca[i*szer+j].c=farba->c;
      }
    }
  }

}

int main(int argc, char* argv[]){
  int bin=0;
  char* nazwa_pliku=NULL;
  if(argc>3){
    fputs("zbyt duzo argumentow\n", stderr);
    return 1;
  }
  for(int i=1;i<argc;i++){
    if(!strcmp(argv[i],"-b")||!strcmp(argv[i],"--binary")){
      if(bin){
        fputs("dwukrotna flaga\n", stderr);
        return 1;
      }
      bin=1;
    }
    else{
      if(nazwa_pliku!=NULL){
        fputs("dwie nazwy pliku\n", stderr);
        return 1;
      }
      nazwa_pliku=argv[i];
    }
  }
  
  FILE* wyjscie=stdout;
    if(nazwa_pliku!=NULL&&bin){
      wyjscie=fopen(nazwa_pliku, "wb");
    }
    else if(nazwa_pliku!=NULL&&!bin){
      wyjscie=fopen(nazwa_pliku, "w");
    }
    if(wyjscie==NULL){
        fputs("blad przy otwieraniu pliku do zapisu\n", stderr);
        return 1;
      }
  
  int szer, wys;
  char* wymiary;
  int koniec=0, ile_wczytano;
  wymiary=fgetline(stdin, NULL);
  ile_wczytano=sscanf(wymiary, "%d %d%n", &szer, &wys, &koniec);
    if(ile_wczytano<2){
      fputs("zbyt malo danych, wiersz 0\n", stderr);
      return 1;
    }
    else if(szer<=0||wys<=0){
      fputs("nieprawidlowe wymiary, wiersz 0\n", stderr);
      return 1;
    }
    tusz* matryca=calloc(szer*wys, sizeof(tusz));
    if(matryca==NULL){
      fputs("blad alokacji pamieci\n", stderr);
      return 1;
    }
    char* wiersz;
    int ktory_wiersz=0;
    int gdzie_koniec=0, gdzie_koniec2=0;
    char figura[11];
    char kolor[11];
    int x, y, a, b, kat;
    int kod_wyjscia=0;
    tusz *farba=calloc(1, sizeof(tusz));
    polygon_wspl* polygon_wspolrzedne=calloc(4, sizeof(polygon_wspl));
    int miejsce_na_wierzcholki=4;
    int index_wierzcholka=0;
    polygon_wspl* kopia;
    int realloc_dal_rade=1;
    for(int i=0;i<wys*szer;i++){
      matryca[i].a=255;
      matryca[i].b=255;
      matryca[i].c=255;
    }
    while((wiersz=fgetline(stdin, NULL))!=NULL){
      ktory_wiersz++;
      ile_wczytano=sscanf(wiersz, "%10s %10s%n", figura, kolor, &gdzie_koniec); //wczytaj pare znakow wiecej niz najdluzsza nazwa gdyby nazwa miala za duzo liter
      if(ile_wczytano<2){
        fprintf(stderr, "zbyt malo danych, linia %d\n", ktory_wiersz);
        kod_wyjscia=2;
        free(wiersz);
        continue;
      }
      if(jaki_kolor(kolor, farba)){
        fprintf(stderr, "nieznany kolor %d\n", ktory_wiersz);
        kod_wyjscia=2;
        free(wiersz);
        continue;
      }
      
      //////////////////////////////////////////////////////////////ELIPSA//////////////////////////////////////////////////////////////////////////////
      if(!strcmp(figura, "ellipse")){
        ile_wczytano=sscanf(wiersz+gdzie_koniec, "%d %d %d %d %d%n", &x, &y, &a, &b, &kat, &gdzie_koniec2);
        if(ile_wczytano<5){
          fprintf(stderr, "zbyt malo argumentow %d\n", ktory_wiersz);
          kod_wyjscia=2;
          free(wiersz);
          continue;
        }
        if(a<=0||b<=0){
          fprintf(stderr, "ujemne boki %d\n", ktory_wiersz);
          kod_wyjscia=2;
          free(wiersz);
          continue;
        }
        if(nadmiarowe_informacje(wiersz, gdzie_koniec+gdzie_koniec2)){
          fprintf(stderr, "nadmiarowe informacje %d\n", ktory_wiersz);
          kod_wyjscia=2;
        }
        rysuj_elipse(x, y, a, b, kat, matryca, farba, szer, wys);
        free(wiersz);
      }
      
      ////////////////////////////////////////////////////////////////PROSTOKAT/////////////////////////////////////////////////////////////////////
      else if(!strcmp(figura, "rectangle")){
        ile_wczytano=sscanf(wiersz+gdzie_koniec, "%d %d %d %d %d%n", &x, &y, &a, &b, &kat, &gdzie_koniec2);
        if(ile_wczytano<5){
          fprintf(stderr, "zbyt malo argumentow %d\n", ktory_wiersz);
          kod_wyjscia=2;
          free(wiersz);
          continue;
        }
        if(a<=0||b<=0){
          fprintf(stderr, "ujemne boki %d\n", ktory_wiersz);
          kod_wyjscia=2;
          free(wiersz);
          continue;
        }
        if(nadmiarowe_informacje(wiersz, gdzie_koniec+gdzie_koniec2)){
          fprintf(stderr, "nadmiarowe informacje %d\n", ktory_wiersz);
          kod_wyjscia=2;
        }
        rysuj_prostokat(x, y, a, b, kat, matryca, farba, szer, wys);
        free(wiersz);
        
      }
      
      ///////////////////////////////////////////////////////////POLYGON////////////////////////////////////////////////////////////////////////
      else if(!strcmp(figura, "polygon")){
        index_wierzcholka=0;
        realloc_dal_rade=1;
        while((ile_wczytano=sscanf(wiersz+gdzie_koniec, "%d %d%n", &x, &y, &gdzie_koniec2))==2){
          gdzie_koniec+=gdzie_koniec2;
          if(index_wierzcholka==miejsce_na_wierzcholki-1){
            kopia=realloc(polygon_wspolrzedne, miejsce_na_wierzcholki*2*sizeof(polygon_wspl));
            if(kopia==NULL){
              realloc_dal_rade=0;
              break;
            }
            polygon_wspolrzedne=kopia;
            miejsce_na_wierzcholki*=2;
          }
          polygon_wspolrzedne[index_wierzcholka].x=x;
          polygon_wspolrzedne[index_wierzcholka].y=y;
          index_wierzcholka++;
        }
        if(!realloc_dal_rade){
          fprintf(stderr, "nie udalo sie wczytac wszystkich wierzcholkow %d\n", ktory_wiersz);
          kod_wyjscia=2;
          free(wiersz);
          continue;
        }
        if(ile_wczytano==1){
          fprintf(stderr, "nieparzysta liczba wspolrzednych wierzcholkow %d\n", ktory_wiersz);
          kod_wyjscia=2;
          free(wiersz);
          continue;
        }
        if(index_wierzcholka<3){
          fprintf(stderr, "za malo wierzcholkow %d\n", ktory_wiersz);
          kod_wyjscia=2;
          free(wiersz);
          continue;
        }
        if(nadmiarowe_informacje(wiersz, gdzie_koniec)){
          fprintf(stderr, "nadmiarowe informacje %d\n", ktory_wiersz);
          kod_wyjscia=2;
        }
        rysuj_polygon(polygon_wspolrzedne, index_wierzcholka, matryca, farba, szer, wys);
        free(wiersz);
       /////////////////////////////////////////////////////////////////NIEZNANA FIGURA////////////////////////////////////////////////////////////// 
      }
      else{
        fprintf(stderr, "nieznana figura %d\n", ktory_wiersz);
        kod_wyjscia=2;
        free(wiersz);
        continue;
      }
    }
    
  if(bin){
    //puts("ola");
    fprintf(wyjscie, "P6 %d %d 255\n", szer, wys);
    if(fwrite(matryca, sizeof(tusz), (size_t)szer*wys, wyjscie)!=(size_t)szer*wys){
      fputs("blad zapisu", stderr);
    }
  }
  else{
    fprintf(wyjscie, "P3 %d %d 255\n", szer, wys);
    for(int i=0;i<szer*wys;i++){
      fprintf(wyjscie, "%d %d %d ",matryca[i].a, matryca[i].b, matryca[i].c);
    }
  }
  if(nazwa_pliku!=NULL){
    if(fclose(wyjscie)==EOF) fputs("blad zapisu\n", stderr);
  }
  free(farba);
  free(matryca);
  free(polygon_wspolrzedne);
  return kod_wyjscia;
}
