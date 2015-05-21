# soyac - Compiler for a Java/C#-like toy language

`soyac` is a compiler I wrote between 2007 and 2009 with the goal to
create my very own programming language. It is written in C++ and uses
a now ancient version of LLVM (early 2.x) as its backend. The
implemented programming language (called *Soya*) is very basic, but
has support for variables, integer types and arithmetic, control flow
statements (`if`, `while`, `for`), functions (including function
pointers) and primitive object orientation. Here is a working example
source file:

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

## Trying it out

`soyac` has been written on - and probably only works on -
Linux. Because the required versions of LLVM are so old, it is not
trivial to build it on a recent Linux distribution, though. For this
reason, I have written a [Vagrant](http://vagrantup.com/)
configuration that makes it possible to test the compiler in a
virtualized Ubuntu 10.04 system. To do so, install VirtualBox and
Vagrant, change to the project root and run

    vagrant up

After this is done, log into the VM with `vagrant ssh` and change to
the `/soyac` directory, which is synced with the project direcory on
your host system. To compile and install `soyac`, run

    scons
    sudo scons install

You can now run the compiler with `soyac <source file>`. Just try it
on one of the example files on `test/torture`. Some files not not work
because I had to use a slightly newer version of LLVM that seems to
have broken a few things (the originally used version didn't compile on
10.04).
