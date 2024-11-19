source build_env.sh

rm -rf $PREFIX

for f in flex-2.6.4 bison-2.7.1
do
  cd $f
  ./configure --prefix=$PREFIX
  make
#  make install
  cd -
done

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
  libnslog 
do
  echo "building " $f
  cd $f && make V=1 && make install PREFIX=$PREFIX;cd -
done
  
cd netsurf && make V=1 TARGET=awtk; cd -

INSTALL_DIR=../../bin

mkdir $INSTALL_DIR
cp -f ./netsurf/libnsawtk.dylib $INSTALL_DIR
cp -fa opt/lib/lib*.dylib $INSTALL_DIR

#make V=1 TARGET=awtk VARIANT=debug -j4 
#mv netsurf/libnsawtk.a inst-awtk/lib
