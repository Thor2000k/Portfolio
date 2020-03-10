Authors:
Group 9
Bjørn Glue Hansen (bhans09)
Thor Skjold Haagensen (thhaa16)
Anthon Nørgaard (antno16)

Reads kitty source file from stdin and outputs 64 bit gas assembler in stdout.
It is expected that the assembler file is compiled using gcc.

Options
    -d      Print call stack during typechecking.
    -dl     Print intermediate language including liveness.
    -pi     Print pretty version of intermediate after code generation.
    -pp     Print pretty version of source after scanning phase.
    -pt     Print pretty version of source after type checking.
    -pw     Print pretty version of source after weeding phase.
    -nc     No runtime checks. Makes debugging harder, but increases performance.
    -np     Don't print current phase beeing worked on.
    -nph    Don't perform peephole optimization.
    -ss     Simplified static link assignment. Some programs will not function, 
            but increases performance of those who do.
    -t      Print symbol table after it has been filled.
    