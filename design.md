Xusto Interpreter Design
========================

Language/interpreter design and documentation. Instructions and program idiosyncrasies may change at any time.

#### Instruction Set

All instructions, with associated operation and other notes.


	# VALUES

	0: push 0x00
	1: push 0x01
	2: push 0x02
	3: push 0x03
	4: push 0x04
	5: push 0x05
	6: push 0x06
	7: push 0x07
	8: push 0x08
	9: push 0x09
	a: push 0x0a
	b: push 0x0b
	c: push 0x0c
	d: push 0x0d
	e: push 0x0e
	f: push 0x0f

	# MATH OPERATORS

	+: pop a, b; push b+a
	-: pop a, b; push b-a
	*: pop a, b; push b*a
	/: pop a, b; push b/a
	%: pop a, b; push b%a

	# BINARY & LOGICAL OPERATORS

	&: pop a, b; push bitwise b AND a
	|: pop a, b; push bitwise b OR a
	r: pop a, b; push bitwise b XOR a
	L: pop a, b; push bitwise b << a
	R: pop a, b; push bitwise b >> a
	~: pop a; push bitwise NOT of a
	!: pop a; push logical NOT of a

	# COMPARISON OPERATORS

	G: pop a, b; push 1 if b GT a, else push 0
	=: pop a, b; push a==b

	# FLOW CONTROL & BRANCHING

	<: set ivec to [255,0]
	^: set ivec to [0,255]
	>: set ivec to [1,0]
	v: set ivec to [0,1]
	x: pop x, set ivec to [x,_]
	y: pop y, set ivec to [_,y]
	B: negates ivec (bounce)
	_: increment iptr by ivdc (teleport)
	T: pop a; set ivec < if 0 else set ivec > (right/left branch)
	K: pop a; set ivec ^ if 0 else set ivec v (up/down branch)
	Q: 50% chance of teleport

	# STACK MANIPULATION

	S: pop a, b; push a, b (swap)
	P: pop a
	D: peek a; push a (duplicate)

	# EXECUTION CONTROL

	 : nop
	H: toggle EXECUTE flag (halt)
	n: push the phase of the moon [0-29]
	l: pop x; sleep for x pico centuries (3156µs, or ~π ms)

	# INPUT/OUTPUT

	i: read in integer i; push i
	s: read in char c; push c
	[: pop i; print i as integer
	]: pop c; print c as ASCII character
	{: peek i; print i as integer
	}: peek c; print c as ASCII character
	': lazy print. Pop c and print c as ASCII until a 0 is popped
	W: whacks the interpreter, causing it to say "Ouch!"

	# PROGRAM SPACE MODIFICATION

	m: pop x, y, v; write v to [x,y] (mutate)
	g: pop x, y; push value at [x,y] (get)

	# OTHER STUFF

	E: pop a; execute a ("execute") (! Unimplemented !)
	`: pop y, x; set warp to [x,y]
	#: store iptr (drop portal)
	@: set iptr stored value (use portal)
	": toggle PUSHCHAR flag
	?: toggle DEBUG flag

**Notes**

x/0, x%0 are math exceptions  
executing a non-existent instruction will throw an error, but will not halt execution  
arithmetic overflows wrap around and are silent  
program header is optional  
if no program header is found, iptr = [0,0] and ivec = [1,0]  
dimensions are implicit from maximum line lengths if not specified  
popping an empty stack returns 0
\ is a special character, indicating the program header  
Program space is 64-bit  
When printing cells of the program space as characters, the least significant byte is displayed. Similarly, when echoing program space in debug mode, values are truncated to the 8 LSBs.

#### Flags

EXECUTE: if set, the interpreter will continue to execute instructions. If unset, the interpreter will halt.

PUSHCHAR: if set, each character encountered by the instruction pointer will be pushed to the stack instead of interpreted as an instruction, unless is it the character '"'.

DEBUG: if set, debug messages will be printed for significant interpreter actions and halting execution will show a stack dump.

#### Header

The general format of the header is as follows:

	\<token>:<value>/<token>:<value>/.../

where token is a token as listed below and value is a value in hex. For example:

	\f:0x81/wx:0x03/

sets the flags to the hex value of 0x81 (DEBUG and EXECUTE set), and also sets the warp in the x dimension to 3.

**Tokens:**

	f:  flags (see below for bits)
	wx: warp x
	wy: warp y
	lx: portal x
	ly: portal y
	vx: ivec x
	vy: ivec y
	px: iptr x
	py: iptr y
	sx: pgmsize x
	sy: pgmsize y

The flags token allows the internal `flags` register to be set directly. The bits of the flags register are as follows:

| Bit | Mask | Name      | Function                                        | Default |
|-----|:----:|-----------|-------------------------------------------------|---------|
| 0   | 0x01 | EXECUTE   | While set, program execution continues          | 1       |
| 1   | 0x02 | PUSHCHAR  | While set, characters are pushed onto the stack | 0       |
| 2   | 0x04 | unused    | --currently unused--                            | 0       |
| 3   | 0x08 | unused    | --currently unused--                            | 0       |
| 4   | 0x10 | unused    | --currently unused--                            | 0       |
| 5   | 0x20 | EXCEPTION | Set internally when an exception occurs         | 0       |
| 6   | 0x40 | VERBOSE   | While set, extra messages are printed           | 0       |
| 7   | 0x80 | DEBUG     | While set, even more extra messages are printed | 0       |

Note that clearing EXECUTE in the header is allowed, it is rather pointless.

#### Miscellaneous

* Source files _must_ end with a newline. Ignoring this will cause the interpreter ignore the last line of source.
* There are likely issues with wrapping and branching when the instruction vector is not a unit vector (a magnitude greater than 1).
