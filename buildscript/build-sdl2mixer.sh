export VERSION=2.0.2
export DIRECTORY_TOINSTALL="$PWD/final"
NBPROC=$(getconf _NPROCESSORS_ONLN)
export CC="musl-gcc"

mkdir ./final
wget https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-$VERSION.tar.gz

tar -xf SDL2_mixer-$VERSION.tar.gz
cd ./SDL2_mixer-$VERSION
CC="musl-gcc" ./configure --prefix=$DIRECTORY_TOINSTALL --enable-static --disable-shared --disable--music-mod-modplug-shared --disable-music-mod-mikmod-shared --disable-music-midi --disable-music-ogg-shared --disable-music-flac-shared --disable-music-mp3-smpeg-shared
make -j$NBPROC
make install
