default: configure build

configure:
  cmake --preset default -DBISON_ROOT=$(brew --prefix bison) -DLLVM_ROOT=$(brew --prefix llvm@19)

build config="debug":
  cmake --build --preset {{config}}

test: (build "debug")
  ctest --preset default
