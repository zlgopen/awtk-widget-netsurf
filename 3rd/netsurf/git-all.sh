
for f in buildsystem libwapcaplet libparserutils libhubbub libcss libdom libnsbmp libnsgif libsvgtiny  \
  libnslog netsurf librosprite libnsutils libutf8proc libnspsl nsgenbind
do
  if [ -e $f ]
  then
    echo "$f exist."
    cd $f  
    git pull
    cd ..
  else  
    git clone git://git.netsurf-browser.org/$f.git
  fi  
done

if [ -e libjpeg ]
then
  echo "libjpeg exist"
else
  git clone https://github.com/winlibs/libjpeg.git
fi

if [ -e curl ]
then
  echo "curl"
else
  git clone https://github.com/curl/curl.git
fi

