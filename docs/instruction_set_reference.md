# Xusto Instruction Set Reference Manual

## Conventions

### Registers

* `ip`: **i**nstruction **p**ointer, stores a vector describing the current instruction to be executed
* `iv`: **i**nstruction **v**ector, stores a vector describing the advancement of the instruction pointer

### Vectors

Vectors are notated as "(x,y,z)" and the coordinate system is right-handed (+z points _toward_ the observer).

----

## Instructions grouped by function

### Core

#### Constants

| Symbol | Code   | Instruction |
|--------|--------|-------------|
| 0      | U+0030 | push 0x0 |
| 1      | U+0031 | push 0x1 |
| 2      | U+0032 | push 0x2 |
| 3      | U+0033 | push 0x3 |
| 4      | U+0034 | push 0x4 |
| 5      | U+0035 | push 0x5 |
| 6      | U+0036 | push 0x6 |
| 7      | U+0037 | push 0x7 |
| 8      | U+0038 | push 0x8 |
| 9      | U+0039 | push 0x9 |
| a      | U+0061 | push 0xa |
| b      | U+0062 | push 0xb |
| c      | U+0063 | push 0xc |
| d      | U+0064 | push 0xd |
| e      | U+0065 | push 0xe |
| f      | U+0066 | push 0xf |

#### Register control

| Symbol | Code   | Instruction |
|--------|--------|-------------|
| <      | U+003C | left: set `iv` = (-1,0,0) |
| >      | U+003E | right: set `iv` = (+1,0,0) |
| ^      | U+005E | up: set `iv` = (0,+1,0) |
| v      | U+0076 | down: set `iv` = (0,-1,0) |
| .      | U+002E | toward: set `iv` = (0,0,+1) |
| o      | U+006F | away: set `iv` = (0,0,-1) |
| J      | U+004A | jump: pop x,y,z; set `ip` = (x,y,z) |

trampoline, conditionals

#### Stack operations

| Symbol | Code   | Instruction |
|--------|--------|-------------|
| p      | U+0070 | pop: pop a |
| \      | U+005C | swap: pop a,b; push a,b |
| :      | U+003A | duplicate: peek a; push a |
| F      | U+0046 | flip: pop a,b,c; push a,b,c |
| U      | U+0055 | unearth: pop n; push n-th stack item |

#### Arithmetic operations (integer)

| Symbol | Code   | Instruction |
|--------|--------|-------------|
| +      | U+002B | add: pop a,b; push (b + a) |
| -      | U+002D | subtract: pop a,b; push (b - a) |
| *      | U+002A | multiply: pop a,b; push (b * a) |
| /      | U+002F | divide: pop a,b; push (b / a) |
| %      | U+0025 | modulo: pop a,b; push (b % a) |

#### Bitwise operations

| Symbol | Code   | Instruction |
|--------|--------|-------------|
| &      | U+0026 | bitwise AND: pop a,b; push (b AND a) |
| |      | U+007C | bitwise OR: pop a,b; push (b OR a) |
|        | U+???? | bitwise XOR: pop a,b; push (b XOR a) |
| ~      | U+007E | bitwise NOT: pop a; push (NOT a) |
| «      | U+00AB | bitwise left shift: pop a,b; push (b LSHIFT a) |
| »      | U+00BB | bitwise right shift: pop a,b; push (b RSHIFT a) |

#### Logical operations

| Symbol | Code   | Instruction |
|--------|--------|-------------|
| !      | U+0021 | logical NOT: pop a; push 1 if 0 == a, 0 otherwise |

#### Input/output operations

| Symbol | Code   | Instruction |
|--------|--------|-------------|
| [      | U+005B | print integer: pop i; print i as an integer |
| ]      | U+005D | print character: pop c; print c as a Unicode character |
| '      | U+0027 | lazy print: pop a; print as Unicode; loop until 0 == a |

#### Program space access

| Symbol | Code   | Instruction |
|--------|--------|-------------|
| G      | U+0047 | get: pop x,y,z; push space(x,y,z) |
| S      | U+0053 | set: pop x,y,z,a; set space(x,y,z) = a |
| R      | U+0052 | jump subroutine: pop x,y,z; push ip.x, ip.y, ip.z; set ip = (x,y,z) |

#### Miscellaneous

| Symbol | Code   | Instruction |
|--------|--------|-------------|
| SPACE  | U+0020 | no-op |
| "      | U+0022 | pushchar: toggle PUSHCHAR mode |
| H      | U+0048 | halt: pop a; halt program execution and exit returning a |

### Debug

* Halt and dump stack and registers
* Toggle debug flag

### Thread

### Float

----

## Instructions ordered by Unicode code point

| Code   | Symbol | Instruction |
|--------|--------|-------------|
| U+0020 | SPACE  | no-op |
| U+0021 |        |  |
| U+0022 |        |  |
| U+0023 |        |  |
| U+0024 |        |  |
| U+0025 |        |  |
| U+0026 |        |  |
| U+0027 |        |  |
| U+0028 |        |  |
| U+0029 |        |  |
| U+002A |        |  |
| U+002B |        |  |
| U+002C |        |  |
| U+002D |        |  |
| U+002E |        |  |
| U+002F |        |  |
| U+0030 |        |  |
| U+0031 |        |  |
| U+0032 |        |  |
| U+0033 |        |  |
| U+0034 |        |  |
| U+0035 |        |  |
| U+0036 |        |  |
| U+0037 |        |  |
| U+0038 |        |  |
| U+0039 |        |  |
| U+003A |        |  |
| U+003B |        |  |
| U+003C |        |  |
| U+003D |        |  |
| U+003E |        |  |
| U+003F |        |  |
| U+0040 |        |  |
| U+0041 |        |  |
| U+0042 |        |  |
| U+0043 |        |  |
| U+0044 |        |  |
| U+0045 |        |  |
| U+0046 |        |  |
| U+0047 |        |  |
| U+0048 |        |  |
| U+0049 |        |  |
| U+004A |        |  |
| U+004B |        |  |
| U+004C |        |  |
| U+004D |        |  |
| U+004E |        |  |
| U+004F |        |  |
| U+0050 |        |  |
| U+0051 |        |  |
| U+0052 |        |  |
| U+0053 |        |  |
| U+0054 |        |  |
| U+0055 |        |  |
| U+0056 |        |  |
| U+0057 |        |  |
| U+0058 |        |  |
| U+0059 |        |  |
| U+005A |        |  |
| U+005B |        |  |
| U+005C |        |  |
| U+005D |        |  |
| U+005E |        |  |
| U+005F |        |  |
| U+0060 |        |  |
| U+0061 |        |  |
| U+0062 |        |  |
| U+0063 |        |  |
| U+0064 |        |  |
| U+0065 |        |  |
| U+0066 |        |  |
| U+0067 |        |  |
| U+0068 |        |  |
| U+0069 |        |  |
| U+006A |        |  |
| U+006B |        |  |
| U+006C |        |  |
| U+006D |        |  |
| U+006E |        |  |
| U+006F |        |  |
| U+0070 |        |  |
| U+0071 |        |  |
| U+0072 |        |  |
| U+0073 |        |  |
| U+0074 |        |  |
| U+0075 |        |  |
| U+0076 |        |  |
| U+0077 |        |  |
| U+0078 |        |  |
| U+0079 |        |  |
| U+007A |        |  |
| U+007B |        |  |
| U+007C |        |  |
| U+007D |        |  |
| U+007E |        |  |
| U+007F |        |  |
