# Fuzz testing

`iggy-cpp-client` uses Clang's [libFuzzer](http://llvm.org/docs/LibFuzzer.html) for fuzz testing; you can find the full set of tests under `fuzz/` and can build it as follows:

```
CC=clang CXX=clang++ cmake -GNinja -DFUZZ=1 -B build
ninja -C build
```

From the `build/` directory, you can then run the fuzzers. For example:

```
./fuzz/pub_client -max_len=10000 -jobs=32 -workers=32
```

Setup based on Google's [boringssl](https://github.com/google/boringssl). *Not yet fully implemented.*