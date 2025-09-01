<h1>Space Invaders with C and Raylib</h1>

In this classic arcade-style shooter, you take control of a spaceship at the bottom of the screen. Your mission: stop the ever-approaching alien invasion fleet before they reach Earth.

<img src="assets/IngameScene.gif" alt="Gameplay Demo" width="500">

<h2>Gameplay</h2>

- Left Arrow Key to move left
  
- Right Arrow Key to move right

- Space to shoot

<h2>Build & Run</h2>
<h3>System Requirements</h3>

- CMAKE >= 3.16

- C-Compiler (gcc, clang, MSVC)
  
- Git

<h3>1. install raylib</h3>
bash:

macOS:

- brew install raylib

Linux: 

- sudo apt install libraylib-dev

Windows:

- install MSYS2

- pacman -S mingw-w64-x86_64-raylib


<h3>2. clone repo</h3>

- git clone https://github.com/dein-user/SpaceInvaders.git

- cd SpaceInvaders

<h3>3. build</h3>
bash:

- cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
- cmake --build build -j


<h3>4. start</h3>

- macOS / Linux
  
bash:

./build/SpaceInvaders


- Windows
  
bash:

.\build\SpaceInvaders.exe




