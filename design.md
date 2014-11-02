Xusto Interpreter Design
========================

Assuming an interpreter implementation in a C-style language.

#### Flags

STATE_F_EXECUTE: if set, the interpreter will continue to execute instructions. If unset, the interpreter will halt.

STATE_F_PUSHCHAR: if set, each character encountered by the instruction pointer will be pushed to the stack instead of interpreted as an instruction, unless is it the character '"'.

STATE_F_DEBUG: if set, debug messages will be printed for significant interpreter actions.

#### Instruction Set

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
	B: negates ivec ("reflect" or "bounce")
	_: increment iptr by ivdc ("teleport")
	T: pop a; set ivec < if 0 else set ivec >
	K: pop a; set ivec ^ if 0 else set ivec v
	Q: 50% chance of teleport

	# STACK MANIPULATION

	S: pop a, b; push a, b ("swap")
	P: pop a ("pop")
	D: peek a; push a ("duplicate")

	# EXECUTION CONTROL

	 : nop
	H: toggle interpreter execution state ("halt")
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

	m: pop x, y, v; write v to [x,y] ("mutate")
	g: pop x, y; push value at [x,y] ("get")

	# OTHER STUFF

	E: pop a; execute a ("execute")
	`: pop y, x; set warp to [x,y]
	#: store iptr to prtl ("drop portal")
	@: set iptr to prtl ("use portal")
	": set/unset pushchar flag
	?: set/unset debug flag

x/0, x%0 are math exceptions  
executing a non-existent instruction is an exception  
arithmetic overflows wrap around and are silent  
program header is optional  
program header specifies iptr start, ivec start, dimensions, and warp settings  
if no program header is found, iptr = [0,0] and ivec = [1,0]  
dimensions are implicit from maximum line lengths if not specified  
popping an empty stack is an exception  
\ is a special character, indicating the program header  

**Header format**

	\f:0/wx:0/wy:0/bx:0/by:0/vx:0/vy:0/px:0/py:0/sx:0/sy:0/

A file is composed of token:value pairs (separated by colors). Each pair is separated by a forward slash. Tokens are as follows:

	f:  flags
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

#### Misc

Note that source files _must_ end with a newline. Just like any sane *nix thingy. Ignoring this will have nasty consequences, typically involving the last line of source being ignored by the interpreter.
