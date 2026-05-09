

## **Quadtree**

Głównym zadaniem programu jest analiza obrazów w formacie PPM i zamiana ich na strukturę drzewiastą. Zamiast przechowywać każdy piksel oddzielnie, program łączy obszary o tym samym kolorze w większe bloki (liście), co pozwala na oszczędność pamięci przy prostych grafikach.

## Struktura Drzewa

Każdy węzeł (`lisc`) w strukturze reprezentuje kwadratowy obszar obrazu:
* **Stan 0**: Węzeł wewnętrzny (posiada 4 dzieci).
* **Stan 1**: Liść zawierający konkretny kolor (RGB).
* **Stan 2**: Obszar pusty (poza granicami obrazu).

### Funkcje programu
* `wczytaj_i_stworz`: Obsługuje formaty PPM (P3 oraz P6). Pomija komentarze w nagłówkach.
* `stworz_drzewo`: Buduje strukturę, dopasowując rozmiar drzewa (bok będący potęgą 2) do wymiarów obrazu.
* `wypisz_obraz`: Generuje binarny plik PPM (P6) na podstawie drzewa.
* `wypisz_szestnastkowo`: Tworzy tekstową reprezentację drzewa z wcięciami.
* `czy_poprawne`: Sprawdza integralność drzewa (wymiary, wskaźniki).
* `usun`: Zwalnia pamięć zajmowaną przez drzewo.

## **Generator Kształtów**

Interpretuje komendy ze standardowego wejścia i nanosi figury na wirtualne płótno. Komendy wczytuje za pomocą własnej funkcji 'fgetline', która nie posiada limitu długości linii:
* Zaczyna od 4 bajtów.
* Przy przepełnieniu wywołuje `realloc(bufor, rozmiar * 2)`.

### Obsługiwane figury:
* `rectangle [kolor] [x] [y] [a] [b] [kat]` – Prostokąt o środku (x,y), bokach a, b i rotacji `kat`.
* `ellipse [kolor] [x] [y] [a] [b] [kat]` – Elipsa o osiach a, b, obrócona o kąt `kat`. 
* `polygon [kolor] [x1] [y1] [x2] [y2] ...` – Wielokąt wypukły o dowolnej liczbie punktów (przynależność punktu do wielokąta jest sprawdzana przy użyciu iloczynu wektorowego (badanie znaku po tej samej stronie każdej krawędzi), dlatego wierzchołki muszą być podawane kolejno).

**Dostępne kolory:** `white`, `black`, `red`, `green`, `blue`, `yellow`.

  *Generator wykorzystuje bibliotekę matematyczną.*
