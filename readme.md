lang2d Readme
=============

lang2d is a draft of an esoteric, stack-based, interpreted, reflective computer programming language. All data is 8 bits. Instructions are single characters, arranged in a grid, through which program execution flows. In many respects, it is very similar to [Befunge](http://en.wikipedia.org/wiki/Befunge). However, it has a non-standard instruction set with extended functionality. While it is reflective, enabling modification of the contents of the program space at runtime, it adds the ability to mutate the topology of the program space itself during runtime. Thus, while Befunge executes a program space topologically equivalent to a torus, lang2d allows the torus to be "warped".

Other languages features:

    * Internationalization
    * Mnemonic (mostly) and logical (mostly) instruction set
    * 2-dimensional program space
    * Not Turing-complete, program space limited to 256x256
