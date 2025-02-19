# Forge

Forge is a collection of code & tools to start my coding adventures.

Initially, I will be writing a language transpiler to convert my invented
language F to C.  This will be used as a basis for all my development in this
library.  This transpiler will be written in C first to generate F0 and then the
main compiler/transpiler will be written in F0.

The bootstrap process is therefore:

- Create F0C by compiling the C codebase.  F0C is a subset of F that matches C.
- Create F0 by compiling the F0 codebase with F0C.
- Create F0 again by compiling the F0 codebase in F0.
- Create F by compiling the F codebase in F0.
- Create F by compiling the F codebase in F.
- 
