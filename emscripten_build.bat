@echo off

if not exist build_web mkdir build_web
cd build_web

call emcmake cmake ..
call emmake ninja

python -m http.server

pause