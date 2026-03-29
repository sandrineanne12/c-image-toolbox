#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <ctype.h>

typedef struct Lisc{
  struct Lisc* dzieci[4];
  int bok;
  int faktyczna_dlugosc;
  int faktyczna_szerokosc;
  unsigned char stan; // 0 wewnetrzny 1 lisc 2 poza
  unsigned char red;
  unsigned char green;
  unsigned char blue;
}lisc;

typedef struct{
  unsigned char red;
  unsigned char green;
  unsigned char blue;
}piksel;

int czy_poprawne(lisc* korzen);
void usun(lisc* korzen);
lisc* wczytaj_i_stworz(FILE* plik);
int wypisz_obraz(FILE* plik, lisc* korzen);
int wypisz_szestnastkowo(FILE* plik, lisc* korzen);
