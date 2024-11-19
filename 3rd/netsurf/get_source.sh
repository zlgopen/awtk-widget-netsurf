
for f in buildsystem libwapcaplet libparserutils libhubbub libcss libdom libnsbmp libnsgif libsvgtiny  \
  libnslog netsurf librosprite libnsutils libutf8proc libnspsl nsgenbind
do
  if [ -e $f ]
  then
    echo "$f exist."
    cd $f  
    git checkout .
    git pull
    cd ..
  else  
    git clone git://git.netsurf-browser.org/$f.git
  fi  
done

./patch.sh

if [ -e libjpeg ]
then
  echo "libjpeg exist"
else
  git clone https://github.com/winlibs/libjpeg.git
fi

if [ -e curl ]
then
  echo "curl exist"
else
  git clone https://github.com/curl/curl.git
fi

if [ -e bison-2.7.1 ]
then
  echo "bison-2.7.1 exist"
else
  wget https://ftp.gnu.org/gnu/bison/bison-2.7.1.tar.gz
  tar xf bison-2.7.1.tar.gz
  wget https://github.com/westes/flex/files/981163/flex-2.6.4.tar.gz
  tar xf flex-2.6.4.tar.gz
fi
