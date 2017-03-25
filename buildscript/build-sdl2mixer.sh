export VERSION=2.0.1
export DIRECTORY_TOINSTALL="$PWD/final"
NBPROC=$(getconf _NPROCESSORS_ONLN)

mkdir ./final
if [ -d "./SDL2_mixer-$VERSION.tar.gz" ]; then
wget https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-$VERSION.tar.gz
fi

tar -xf SDL2_mixer-$VERSION.tar.gz
cd ./SDL2_mixer-$VERSION
./configure --prefix=$DIRECTORY_TOINSTALL --enable-static --disable-shared --disable--music-mod-modplug-shared --disable-music-mod-mikmod-shared --disable-music-midi --disable-music-ogg-shared --disable-music-flac-shared --disable-music-mp3-smpeg-shared
make -j$NBPROC
make install
