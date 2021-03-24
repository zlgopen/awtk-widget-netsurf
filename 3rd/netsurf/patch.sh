cp -rf awtk-port/awtk netsurf/frontends/
cd netsurf
patch -p1 <../awtk-port/netsurf.diff
cd -
