set source="%~dp0\"

set build_dir=build-debug

cd ..
mkdir %build_dir%
cd %build_dir%

conan install %source% --build=outdated -s build_type=Debug

cd %source%
premake5 vs2019 --location=../%build_dir%/