#include "tree.h"
#include "treeweed.h"
#include "typecheck.h"
#include "pretty_printer.h"
#include "misc.h"
#include "intermediate_tree.h"
#include "intermediate.h"
#include "imt_print.h"
#include "liveness.h"
#include "peep_hole.h"
#include <string.h>

int lineCount;
int flags;
int commentNested;
int parsePassed;
body_t *asm_tree;

void yyparse();

typedef enum _options
{
    opt_pretty_parser,
    opt_pretty_typecheck,
    opt_pretty_weeding,
    opt_pretty_intermediate,
    opt_no_phases,
    opt_debug_check,
    opt_show_table,
    opt_show_liveness,
    opt_no_check_run,
    opt_no_peep_hole,
    opt_simple_static
} options_t;

void set_options(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-pp") == 0)
        {
            SET_FLAG(flags, opt_pretty_parser);            
        }
        else if (strcmp(argv[i], "-pt") == 0)
        {
            SET_FLAG(flags, opt_pretty_typecheck);            
        }
        else if (strcmp(argv[i], "-pw") == 0)
        {
            SET_FLAG(flags, opt_pretty_weeding);            
        }
        else if (strcmp(argv[i], "-pi") == 0)
        {
            SET_FLAG(flags, opt_pretty_intermediate);            
        }
        else if (strcmp(argv[i], "-np") == 0)
        {
            SET_FLAG(flags, opt_no_phases);            
        }
        else if (strcmp(argv[i], "-ss") == 0)
        {
            SET_FLAG(flags, opt_simple_static);            
        }
        else if (strcmp(argv[i], "-d") == 0)
        {
            SET_FLAG(flags, opt_debug_check);            
        }
        else if (strcmp(argv[i], "-t") == 0)
        {
            SET_FLAG(flags, opt_show_table);            
        }
        else if (strcmp(argv[i], "-dl") == 0)
        {
            SET_FLAG(flags, opt_show_liveness);            
        }
        else if (strcmp(argv[i], "-nc") == 0)
        {
            SET_FLAG(flags, opt_no_check_run);            
        }
        else if (strcmp(argv[i], "-nph") == 0)
        {
            SET_FLAG(flags, opt_no_peep_hole);            
        }
    }
}

int main(int argc, char *argv[])
{     
    lineCount = 1;
    flags = 0;

    set_options(argc, argv);
    
    if (!GET_FLAG(flags, opt_no_phases)) 
    {
        fprintf(stderr, "Compiler started...\n");
    }


    if (!GET_FLAG(flags, opt_no_phases)) 
    {
        fprintf(stderr, "Parsing...\n");
    }
    parsePassed = 1;
    yyparse();

    if (commentNested > 0)
    {
        fprintf(stderr, "Amount of end of block comment '*)' did not match start of block comment '(*'.\n");
        parsePassed = 0;
    }
    
    if (!parsePassed)
    {        
        if (!GET_FLAG(flags, opt_no_phases)) 
        {
            fprintf(stderr, "Compiler exited with error in parsing.\n");
        }
        return -1;
    }
    
    if (asm_tree == NULL)
    {
        if (!GET_FLAG(flags, opt_no_phases)) 
        {
            fprintf(stderr, "Compiler exited with error in parsing.\n");
        }
        return -1;
    }
    
    if (GET_FLAG(flags, opt_pretty_parser))
    {
        print_settings(1, 0);
        print_body(asm_tree);
    }

    // Typechecking
    
    if (!GET_FLAG(flags, opt_no_phases)) 
    {
        fprintf(stderr, "Type checking...\n");
    }

    check_init_collection();
    debug_check = GET_FLAG(flags, opt_debug_check);
    debug_show_table = GET_FLAG(flags, opt_show_table);
    int result = check_start_traversel(asm_tree);

    if (!result)
    {
        if (!GET_FLAG(flags, opt_no_phases)) 
        {
            fprintf(stderr, "Compiler exited with error in typechecking.\n");
        }
        return -1;
    }    

    if (GET_FLAG(flags, opt_pretty_typecheck))
    {
        print_settings(1, 1);
        print_body(asm_tree);
    }

    
    if (!GET_FLAG(flags, opt_no_phases)) 
    {
        fprintf(stderr, "Weeding...\n");
    }
    result = weed_start(asm_tree);

    if (!result)
    {
        if (!GET_FLAG(flags, opt_no_phases)) 
        {
            fprintf(stderr, "Compiler exited with errors in weeding.\n");
        }
        return -1;
    }

    if (GET_FLAG(flags, opt_pretty_weeding))
    {
        print_settings(1, 1);
        print_body(asm_tree);
    }

    if (!GET_FLAG(flags, opt_no_phases)) 
    {
        fprintf(stderr, "Creating Intermediate...\n");
    }

    asm_initialize();
    intermediate_add_check = !GET_FLAG(flags, opt_no_check_run);
    intermediate_simple_static = GET_FLAG(flags, opt_simple_static);
    program_t *program = im_make_il_program(asm_tree);
    
    if (GET_FLAG(flags, opt_pretty_intermediate))
    {
        imt_print_program(program);
    }

    if (!GET_FLAG(flags, opt_no_phases)) 
    {
        fprintf(stderr, "Optimizing...\n");
    }

    debug_show_liveness = GET_FLAG(flags, opt_show_liveness);
    live_start_analysis(program);

    if (!GET_FLAG(flags, opt_no_peep_hole))
    {
        peep_hole_optimizer(program);
    }

    if (!GET_FLAG(flags, opt_no_phases)) 
    {
        fprintf(stderr, "Emitting Code...\n");
    }

    asm_program(program);
    
    if (!GET_FLAG(flags, opt_no_phases)) 
    {
        fprintf(stderr, "Compile successful\n");
    }
    return 0;
}
