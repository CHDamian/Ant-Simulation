# Ants Simulation (OpenMP)

Wielowątkowa symulacja klasycznej zagadki logicznej z mrówkami na patyku, napisana w C++17 z wykorzystaniem standardu OpenMP. 

Zagadka zakłada, że `n` mrówek porusza się po patyku o długości `d` ze stałą prędkością. Gdy dwie mrówki się spotykają, odwracają swoje wektory ruchu. Program rozwiązuje ten problem symulując te środowisko.

## Wymagania
* Środowisko obsługujące konteneryzację (np. Docker Desktop) 
LUB
* Kompilator wspierający C++17 oraz OpenMP (np. GCC 9+ w środowisku Linux/WSL).

## Uruchomienie (Docker - rekomendowane)
Użycie Dockera gwarantuje poprawne działanie OpenMP niezależnie od Twojego systemu operacyjnego.

1. Zbuduj obraz z kodem:
   ```bash
   docker build -t ants-sim .

```

2. Uruchom kontener przekazując długość patyka i liczbę mrówek (np. patyk 20, 4 mrówki):
```bash
docker run --rm ants-sim 20 4

```

## Uruchomienie natywne (Linux / WSL)

Jeśli posiadasz natywny kompilator GCC:

1. Skompiluj kod:
```bash
g++ -std=c++17 -fopenmp -O3 main.cpp -o ants_sim

```


2. Uruchom program:
```bash
./ants_sim 20 4

```
## Uwaga dotycząca pamięci

Program przed symuacją losuje pozycje startowe mrówek upewniając się, że żadna nie pojawi się w tym samym miejscu. Wykoarzystuje do tego pobieranie istniejących jeszcze pozycji z wektora, co przy dużym `d` może doprowadzić do przepełnienia pamięci.