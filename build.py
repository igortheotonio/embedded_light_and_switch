import sys
import os
import shutil

build = 'build'
compile_code = 'cmake -GNinja  ..'
make = 'make -j4'

if __name__ == "__main__":
    
    if len(sys.argv) != 2:
        print('Error in parameters')
        exit(-1)

    print('Run...')
    
    path = sys.argv[1]

    try:
        path = os.path.join(os.getcwd(), path)
        folder = path + build
        if os.path.isdir(folder) :
            shutil.rmtree(folder)
        os.mkdir(folder)
        os.chdir(folder)
        os.system(compile_code)
        os.system(make)
    except Exception as e:
        print('Error in compile', e)
        pass