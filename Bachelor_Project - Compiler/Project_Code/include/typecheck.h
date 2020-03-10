#ifndef __H_TYPECHECK__
#define __H_TYPECHECK__

#include "tree.h"

int debug_check;
int debug_show_table;
#define DEBUG_CHECK_INDENT 2
#define DEBUG_CHECK_START(S) \
    char *debug_name_string = #S; \
    if (debug_check) \
    { \
        fprintf(stderr, "%*sCalled method %s\n",  debug_indent, "", debug_name_string); \
        debug_indent += DEBUG_CHECK_INDENT; \
    }

#define DEBUG_CHECK_END \
    if (debug_check) \
    { \
        debug_indent -= DEBUG_CHECK_INDENT; \
        fprintf(stderr, "%*sEnded method %s\n",  debug_indent, "", debug_name_string); \
    }

#define DEBUG_CHECK_IS_NULL(T) \
    if (debug_check && T == NULL) \
    { \
        fprintf(stderr, "%*s%s is NULL!\n", debug_indent, "", #T); \
    }

#define TYPE_MISMATCH(Name, Node, Print, Type) \
    fprintf(stderr, "main:%d  %s '", Node->lineno, Name); \
    Print(Node); \
    fprintf(stderr, "' is of type <"); \
    print_type(Node->type); \
    fprintf(stderr, ">, type <%s> was expected.\n", Type);

#define CHECK_TYPE_MISMATCH(Name, Node, Kind, Print, Type) \
    if (!check_can_cast_enum(Node->type, Kind)) \
    { \
        TYPE_MISMATCH(Name, Node, Print, Type) \
        DEBUG_CHECK_END \
        return 0; \
    }

#define MISSING_ID(ID, LineNo) \
    fprintf(stderr, "main:%d  identifier '%s' is not declared.\n", \
        LineNo, (char *)ID);

#define INCOMPATIBLE_TYPES(Name1, Node1, Print1, Name2, Node2, Print2, PrintT) \
    fprintf(stderr, "main:%d  %s '", Node2->lineno, Name1); \
    Print1(Node1); \
    fprintf(stderr, "' of type <"); \
    PrintT(Node1->type); \
    fprintf(stderr, "> is incompatible with %s '", Name2); \
    Print2(Node2); \
    fprintf(stderr, "' of type <"); \
    PrintT(Node2->type); \
    fprintf(stderr, ">.\n");

#define CHECK_INCOMPATIPLE_TYPES(Name1, Node1, Print1, Name2, Node2, Print2, PrintT) \
    if (!check_can_cast_type(Node1->type, Node2->type)) \
    { \
        INCOMPATIBLE_TYPES(Name1, Node1, Print1, Name2, Node2, Print2, PrintT) \
        DEBUG_CHECK_END \
        return 0; \
    }

#define GET_SYMBOL(S, ID, LineNo, Table) \
    if (sym_get(Table, ID, &S, 1) == -1) \
    { \
        MISSING_ID(ID, LineNo) \
        DEBUG_CHECK_END \
        return 0; \
    }

#define PUT_SYMBOL(ID, Type, Kind, Lineno, Table) \
    if (sym_put(Table, ID, Kind, Type) == NULL) \
    { \
        fprintf(stderr, "main:%d  identifier '%s' is already declared.\n", \
            Lineno, (char *)ID); \
        DEBUG_CHECK_END \
        return 0; \
    } \

#define CHECK_ONE(Child, Check) \
    if (!Check(Child)) \
    { \
        DEBUG_CHECK_END \
        return 0; \
    }
#define CHECK_SYMBOL(Name, Symbol, Node, Kind) \
    if (Symbol->kind != Kind) \
    { \
        fprintf(stderr, "main:%d  identifier '%s' is not a %s.\n", \
            Node->lineno, Symbol->name, Name); \
        DEBUG_CHECK_END \
        return 0; \
    }

int debug_indent;

typedef struct _case
{
    int flags;
    char *error_message;
} case_t;

typedef struct _checks
{
    case_t *is_compare;
    case_t *is_calc;
    case_t *is_boolean;
    case_t *is_pointer;
} checks_t;

checks_t *checks;

void check_init_collection();
case_t *check_init_case(int flags, char *error_message);

int check_can_cast_type(type_t *type1, type_t *type2);
int check_can_cast_enum(type_t *type1, type_kind_t kind);
int check_is_valid(case_t *err, type_t *type);
int check_start_traversel(body_t *asm_tree);

int check_function(void *v_function);
// void check_head(void *v_head);
int check_type(void *v_type);
int check_var_type(void *v_var_type);
int check_body(void *v_body);
int check_declaration(void *v_declaration);
int check_statement(void *v_statement);
int check_variable(void *v_variable);
int check_exp(void *v_exp);
int check_term(term_t *term);

#endif
