export VERSION=2.0.8
export DIRECTORY_TOINSTALL="$PWD/final"
NBPROC=$(getconf _NPROCESSORS_ONLN)

mkdir ./final
wget https://libsdl.org/release/SDL2-$VERSION.tar.gz
tar -xf SDL2-$VERSION.tar.gz
cd ./SDL2-$VERSION
./configure --prefix=$DIRECTORY_TOINSTALL --enable-static --disable-shared --disable-sndio CFLAGS="-Ofast" LDFLAGS="-flto" --disable-pulseaudio --disable-jack
make -j$NBPROC
make install
