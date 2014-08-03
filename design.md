lang2d Design
=============

Assuming an interpreter implementation in a C-style language.

Interpreter vars

	uint8_t iptr[2]; // instruction pointer, [x,y]
	uint8_t ivec[2]; // instruction vector, [x,y]
	uint8_t bcon[2]; // beacon position [x,y]
	uint8_t warp[2]; // space warp [x,y]
	uint8_t flags; // state flags
	uint8_t pgm[256][256]; // program space
	uint8_t* stack; // stack pointer, dynamic array
	uint8_t heap[256]; // heap vector

Flags
	
	#define F_EXECUTE  0x01
	#define F_PUSHCHAR 0x02
	
	#define F_DEBUG    0x80
	
Structure

	initialize all vars
	read program data into mem
	set execute flag
	while (execute)
		check flags
		handle instruction
		increment iptr
		check warp
	cleanup
	
#### Instruction Set

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
	+: pop a, b; push b+a
	-: pop a, b; push b-a
	*: pop a, b; push b*a
	/: pop a, b; push b/a
	%: pop a, b; push b%a
	&: pop a, b; push bitwise b AND a
	|: pop a, b; push bitwise b OR a
	r: pop a, b; push bitwise b XOR a
	L: pop a, b; push bitwise b << a
	R: pop a, b; push bitwise b >> a
	~: pop a; push bitwise NOT of a
	!: pop a; push logical NOT of a
	G: pop a, b; push 1 if b GT a, else push 0
	L: pop a, b; push 1 if b LT a, else push 0
	=: pop a, b; push a==b
	<: set ivec to [255,0]
	^: set ivec to [0,255]
	>: set ivec to [1,0]
	v: set ivec to [0,1]
	x: pop x, set ivec to [x,_]
	y: pop y, set ivec to [_,y]
	B: negates ivec ("reflect")
	S: pop a, b; push a, b ("swap")
	 : nop
	m: wait 1sec + 1sec * (phase of moon as %)
	H: set ivec to [0,0] ("Halt, Hammerzeit")
	_: increment iptr by ivdc ("teleport")
	T: pop a; set ivec < if 0 else set ivec >
	K: pop a; set ivec ^ if 0 else set ivec v
	Q: 50% chance of teleport
	D: peek a; push a ("duplicate")
	i: read in integer i; push i
	s: read in char c; push c
	[: pop i; print i as integer
	]: pop c; print c as ASCII character
	{: peek i; print i as integer
	}: peek c; print c as ASCII character
	w: pop v, y, x; write v to [x,y] ("write")
	r: pop y, x; push value at [x,y] ("read")
	E: pop a; execute a ("execute")
	`: pop y, x; set warp to [x,y]
	p: pop a
	#: store iptr to bcon ("store beacon")
	@: set iptr to bcon ("return to beacon")
	': set/unset pushchar flag
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

