#include "tree.h"
#include "pretty_printer.h"

int lineCount;

void yyparse();

body_t *asm_tree;

int main()
{ 
    lineCount = 1;
    yyparse();
    print_body(asm_tree);
    return 0;
}
