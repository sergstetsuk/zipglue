# zipglue
zip file splitter/combiner compatible with peazip/7zip produced multivolume archives

# BUILD PROCESS

#Prerequisites

sudo apt install autotools-dev build-essential mingw-w64 upx

# Compilation for Linux
autoreconf -i
mkdir ./build
cd ./build
../configure
make

# Cross compilation for Windows under Linux
autoreconf -i
mkdir ./build
cd ./build
../configure --host=x86_64-w64-mingw32
make

# Strip/Pack Windows
x86_64-w64-mingw32-strip ./src/zipglue.exe
upx ./src/zipglue.exe

# Cleanup
git clean -x -d -f
