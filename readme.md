Xusto
=====

Xusto is a draft of an esoteric, stack-based, interpreted, reflective computer programming language. All data is 8 bits (this is likely to be changed in a future version). Instructions are single characters, arranged in a grid, through which program execution flows. It is a Fungeoid, as the underlying concept of the program space and some of the instruction set is similar to [Befunge](http://en.wikipedia.org/wiki/Befunge). However, Xusto differs in several respects. Xusto uses a different instruction set than Befunge, and extends the typical Funge instruction set with many useful features. Not only is it reflective, but it allows the topology of the program space to be modified at runtime. The program space is best visualized as a plane formed into a donut that can be "warped" or "twisted" in each dimension. This allows for greater code readability and increased program versatility.

#### Feature Summary

* 2-dimensional program space
* Runtime program space warping
* Internationalization
* Mnemonic (mostly) and logical (mostly) instruction set
* Turing-incomplete, program space limited to 256x256
* Portals
* Versatile I/O

### Interpreter

Xusto has been designed to be an interpreted language. A Xusto compiler is possible, but presents an unnecessarily complicated task (due to the reflective nature of Xusto). Currently, Xusto is not recommended for temporally critical applications, and consequently, raw execution speed is not a priority. Therefore, an interpreter proves to be completely sufficient.

An interpreter, written in C and known simply as `xusto`, is under development. `xusto` is developed and built on OS X, but ought to build on most sane platforms.

### Source Files

By convention, source files are named with the suffix `.xs2`. However, this is purely a matter of convention, the interpreter does not trouble itself with extensions. Source files may, optionally, include a header line containing initial values for the interpreter. These header options are documented in greater detail in <b style="color:red;">document not yet created</b>.

### Instruction Set

<b style="color:red;">Instruction set not yet finalized.</b>

### Other Important Things

Xusto is **in development**, the instruction set and core functionality of the language may change without warning. You should not use Xusto for life-, safety-, or mission-critical systems.
