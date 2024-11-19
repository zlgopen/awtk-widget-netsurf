export PREFIX=$PWD/opt
export PKG_CONFIG_PATH=$PREFIX/lib/pkgconfig
export PATH=$PATH:$PREFIX/bin

rm -rf $PREFIX

for f in libjpeg curl
do
  cmake -S $f -B $f/build
  cmake --build $f/build --config Release
  cmake --install $f/build --prefix opt
done

for f in buildsystem \
  libparserutils \
  libwapcaplet \
  libhubbub \
  libnsutils \
  libutf8proc \
  libdom \
  libcss \
  libnsbmp \
  libnsgif \
  libnsfb \
  librosprite \
  libnspsl \
  libsvgtiny \
  nsgenbind \
  libnslog \
  netsurf
do
  echo "building " $f
  cd $f && make V=1 && make install PREFIX=$PREFIX && cd -
done

cp -f ./netsurf/libnsawtk.dylib ../../bin
cp -fa opt/lib/lib*.dylib ../../bin

#make V=1 TARGET=awtk VARIANT=debug -j4 
#mv netsurf/libnsawtk.a inst-awtk/lib
