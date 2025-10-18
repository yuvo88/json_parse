addfiles:
    cmake -DCMAKE_CXX_COMPILER=g++-15 -S . -G "Unix Makefiles" -B cmake
run:
    /bin/bash -c 'pushd cmake; make all && ./json-parse "[1,2,3,4]"; popd'
