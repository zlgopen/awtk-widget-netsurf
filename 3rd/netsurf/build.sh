export PREFIX=$PWD/opt
export PKG_CONFIG_PATH=$PREFIX/lib/pkgconfig
export PATH=$PATH:$PREFIX/bin

rm -rf $PREFIX

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
  cd $f && make install PREFIX=$PREFIX && cd -
done

#make V=1 TARGET=awtk VARIANT=debug -j4 
#mv netsurf/libnsawtk.a inst-awtk/lib
