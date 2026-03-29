#include "quadtree.h"

int main(int argc, char* argv[]){  //jako pierwsze wejscie, jako drugie wyjscie
  //printf("%zu\n", sizeof(lisc));
  
  if(argc!=3) return 5;
  FILE* wejscie=fopen(argv[1], "rb");
  if(wejscie==NULL){
    fputs("nie udalo sie otworzyc pliku zrodlowego", stderr);
    return 2;
    }
  
  lisc* korzen=wczytaj_i_stworz(wejscie);
  fclose(wejscie);
  if(korzen==NULL){
    fputs("niepoprawny plik lub blad przydzialu pamieci\n", stderr);
    //fclose(wejscie);
    return 1;
  }
  
  FILE* wyjscie=fopen(argv[2], "wb");
  if(wyjscie==NULL){
    //fclose(wejscie);
    fputs("nie udalo sie otworzyc pliku wyjsciowego", stderr);
    return 2;
  }
  FILE* test_budowania_drzewa=fopen("budowanie_drzewa.ppm", "wb");
  wypisz_szestnastkowo(wyjscie, korzen);
  wypisz_obraz(test_budowania_drzewa, korzen);
  usun(korzen);
  if(fclose(wyjscie)){
    fputs("blad zamkniecia pliku\n", stderr);
    fclose(test_budowania_drzewa);
    return 1;
  }
  fclose(test_budowania_drzewa);
  return 0;
}
