<h1>Space Invaders with C and Raylib</h1>



<h2>Build & Run</h2>
<h3>System Requirements</h3>

- CMAKE >= 3.16

- C-Compiler (gcc, clang, MSVC)
  
- Git

<h3>1. clone repo</h3>
bash:

- git clone --recursive https://github.com/dein-user/SpaceInvaders.git
  
- cd SpaceInvaders
  
- if you forgot "--recursive": git submodule update --init --recursive

<h3>2. build</h3>
bash:

- cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
- cmake --build build -j


<h3>3. start</h3>

- macOS / Linux
  
bash:

./build/run/SpaceInvaders


- Windows
  
bash:

.\build\run\SpaceInvaders.exe




