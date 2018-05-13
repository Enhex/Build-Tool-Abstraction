set source="%~dp0\"

cd ..
mkdir build
cd build

conan install %source% --build=missing
REM conan install %source% --build=missing -s build_type=Debug

cd %source%
premake5 vs2017