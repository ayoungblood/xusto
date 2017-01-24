Notes and Ideas
===============

**Preprocessor/Parser**

We need a preprocessor/parser to load code into the execution space: the 2D nature of a plaintext file (source code) must be smooshed into the 3D execution space.

This is done by reserving certain characters for parser instruction. The first 32 code points in UTF-8 (which are identical to the first 32 codes of ASCII) are reserved as parser instructions. The parser instruction mapping is designed to be as logical as possible, letting the control chars move a "virtual cursor" through the execution space during parsing.

**Instruction decoding**

Because the instruction space is very large (UTF-8 encodes 1,112,064 valid code points), and consequently only a small number of code points map to valid instructions, an array is not a satisfactory strategy for instruction decoding. Nor is a case statement (as was used by the original incarnation of Xusto). Additionally, as we eventually would like to support dynamic instruction remapping, we need to be able to dynamically assign instructions or macros of instructions to arbitrary code points. A hash table of "executable things" (a combination of function pointers for native instructions and microprograms for macros) seems to be a reasonable solution. The code point value indexes into the hash table, and the "executable thing" stored in the hash table, if it exists, is executed. The overhead should be minimal. Eventually, instructions that can reassign and populate the hash table with "executable things" should make the whole thing dynamic.

**Instruction wish list**

- Instructions to set and get flags
- File I/O
- stdin/stdout
- Floating point support?
- More flexible type support?

**Project wish list**

- Add CI
- Add some semblance of integration testing

**Object-oriented nature**

`space_t`: signed type able to store a dimension/size/delta

- Cube: represents an execution space

- Stack: provides a stack for program execution

- State: stores the execution state
    - InstructionPointer
        - `space_t[3]`
    - InstructionVector
        - `space_t[3]`
    - Stack (ref)
    - Portal: stores the portal location
        - `space_t[3]`
    - Warp: stores the execution space warp coefficients
        - `space_t[3]`
    - Flags: boolean flag array

- Parser: given a filename, returns a Cube

- Executor: given a execution space and a state, runs the execution loop
    - State (ref)
    - Cube (ref)

**Threading ideas**

An Executor manages the execution loop (does the interpreting), and has:  
- Pointer to a State
- Pointer to a Cube (the execution space)

Eventually, Executors can be threaded, so that an instruction causes an Executor to spawn another Executor, copying State (should stack be duplicated? not sure), changing the InstructionVector and using the same Cube reference. Another instruction will cause the Executor to terminate. By spinning on memory locations, Executors can be synchronized.

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
[Z80 Assembly](http://sgate.emt.bme.hu/patai/publications/z80guide/), see also [Z80 Instructions](http://sgate.emt.bme.hu/patai/publications/z80guide/app1.html)  
[6502 Assembly](https://en.wikibooks.org/wiki/6502_Assembly), see also [Easy 6502](https://skilldrick.github.io/easy6502/) and [6502.org](http://www.6502.org/)  
[AVR Instruction Set](http://www.atmel.com/images/Atmel-0856-AVR-Instruction-Set-Manual.pdf)  
[Nios II Instruction Set](https://www.altera.com/content/dam/altera-www/global/en_US/pdfs/literature/hb/nios2/n2cpu_nii51017.pdf)  
[MIPS Instruction Set](https://en.wikipedia.org/wiki/MIPS_instruction_set)  
[x86 Assembly](https://en.wikipedia.org/wiki/X86_assembly_language)  
[PicoBlaze Instruction Set](https://www.xilinx.com/support/documentation/ip_documentation/ug129.pdf)  
[FALSE](https://esolangs.org/wiki/FALSE)  
[Esolangs: Two-dimensional languages](https://esolangs.org/wiki/Category:Two-dimensional_languages)  

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
