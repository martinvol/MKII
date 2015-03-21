Configurar git:

http://git-scm.com/book/en/v2/Getting-Started-First-Time-Git-Setup

Instalar SDL2

Installar g++ y make

```bash    
    linux$: sudo apt-get install g++ cmake libglu-dev libxi-dev p7zip-full
```



```bash
    linux$: sudo sudo apt-get install build-essential xorg-dev libudev-dev libts-dev libgl1-mesa-dev libglu1-mesa-dev libasound2-dev libpulse-dev libopenal-dev libogg-dev libvorbis-dev libaudiofile-dev libpng12-dev libfreetype6-dev libusb-dev libdbus-1-dev zlib1g-dev libdirectfb-dev 
    linux$: wget http://www.libsdl.org/release/SDL2-2.0.3.zip
	linux$: unzip SDL2-2.0.3.zip
	linux$: cd SDL2-2.0.3/
	linux$: sudo ./configure 
	linux$: make
	linux$: sudo make install
	linux$: cd ..
	linux$: rm -fr SDL2-2.0.3 SDL2-2.0.3.zip 
```


Ahora ya se puede compilar así

```bash    
	linux$: g++ test_grafico.c -w -lSDL2 -o prueba.bin
	linux$: ./prueba.bin
```


Instalar Box2D

```bash
	linux$: wget https://box2d.googlecode.com/files/Box2D_v2.3.0.7z
	linux$: 7zr x Box2D_v2.3.0.7z 
	linux$: cd Box2D_v2.3.0/Box2D/Build
	linux$: sudo cmake -DBOX2D_INSTALL=ON -DBOX2D_BUILD_SHARED=ON ..
	linux$: sudo make
	linux$: sudo make install
	linux$: export LD_LIBRARY_PATH
```

El export de ahí hay que ponerlo cada vez que se reinicia la computadora, después veo si tego un comando definitivo

luego para compilar:

```bash
	linux$: g++ test_box.c -L/usr/local/lib  -lBox2D
```

Para agregar la librería de SDL2 inherente a imágenes:
```bash
	linux$: sudo apt-get install libsdl2-image-2.0-0
```
Para usar esa librería no olvidarse de compilar con -lSDL2_image, además de hacer los includes necesarios