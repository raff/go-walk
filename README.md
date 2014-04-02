walkngo
=======

A "walker" for Go AST

This is the "skeleton" for something that can parse a Go source file and print it in some other format or, in any case, a way to understand how Go AST works.

The walker package contains the AST walker/visitor, the printer package contains modules that can "print out" the tree as different (probably broken) languages.

The "GoPrinter" module generates a Go source that compile and should work just as good as the original. The "CPrinter" module tries to convert the Go source file to C (actually C++)

The main program accepts a -c boolean argument to select the output (c=true for C++, false for Go)

    walkngo -c walkngo.go

Usage:
======

    walkngo [--c] [--debug] [--debug-printer] [--outdir={output-folder}] file.go|folder

Where:
    --c : convert to C/C++ (or re-generate a "Go" file if --c==false or if the option is missing)
    --debug : print out AST nodes for debugging
    --debug-printer : print out calls to Printer methods
    --outdir={output-folder} : creates output files in output-folder following original paths

If a folder is specified as input, the program will "walk" the directory structure and convert all files with extension ".go" (it skips folders with name starting with ".")

Notes:
======

When running with "go run" you should use:

    go run walkngo.go -- go-source-file.go
    
The '--' tell 'go run' to stop looking for go files, otherwise it gets utterly confused.

If you build it you can just run:

    walkngo go-source-file.go


Runtime:
========
The "runtime" folder contains the implementation of some Go runtime and common modules that the language translator
can call.

For C++ there is some support for goroutines (via C++11 threads) and channels (C++11 queue, mutex, condition variables) and some initial implementations of the fmt, time and sync modules.

Also, multiple initializations and multiple return values are implemented using C++11 tuples (make_tuple and tie).

Note that the current implementation is very basic, just to verify that things work more or less as expected.

TODO:
=====
Variable initialization: in go all variables are initizialized to their "zero value". In C/C++ they are whatever they are.

Module initialization: in go each module/file can have an init() method, that is called when the module is imported.

defer calls: In go the "parameter" of the defer statement is called before returning from the current method. This could be implemented with a Defer object that takes the function/block as input and executes it in its destructor... maybe.

recover: panic is currently implemented as a method that causes a NPE. It should be implemented as a method throwing a Panic exception and the recover method can catch it (Would it work with defer ?).
* named return values: right now the name in the method declaration is commented out so that it doesn't generate an error.It should be possible to add these as variable inside the body, so that they can be properly referenced, and then make sure that a return with no parameters is changed to a return with those variables.
* range statement: a range statement with '_, x' should be converted into a for statement and array dereferencing.  Not sure there is a way to correctly distinguish between a range on an array vs. a range on a map (or a range on a channel).
* select on channels: not sure of what to do here.
