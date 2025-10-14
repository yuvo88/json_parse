When adding a new file:
```bash
cmake -S . -G "Unix Makefiles" -B cmake
```

To compile:
```bash
pushd cmake; make all && ./json-parse; popd
```
