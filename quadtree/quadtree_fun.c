#include "quadtree.h"
int czy_poprawne(lisc* korzen){
  if(korzen==NULL) return 0;
  if(korzen->bok>0&&(korzen->bok&(korzen->bok-1))) return 0; //pot 2 ma tylko jedna 1 w bitowym zapisie
  int ile_nulli=0;
  for(int i=0;i<4;i++){
    if(korzen->dzieci[i]==NULL){
      ile_nulli++;
    }
    else{
      if(korzen->bok!=korzen->dzieci[i]->bok*2) return 0;
    }
  }
  if(korzen->stan==0){
    if(ile_nulli) return 0;
    for(int i=0;i<4;i++){
      if(!czy_poprawne(korzen->dzieci[i])) return 0;
    }
  }
  else if(ile_nulli!=4) return 0;
  else if(korzen->stan){
    if(korzen->stan==2){
      if(korzen->faktyczna_dlugosc||korzen->faktyczna_szerokosc) return 0;
    }
    else if(korzen->stan==1){
      if(korzen->faktyczna_dlugosc<=0||korzen->faktyczna_szerokosc<=0) return 0;
    }
    else return 0;
  }
  return 1;
}

void usun(lisc* korzen){
  if(korzen==NULL) return;
  for(int i=0;i<4;i++){
    usun(korzen->dzieci[i]);
  }
  free(korzen);
}

int czy_podziel(piksel* obraz, int bok, int pocz_x, int pocz_y, int szer, int wys){  //0 podziel, 1 lisc, 2 poza //sprawdza czy dany kwadrat nalezy jeszcze podzielic
  if(pocz_x>=szer||pocz_y>=wys) return 2;
  if(pocz_x+bok>szer||pocz_y+bok>wys) return 0;
  if(bok==1) return 1;
  unsigned char r=obraz[pocz_y*szer+pocz_x].red,g=obraz[pocz_y*szer+pocz_x].green,b=obraz[pocz_y*szer+pocz_x].blue;
  for(int i=pocz_y;i<pocz_y+bok;i++){
    for(int j=pocz_x;j<pocz_x+bok;j++){
      if(obraz[i*szer+j].red!=r||obraz[i*szer+j].green!=g||obraz[i*szer+j].blue!=b) return 0;
    }
  }
  return 1;
}
int uzupelnij_strukture(piksel* obraz, int bok, int pocz_x, int pocz_y, int szer, int wys, lisc* korzen){ //podobnie jesli cos poszlo nie tak zwraca 0
  int podziel=czy_podziel(obraz, bok, pocz_x, pocz_y, szer, wys);
  if(podziel==2){
    korzen->bok=bok;
    korzen->faktyczna_dlugosc=0;
    korzen->faktyczna_szerokosc=0;
    korzen->stan=2;
    return 1;
  }
  else if(podziel==1){
    korzen->bok=bok;
    korzen->faktyczna_dlugosc=bok;
    korzen->faktyczna_szerokosc=bok;
    korzen->stan=1;
    korzen->red=obraz[pocz_y*szer+pocz_x].red;
    korzen->green=obraz[pocz_y*szer+pocz_x].green;
    korzen->blue=obraz[pocz_y*szer+pocz_x].blue;
    return 1;
  }
  else{
    korzen->stan=0;
    korzen->bok=bok;
    korzen->faktyczna_szerokosc=szer-pocz_x<bok? szer-pocz_x: bok;
    korzen->faktyczna_dlugosc=wys-pocz_y<bok? wys-pocz_y: bok;
    for(int i=0;i<4;i++){
      korzen->dzieci[i]=calloc(1, sizeof(lisc));
      if(korzen->dzieci[i]==NULL){
        return 0;
      }
      if(!uzupelnij_strukture(obraz, bok/2, pocz_x+(i%2)*(bok/2), pocz_y+(i/2)*(bok/2), szer, wys, korzen->dzieci[i])) return 0;
    }
  }
  return 1;
}

lisc* stworz_drzewo(piksel* obraz, int bok, int pocz_x, int pocz_y, int szer, int wys){
  lisc* korzen=calloc(1, sizeof(lisc));
  if(korzen==NULL) return NULL;
  if(!uzupelnij_strukture(obraz, bok, pocz_x, pocz_y, szer, wys, korzen)){
    usun(korzen);
    return NULL;
  }
  return korzen;
}

void usun_komentarze(FILE* plik){
  int c;
  while((c=fgetc(plik))!=EOF&&(c=='#'||isspace(c))){
    if(c=='#'){
      while(c!=EOF&&c!='\n') c=fgetc(plik);
    }
  }
  if(c!=EOF) ungetc(c, plik);
}

lisc* wczytaj_i_stworz(FILE* plik){
  if(plik==NULL) return NULL;
  char naglowek[3];
  int wys, szer, max, pot, bok_startowy=1;
  usun_komentarze(plik);
  if(!fscanf(plik, "%2s", naglowek)){
    return NULL;
    }
  usun_komentarze(plik);
  if(fscanf(plik, "%d", &szer)!=1){
    return NULL;
    }
  usun_komentarze(plik);
  if(fscanf(plik, "%d", &wys)!=1){
    return NULL;
  }
  usun_komentarze(plik);
  if(fscanf(plik, "%d", &max)!=1){
    return NULL;
  }
  fgetc(plik);
  pot=(szer>wys)?szer:wys;
  while(bok_startowy<pot) bok_startowy*=2;
  piksel* zawartosc=calloc(szer*wys, sizeof(piksel));
  if(zawartosc==NULL){
    return NULL;
  }
  if(!strcmp(naglowek, "P6")){
    if(fread(zawartosc, sizeof(piksel), (size_t)(szer*wys), plik)!=(size_t)szer*wys){
      free(zawartosc);
      return NULL;
      }
    }
  else if(!strcmp(naglowek, "P3")){
    for(int i=0; i<szer*wys;i++){
      if(fscanf(plik, "%hhu %hhu %hhu", &zawartosc[i].red, &zawartosc[i].green, &zawartosc[i].blue)!=3){
        free(zawartosc);
        //fclose(plik);
        return NULL;
      }
    }
  }
  else{
    return NULL;
  }
  lisc* korzen;
  if((korzen=stworz_drzewo(zawartosc, bok_startowy, 0, 0, szer, wys))==NULL){
    free(zawartosc);
    return NULL;
    }
  free(zawartosc);
  return korzen;
}

void wypelnij_plotno(piksel* obraz, int pocz_x, int pocz_y, lisc* korzen, int bok){
  if(korzen==NULL) return;
  if(korzen->stan==1){
    for(int i=0;i<korzen->bok;i++){
      for(int j=0;j<korzen->bok;j++){
        obraz[pocz_y*bok+pocz_x+i*bok+j].red=korzen->red;
        obraz[pocz_y*bok+pocz_x+i*bok+j].green=korzen->green;
        obraz[pocz_y*bok+pocz_x+i*bok+j].blue=korzen->blue;
      }
    }
  }
  else if(korzen->stan==2) return;
  else{
    for(int i=0;i<4;i++){
      wypelnij_plotno(obraz, pocz_x+(i%2)*(korzen->bok/2), pocz_y+(i/2)*(korzen->bok/2), korzen->dzieci[i], bok);
    }
  }
  return;
}

int wypisz_obraz(FILE* plik, lisc* korzen){
  if(plik==NULL) return 3;
  if(!czy_poprawne(korzen)) return 2;
  int rozmiar=korzen->bok;
  piksel* obraz=calloc(rozmiar*rozmiar, sizeof(piksel));
  wypelnij_plotno(obraz, 0, 0, korzen, rozmiar);
  fprintf(plik, "P6 %d %d 255\n", korzen->faktyczna_szerokosc, korzen->faktyczna_dlugosc);
  for(int i=0;i<korzen->faktyczna_dlugosc;i++){
    if(fwrite(&obraz[i*rozmiar], sizeof(piksel), (size_t)korzen->faktyczna_szerokosc, plik)!=(size_t)korzen->faktyczna_szerokosc){
      free(obraz);
      return 1;
      }
  }
  free(obraz);
  return 0;
}

void wypisz_szestnastkowo_rek(lisc* korzen, int poziom, FILE* plik){
  if(korzen==NULL) return;
  if(korzen->stan==0){
    for(int i=0;i<poziom;i++) fputs(" ", plik);
      fputs("{\n",plik);
      for(int i=0;i<4;i++){
        wypisz_szestnastkowo_rek(korzen->dzieci[i], poziom+4, plik);
      }
      for(int i=0;i<poziom;i++) fputs(" ", plik);
      fputs("}\n", plik);
  }
  else if(korzen->stan==1){
    for(int i=0;i<poziom;i++) fputs(" ", plik);
    fprintf(plik, "%02hhX%02hhX%02hhX\n", korzen->red, korzen->green, korzen->blue);
  }
  else{
    for(int i=0;i<poziom;i++) fputs(" ", plik);
    fputs("-\n", plik);
  }
}

int wypisz_szestnastkowo(FILE* plik, lisc* korzen){
  if(plik==NULL) return 3;
  if(!czy_poprawne(korzen)) return 2;
  fprintf(plik, "%d %d %d\n", korzen->bok, korzen->faktyczna_szerokosc, korzen->faktyczna_dlugosc);
  wypisz_szestnastkowo_rek(korzen, 0, plik);
  return 0;
}

