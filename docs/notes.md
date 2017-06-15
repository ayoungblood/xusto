Notes and Ideas
===============

**Preprocessor/Parser**

The preprocessor/parser loads instructions from the source file into the execution space: the source files (regular text files encoded with the UTF-8) are one-dimensional byte streams, typically displayed as two-dimensional plaintext, but must be squished into a three-dimensional execution space. Care is taken to ensure that a typical text file will map intuitively into a 2D plane matching its appearance in a text editor, and additional characters may be used to place instructions at arbitrary locations in the 3D execution space.

This is done by reserving certain characters for parser instruction. The first 32 code points in UTF-8 (which are identical to the first 32 ASCII characters, and are non-printable) are reserved as parser instructions. The parser instruction mapping is designed to be as logical as possible, letting the control chars move a "virtual cursor" (a write pointer) through the execution space during parsing in a way that matches the behavior of a plaintext file.

As the parser instructions refer to moving through 3D space, the instruction descriptions use a consistent convention when referring to movement through 3D space. The space is modeled with X,Y,Z coordinates. Relative to a computer screen, X may be thought of as "left/right", Y is "up/down" and Z is "into/out":

- +x: right
- -x: left
- +y: down
- -y: up
- +z: into
- -z: out

The slightly annoying consequence is that programs with very complex parser instructions may require editing or inspection via a hex editor or modified text editor due to the quantity of non-printable characters. Hopefully, one day there will be custom-built editor for Xusto that makes this easier.

**Instruction decoding**

Because the instruction space is very large (UTF-8 encodes 1,112,064 valid code points), and consequently only a small number of code points map to valid instructions, an array is not a satisfactory strategy for instruction decoding. Nor is a case statement (as was used by the original incarnation of Xusto). Additionally, as we eventually would like to support dynamic instruction remapping, we need to be able to dynamically assign instructions or macros of instructions to arbitrary code points. A hash table of "executable things" (a combination of function pointers for native instructions and microprograms for macros) seems to be a reasonable solution. The code point value indexes into the hash table, and the "executable thing" stored in the hash table, if it exists, is executed. The overhead should be minimal. Eventually, instructions that can reassign and populate the hash table with "executable things" should make the whole thing dynamic.

**Space implementation**

In order to store a three-dimensional instruction space that may be very large in one or more dimensions without an extreme memory cost, we map a small space near the origin directly to a 3D array: accesses near the origin are very efficient. Accesses outside of this locality are instead directed to a hash table that can store cell values at a distance of up to the integer limit of the machine architecture. This all happens regardless of the current bounds of execution space, which may change at runtime (if the program decides to grow or shrink its hyperdonut). This prevents the interpreter from wasting a large quantity of memory allocating a larger and larger cube: the hash table can more easily handle the irregular access patterns that may arise.

**Instruction wish list**

- Instructions for setting/getting flags
- File I/O
- stdin/stdout
- Floating point support?

**Project wish list**

- ~~Add CI~~
- ~~Add some semblance of unit testing~~
- Bounded vs. bounded space access functions: unbounded for parsing, bounded for execution
- More flexible/complicated type support
- Glossary of terms

**Types**

`vector3_t`: a point or dimension in 3D space  
`cell_t`: element of an execution space  
`space_t`: represents an execution space  

@TODO

`stack_t`: stack type and wrappers for program execution
`state_t`: program state type

- state: stores the execution state
    - instruction_pointer: current location in the program space
        - `vector3_t`
    - instruction_vector: direction of instruction pointer movement
        - `vector3_t`
    - stack: the stack on which the execution is happening on
        - `stack_t`
    - portal: stores the portal location
        - `vector3_t`
    - warp: stores the execution space warp coefficients
        - `vector3_t`
    - flags: boolean flag array
        - `flags_t`

- Parser: given a filename, returns a Cube

- executor: given a execution space and a state, runs the execution loop
    - state
    - space

This should allow multiple executors (threads), see below.

**Threading ideas**

An executor manages the execution loop (does the interpreting), and has:  
- pointer to a state
- pointer to a cube (the execution space)

Eventually, executors can be threaded, so that an instruction causes an executor to spawn another executor, copying state (probably duplicated the stack, but maybe not?), changing the instruction_vector (so the thread goes off in a new direction) and using the same space reference. Another instruction will cause the executor to terminate. By spinning on memory locations, executors can be synchronized. There could also be an instruction for explictly blocking/synchronizing executors.

### References

**Character sets and character encoding**

[Wikipedia: Unicode](https://en.wikipedia.org/wiki/Unicode)  
[Wikipedia: Plane (Unicode)](https://en.wikipedia.org/wiki/Plane_(Unicode))  
[Wikipedia: List of Unicode Characters](https://en.wikipedia.org/wiki/List_of_Unicode_characters)  
[Wikipedia: UTF-8](https://en.wikipedia.org/wiki/UTF-8)  
[Wikipedia: C0 and C1 control codes](https://en.wikipedia.org/wiki/C0_and_C1_control_codes)  
[Wikipedia: Control character](https://en.wikipedia.org/wiki/Control_character)  
[Wikipedia: ANSI escape code](https://en.wikipedia.org/wiki/ANSI_escape_code)  
[Wikipedia: List of ISO 639-1 codes](https://en.wikipedia.org/wiki/List_of_ISO_639-1_codes)  

**Funges**

[Funge-98 Final Specification](http://quadium.net/funge/spec98.html)  
[Concurrent Funge-98](http://quadium.net/funge/spec98.html#Concurrent)  
[Fish (><>)](https://esolangs.org/wiki/Fish), see also [Starfish (*><>)](https://esolangs.org/wiki/Starfish)  
[Trefunge-98](http://catseye.tc/node/Trefunge-98)  

**Other languages**

[05AB1E](https://github.com/Adriandmen/05AB1E) (large instruction set)  
[Pyth](https://pyth.readthedocs.io/en/latest/) (compiles to Python)  
[J](https://en.wikipedia.org/wiki/J_(programming_language))  
[APL](https://en.wikipedia.org/wiki/APL_(programming_language)), see also [APL syntax and symbols](https://en.wikipedia.org/wiki/APL_syntax_and_symbols)  
[GolfScript](https://esolangs.org/wiki/GolfScript)  
[CJAM](https://sourceforge.net/p/cjam/wiki/Home/), see also [CJAM syntax cheat sheet](https://dl.dropboxusercontent.com/u/15495351/cjam.pdf)  
[Z80 Assembly](http://sgate.emt.bme.hu/patai/publications/z80guide/), see also [Z80 Instructions](http://sgate.emt.bme.hu/patai/publications/z80guide/app1.html)  
[6502 Assembly](https://en.wikibooks.org/wiki/6502_Assembly), see also [Easy 6502](https://skilldrick.github.io/easy6502/) and [6502.org](http://www.6502.org/)  
[AVR Instruction Set](http://www.atmel.com/images/Atmel-0856-AVR-Instruction-Set-Manual.pdf)  
[Nios II Instruction Set](https://www.altera.com/content/dam/altera-www/global/en_US/pdfs/literature/hb/nios2/n2cpu_nii51017.pdf)  
[MIPS Instruction Set](https://en.wikipedia.org/wiki/MIPS_instruction_set)  
[x86 Assembly](https://en.wikipedia.org/wiki/X86_assembly_language)  
[PicoBlaze Instruction Set](https://www.xilinx.com/support/documentation/ip_documentation/ug129.pdf)  
[FALSE](https://esolangs.org/wiki/FALSE)  
[Esolangs: Two-dimensional languages](https://esolangs.org/wiki/Category:Two-dimensional_languages)  

**Turing-Completeness**

[Brainfuck](https://esolangs.org/wiki/Brainfuck) - Tape and 8 instructions  
[1L](https://esolangs.org/wiki/1L) - Two-dimensional with 2 instructions  
[Minimal-2D](https://esolangs.org/wiki/Minimal-2D) - Two-dimensional with 11 instructions  
[Swordfish](https://esolangs.org/wiki/Swordfish) - Two-dimensional, a reduced version of ><>  
[Nopfunge](https://esolangs.org/wiki/Nopfunge) - Two-dimensional, degenerate reduction of Befunge

**Neat but not really related/relevant esoteric languages**

[MATL: MATLAB/Octave-based golfing language](https://github.com/lmendo/MATL)  
[V: vim-based golfing language](https://github.com/DJMcMayhem/V)  

### Miscellany

This applies to Xusto quite well:

> #310194
> <madk> The interpreter uses an unbounded tape size, but due to technical limitations will stop being unbounded if the tape size reaches 2^63 cells.

A poem, regarding the pronunciation of certain characters:

```
< > ! * ' ' #
^ " ` $ $ -
! * = @ $ _
% * < > ~ # 4
& [ ] . . /
| { , , SYSTEM HALTED

- Fred Bremmer & Steve Kroese
```

Translated

> Waka waka bang splat tick tick hash,  
> Caret quote back-tick dollar dollar dash,  
> Bang splat equal at dollar under-score,  
> Percent splat waka waka tilde number four,  
> Ampersand bracket bracket dot dot slash,  
> Vertical-bar curly-bracket comma comma CRASH!  
