#include "fgetline.h"
char* fgetline(FILE* plik, long* rozmiar_bufora){
  if(plik==NULL) return NULL;
  int c, ile=0;
  long rozmiar=4;
  char* kopia;
  char* bufor=calloc(4, sizeof(char));
  if(bufor==NULL) return NULL;
  
  while((c=fgetc(plik))!=EOF){
    if(ile+1==rozmiar){
      kopia=realloc(bufor, rozmiar*2);
      if(kopia==NULL){
        ungetc(c, plik);
        break;
      }
      bufor=kopia;
      rozmiar*=2;
    }
    bufor[ile]=c;
    ile++;
    
    if(c=='\n') break;
  }
  if(!ile){
    free(bufor);
    return NULL;
  }
  
  bufor[ile]='\0';
  if(rozmiar>ile+1){
    kopia=realloc(bufor, ile+1);
    if(kopia!=NULL){
      bufor=kopia;
      rozmiar=ile+1;
    } 
  }
  if(rozmiar_bufora!=NULL) *rozmiar_bufora=rozmiar;
  return bufor;
}
