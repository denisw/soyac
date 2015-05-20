About Soya
==========
Soya is a general-purpose, object-oriented, safe programming language. It is
designed to be simple, consistent and easy to use, while providing the power
and safety to develop both low-level performance-critical systems and
high-level applications of any size.

Elaboration on the terms used in the description (braindump):

* general-purpose: not tailored towards a specific problem domain
* object-oriented: "everything is an object"
* safe: designed for operation safety, e.g. type checking, array
  bounds checking, exceptions
* simple: minimal amount of concepts, small language core
* consistent: minimal amount of exceptions to concepts, orthogonal where possible
* easy to use: convenient to program in, nice learning curve, convenient to read
* low-level: ability to program verything down to operating systems
* performance-critical: ability to use a subset of Soya to archieve C-like
  performance (procedural subset mostly)
* high-level applications: ability to conveniently program on the application
  level
* of any size: scalable to allowi the easy development of both small
  prototypes and large, well-maintainable applications / libraries

Getting Started
===============

This section describes the installation on Unix-like systems.

First, make sure you have the following dependencies installed:

* cmake >= 2.4.7
* boost >= 1.33 (regex, options)
* LLVM >= 2.5
* libpopt >= 1.14
* libsigc++ >= 2.0
* bison >= 2.4.2
* libgc (Boehm garbage collector)
* gcc (needed for linking, FIXME: make this dependency go away)

Then, change to the soyac directory and type the following:

 cmake .
 make
 make install

On Linux, you might have to run "ldconfig" as root afterwards.

The soyac binary will now be located in /usr/local/bin.
The Soya runtime library (libsr) will be in /usr/local/lib.
One Soya interface file (__core__.soya) will be installed in /usr/local/include.

To build the test suite, change to the test/ directory and type "make". The
suite depends on cppunit being installed. You can then run the tests with
"./test-soyac". You must be in the test/ directory for this to work.

Documentation
=============

The doc/ folder contains the beginning of a first draft of the Soya Language
Reference. Note that the reference might be outdated at places, the language
has evolved since these documents were written.

If you have doxygen installed, you can generate a reference of the internal
API of the compiler. Just run "doxygen" in the soyac directory.

