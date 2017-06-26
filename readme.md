Xusto
=====

branch `master`: ![master build status](https://api.travis-ci.org/ayoungblood/xusto.svg?branch=master)
[![codecov](https://codecov.io/gh/ayoungblood/xusto/branch/master/graph/badge.svg)](https://codecov.io/gh/ayoungblood/xusto)  
branch `develop`: ![develop build status](https://api.travis-ci.org/ayoungblood/xusto.svg?branch=develop)
[![codecov](https://codecov.io/gh/ayoungblood/xusto/branch/develop/graph/badge.svg)](https://codecov.io/gh/ayoungblood/xusto/branch/develop)

Xusto is an esoteric, stack-based, interpreted (thus far), reflective, computer programming language. This repository contains both the language specification and an interpreter written in C.

Instructions are arranged in a three-dimensional cube, through which program execution flows. Xusto is a Fungeoid, as the underlying concept of the program space and some of the instruction set is similar [Befunge](http://en.wikipedia.org/wiki/Befunge): text files representing an array of instructions are parsed into an execution space and a instruction pointer moves through the space to execute the program. However, Xusto differs in several respects. Xusto uses a different and _much_ larger instruction set than Befunge, and extends the typical Funge instruction set with many higher level instructions, similar to some golfing languages. Not only is it reflective, but it allows the topology of program space to be modified at runtime. Neglecting the third dimension, the program space may be visualized as a plane formed into a donut that can be "warped" in each dimension. In three dimensions, the program space is a sort of twisted "hyperdonut". Finally, the "density" of the donut or hyperdonut may also be modulated at runtime, leading to all sorts of strange instruction pointer behavior. The program space is bounded, but the bound may also be modified at runtime: the hyperdonut can grow or shrink throughout the program's lifetime.

Extending the typical Funge behavior, sources files are UTF-8 encoded, and any valid UTF-8 codepoint may represent an instruction. This allows for a truly overwhelming number of instructions: in instruction set capabilities, Xusto is to Befunge what x86 is to the 6502 instruction set. A major upgrade from the initial draft of Xusto, each "cell" in the space is 64 bits, allowing the interpreter to handle very large numbers and very large spaces (essentially, limited only by available memory of the machine).

#### Feature Summary

* Three-dimensional program space
* Runtime program space configuration (warping, bounds, and density)
* Flexible instruction pointer movement
* Internationalization
* Capable (mostly) and logical (somewhat) instruction set
* Turing-complete (supposedly), program space only limited by machine architecture and available memory
* Emoji support

### Interpreter

Xusto has been designed to be an interpreted language. A Xusto compiler is possible, but presents an unnecessarily complicated task (due to the reflective nature of Xusto). Currently, Xusto is not recommended for temporally critical applications, and consequently, raw execution speed is not a priority. Therefore, an interpreter proves to be completely sufficient.

An interpreter, written in C and known simply as `xusto`, is under development. `xusto` is developed and built on OS X, but ought to build on most sane platforms.

### Source Files

By convention, source files are named with the suffix `.xs2` for two-dimensional programs, and `.xs3` for three-dimensional programs. However, this is purely a matter of convention, the interpreter does not trouble itself with extensions. Source files _must_ be UTF-8 encoded (ASCII is a perfectly valid subset of UTF-8 and may be used as well).

### Instruction Set

<b style="color:red;">Instruction set not yet finalized.</b> See [docs/syntax-draft.txt](docs/syntax-draft.txt) for now.

### Other Important Things

Xusto is **in development**, the instruction set and core functionality of the language may change without warning. You should not use Xusto for life-, safety-, or mission-critical systems.
