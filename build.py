import os
import sys

source = os.path.dirname(os.path.realpath(__file__))
build_dir = '../build/'

os.makedirs(build_dir, exist_ok=True)
os.chdir(build_dir)

os.system('conan install "' + source + '/" --build=outdated')

os.chdir(source)
if sys.platform.startswith('win'):
    generator = 'vs2019'
else:
    generator = 'gmake2'
os.system('premake5 ' + generator + ' --location="' + build_dir + '"')