export VERSION=2.0.5
export DIRECTORY_TOINSTALL="$PWD/final"
NBPROC=$(getconf _NPROCESSORS_ONLN)

mkdir ./final
if [ -d "./SDL2-$VERSION.tar.gz" ]; then
wget https://libsdl.org/release/SDL2-$VERSION.tar.gz
fi
tar -xf SDL2-$VERSION.tar.gz
cd ./SDL2-$VERSION
./configure --prefix=$DIRECTORY_TOINSTALL --enable-static --disable-shared --disable-sndio CFLAGS="-Ofast" LDFLAGS="-flto"
make -j$NBPROC
make install
