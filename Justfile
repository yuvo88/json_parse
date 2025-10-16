addfiles:
    cmake -DCMAKE_CXX_COMPILER=g++-15 -S . -G "Unix Makefiles" -B cmake
run:
    /bin/bash -c 'pushd cmake; make all && ./json-parse "{\"something\": 1, \"alksjdf\": true, 1: true }"; popd'
