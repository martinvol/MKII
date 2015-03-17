installar SDL

::
    linux$: sudo sudo apt-get install build-essential xorg-dev libudev-dev libts-dev libgl1-mesa-dev libglu1-mesa-dev libasound2-dev libpulse-dev libopenal-dev libogg-dev libvorbis-dev libaudiofile-dev libpng12-dev libfreetype6-dev libusb-dev libdbus-1-dev zlib1g-dev libdirectfb-dev 
    
    linux$: wget http://www.libsdl.org/release/SDL2-2.0.3.zip
	linux$: unzip SDL2-2.0.3.zip
	linux$: cd SDL2-2.0.3/
	linux$: sudo ./configure 
	linux$: make
	linux$: sudo make install
	linux$: cd ..
	linux$: rm -fr SDL2-2.0.3 SDL2-2.0.3.zip 


Ahora ya se puede compilar así

::	
	 g++ test_grafico.c -w -lSDL2 -o prueba.bin



