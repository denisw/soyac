# soyac - Compiler for a Java/C#-like toy language

`soyac` is a compiler I wrote between 2007 and 2009 with the goal to create my
very own programming language. implemented programming language (called *Soya*)
is very basic, but has support for variables, integer types and arithmetic,
control flow statements (`if`, `while`, `for`), functions (including function
pointers) and primitive object orientation. Here is a working example source
file:

```javascript
struct Point
{
    var x : int;
    var y : int;

    constructor(x : int, y : int)
    {
        this.x = x;
        this.y = y;
    }
}

function offset_by(p : Point, x : int, y : int) : Point
{
    var offset = p;

    offset.x = offset.x + x;
    offset.y = offset.y + y;

    return offset;
}

var p = new Point(1, 218);

var p2 = offset_by(p, 10, 4);
print(p2.x);
print(p2.y);
```

More code examples can be found in the `test/torture` folder, which
doubles as regression test suite. I also tried to write a language
specification in DocBook at that time, which can be found at
`doc/langref`.

The compiler is written in C++ and originally used [LLVM](https://llvm.org/)
2.x as its backend. Recently I rebased it on LLVM 19.x, switched the build
system from SCons to CMake and made it compile on macOS in addition to Linux,
on which it was originally implemented.

## Trying it Out

### Dev Containers (Recommended)

The repository includes a [Dev Container](https://containers.dev/)
configuration, which allows you to set up a container-based development
environment with all dependencies pre-installed. You can set it up in one of the
following ways: 

- Open the project in Visual Studio Code, CLion, Zed or another editor with Dev 
  Containers support and follow the prompt to reopen it in the Dev Container.

- Use the [Dev Container CLI](https://github.com/devcontainers/cli) to build
  and run the container with:

  ```sh
  devcontainer up
  ```

  Once the container has started, you can then run the commands described in
  the next section by prefixing them with `devcontainer exec`, e.g. 
  `devcontainer exec just build`. 

### Manual Environment Setup

Install the following prerequisites:

* [CMake](https://cmake.org/) >= 3.31
* [Ninja](https://ninja-build.org/)
* [Just](https://just.systems/) (optional)
* [LLVM](https://llvm.org/) 19.x
* [Bison](https://www.gnu.org/software/bison/) >= 3.8
* [Boehm-Demers-Weiser GC](https://www.hboehm.info/gc/) 
* [Boost](https://www.boost.org/doc/libs/1_83_0/doc/html/signals2.html)
* [CppUnit](https://sourceforge.net/projects/cppunit/) (test suite only)

**Debian / Ubuntu:**

```sh
sudo apt-get install \
      build-essential \
      bison \
      cmake \
      libboost-dev \
      libboost-filesystem-dev \
      libboost-program-options-dev \
      libboost-regex-dev \
      libcppunit-dev \
      libgc-dev \
      llvm-19-dev \
      ninja-build \
      just 
```

**macOS (Homebrew):**

```sh
brew install bison boost cmake cppunit just llvm 
```

### Configuring the Build

Run CMake to configure the build with the `default` preset (defined in
`CMakePresets.json`). This will set up a `build` directory within the
project root.


```sh
just configure
# cmake --preset default
```

> [!IMPORTANT]
> The Homebrew packages for Bison and LLVM are [keg-only] to avoid conflicts
> with the copies that ship with macOS / Xcode. To point CMake to the Homebrew
> versions, pass their locations via the corresponding `_ROOT` variables:
> 
> ```sh
> cmake --preset default \
>     -DBISON_ROOT=$(brew --prefix bison) \
>     -DLLVM_ROOT=$(brew --prefix llvm)
> ```

[keg-only]: https://docs.brew.sh/FAQ.html#what-does-keg-only-mean

### Building

You can build the Soya compiler, runtime and tests with:

```sh
just build
# cmake --build --preset debug
```

### Running the Tests

The Soya compiler comes with both unit and end-to-end tests. Both can be run
with:


```sh
just test
# ctest --preset default
```

`just test` automatically rebuilds before the tests are run.
