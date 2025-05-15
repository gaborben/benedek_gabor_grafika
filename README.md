## Számítógépes Grafika Project
Benedek Gábor (ABCHLD)

## Leírás
Ez a projekt egy 3D-s játék, amelyben a játékosnak faágakat kell gyűjtenie, hogy táplálhassa velük a pályán lévő tábortüzet.
A cél, hogy minél tovább égjen a tűz.

## Főbb funkciók
- OpenGL fix-pipeline alapú 3D-s megjelenítés
- OBJ modell- és JPG textúra-betöltés SDL2_image segítségével
- A fa és kő modelleken található collider (A kő tetejére fel lehet ugrani)
- 2D-s szövegkirajzolás: idő és highscore megjelenítése
- Highscore rendszer, amely a `data/highscore.txt` fájlban tárolja a rekordidőt

## Követelmények
- GCC (MinGW) vagy más C fordító
- SDL2, SDL2_image
- OpenGL (lopengl32)
- `make` (Git Bash vagy MSYS2 alatt Windows-on)

## Telepítés és futtatás

1. A repository klónozása:
bash
   git clone https://github.com/gaborben/benedek_gabor_grafika.git
   cd benedek_gabor_grafika

2. Fordítás és futtatás:
bash
   make

A sikeres build után a `build/project.exe` indítja a játékot.

## Fájlstruktúra

project/
│
├── src/            # C források (.c)
├── include/        # Fejlécek (.h)
├── assets/         # Modellek és textúrák
│   ├── models/
│   └── textures/
├── data/           # Futásidejű adatok (highscore.txt)
├── build/          # Fordított állományok (project.exe)
├── Makefile        # Build script
└── README.md       # Dokumentáció

## Billentyűzet-kiosztás

W/A/S/D:          mozogás
Egér mozgatás:    körbenézés
Space:            ugrás
Left Shift:       sprint
E:                faágak gyűjtéése
F1:               on-screen guide ki-/bekapcsolása
Esc:              kilépés a játékból

---

Kellemes játékot és sikeres rekorddöntést!

```
```
