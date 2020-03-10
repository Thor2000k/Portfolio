#include "typecheck.h"
#include "misc.h"
#include "tree.h"
#include "symbol.h"
#include "pretty_printer.h"
#include <stdio.h>
#include <string.h>

array_list *function_queue;
array_list *type_queue;
symbol_table *current_table;
function_t *current_function = NULL;

void print_head_name(head_t *head)
{
    fprintf(stderr, "%s", head->ID);
}

void check_init_collection()
{
    checks = NEW(checks_t);

    int flags = 0;
    char *str;

    flags = 0;
    SET_FLAG(flags, kind_t_integer);
    str = "ERROR: expression is not a number, line %d.\n";
    checks->is_calc = check_init_case(flags, str);

    flags = 0;
    SET_FLAG(flags, kind_t_boolean);
    str = "ERROR: expression is not of type boolean, line %d.\n";
    checks->is_boolean = check_init_case(flags, str);
    
    flags = 0;
    SET_FLAG(flags, kind_t_array);
    SET_FLAG(flags, kind_t_record);
    SET_FLAG(flags, kind_t_symbol);
    str = "ERROR: expression is not of type boolean, line %d.\n";
    checks->is_pointer = check_init_case(flags, str);
    
}

case_t *check_init_case(int flags, char *error_message)
{
    case_t *chk = NEW(case_t);
    chk->flags = flags;
    chk->error_message = malloc_string(error_message);

    return chk;
}

int check_is_valid(case_t *err, type_t *type)
{
    DEBUG_CHECK_START(check_is_valid)

    if (type == NULL)
    {
        DEBUG_CHECK_END
        return 0;
    }

    int flag = GET_FLAG(err->flags, type->kind);
    DEBUG_CHECK_END
    return flag;
}

// can be used by all lists
int check_list(array_list *list, int (*check_f)(void *node))
{
    DEBUG_CHECK_START(check_list)

    if (list == NULL)
    {
        DEBUG_CHECK_END
        return 1;
    }

    int size = list->size;
    void *ptr;

    int result = 1;

    for (int i = 0; i < size; i++)
    {
        AL_GET_PTR(ptr, list, i);
        result = result & check_f(ptr);
    }

    DEBUG_CHECK_END
    return result;
}

int check_can_cast_type(type_t *type1, type_t *type2)
{
    DEBUG_CHECK_START(check_can_cast_type)
    if (type1 == NULL || type2 == NULL)
    {
        DEBUG_CHECK_END
        return 0;
    }

    if (type1->kind == kind_t_undeclared && 
    check_is_valid(checks->is_pointer, type2))
    {
        DEBUG_CHECK_END
        return 1;
    }
    else if (type2->kind == kind_t_undeclared && 
    check_is_valid(checks->is_pointer, type1))
    {
        DEBUG_CHECK_END
        return 1;
    }

    if (type1->kind != type2->kind)
    {
        DEBUG_CHECK_END
        return 0;
    }    

    if (type1->kind == kind_t_symbol)
    {
        int is_equal = type1->value.symbol == type2->value.symbol;

        DEBUG_CHECK_END
        return is_equal;
    }

    if (type1->kind == kind_t_record)
    {
        array_list *var_list1 = type1->value.record.var_list;
        array_list *var_list2 = type2->value.record.var_list;
        
        if (var_list1->size != var_list2->size)
        {
            DEBUG_CHECK_END
            return 0;
        } 

        var_type_t *var_type1, *var_type2;

        for (int i = 0; i < var_list1->size; i++)
        {
            AL_GET_PTR(var_type1, var_list1, i);
            AL_GET_PTR(var_type2, var_list2, i);

            if (!check_can_cast_type(var_type1->type, var_type2->type))
            {
                DEBUG_CHECK_END
                return 0;
            }
        }
    }

    DEBUG_CHECK_END
    return 1;
}

int check_can_cast_enum(type_t *type, type_kind_t kind)
{
    DEBUG_CHECK_START(check_can_cast_enum)
    
    if (type == NULL)
    {
        DEBUG_CHECK_END
        return 0;
    }

    if (kind == kind_t_record && type->kind == kind_t_symbol)
    {
        symbol_t *symbol = type->value.symbol;

        DEBUG_CHECK_END
        return symbol->kind == kind_s_ID;
    }

    DEBUG_CHECK_END
    return type->kind == kind;
}

int check_start_traversel(body_t *asm_tree)
{
    print_settings(0, 0);
    
    if (asm_tree == NULL)
    {
        return 1;
    }

    asm_tree->table = sym_init_table();
    current_table = asm_tree->table; 

    return check_body(asm_tree);
}

/* ==== Tree Traversel ==== */
int check_function(void *v_function)
{
    DEBUG_CHECK_START(check_function)
    function_t *function = v_function;

    function_t *temp_function = current_function;
    current_function = function;

    symbol_table *temp_table = current_table;
    current_table = function->body->table;

    int result = check_list(function->head->par_list, &check_var_type);
    result = result && check_body(function->body);

    current_table = temp_table;
    current_function = temp_function;

    DEBUG_CHECK_END
    return result;
}

int check_type(void *v_type)
{
    DEBUG_CHECK_START(check_type)
    type_t *type = v_type;

    symbol_t *symbol;
    symbol_table *temp_table;
    type_t *s_type;
    int result;

	switch(type->kind)
    {            
		case kind_t_array:
            result = check_type(type->value.array_type);
            DEBUG_CHECK_END
            return result;
            break;
            
		case kind_t_record:
            temp_table = current_table;
            current_table = sym_scope_table(temp_table);
            
            array_list *tq = al_init_list(5, sizeof(void *));
            type_queue = tq;
            
            result = check_list(type->value.record.var_list, &check_var_type);
            result = result && check_list(tq, &check_type);
            al_destroy(tq, NULL);
            type_queue = NULL;

            if (debug_show_table) sym_print_table(current_table);

            type->value.record.table = current_table;
            current_table = temp_table;

            DEBUG_CHECK_END
            return result;
            break;
            
		case kind_t_identifier:                    
            GET_SYMBOL(symbol, type->value.ID, type->lineno, current_table)

            if (symbol->kind == kind_s_ID)
            {
                type->value.symbol = symbol;
                type->kind = kind_t_symbol;

                DEBUG_CHECK_END
                return 1;
            }

            CHECK_SYMBOL("type", symbol, type, kind_s_type)

            s_type = symbol->value.type;
            
            if (symbol->is_marked)
            {
                fprintf(stderr, "main:%d  identifier '%s' is  a cyclic reference.\n",
                    type->lineno, type->value.ID);

                DEBUG_CHECK_END
                return 0;
            }

            symbol->is_marked = 1;

            result = check_type(s_type);
            if (!result)
            {
                DEBUG_CHECK_END
                return 0;
            }

            symbol->is_marked = 0;
            
            type->kind = s_type->kind;
            type->value = s_type->value;
            break;
            
		default:            
            break;            	
    }

    DEBUG_CHECK_END
    return 1;
}

int check_var_type(void *v_var_type)
{
    DEBUG_CHECK_START(check_var_type)
    var_type_t *var_type = v_var_type;
    
    PUT_SYMBOL(var_type->ID, var_type->type, kind_s_var,
        var_type->lineno, current_table)
    
    al_add(type_queue, &var_type->type);

    DEBUG_CHECK_END
    return 1;
}

int check_body(void *v_body)
{
    DEBUG_CHECK_START(check_body)
    body_t *body = v_body;

    if (body == NULL)
    {
        DEBUG_CHECK_END        
        return 0;
    }

    array_list *fq = al_init_list(10, sizeof(void *));
    array_list *tq = al_init_list(10, sizeof(void *));
    type_queue = tq;
    function_queue = fq;

    // add declarations in this scope to symbol table 
    int result = check_list(body->decl_list, &check_declaration);
    
    // check symbols of current symbol table
    result = result && check_list(type_queue, &check_type);
    al_destroy(tq, NULL);
    type_queue = NULL;

    if (debug_show_table)
    {
        sym_print_table(current_table);
    }
    
    // check next depth of scopes
    result = result && check_list(fq, &check_function);
    
    al_destroy(fq, NULL);
    function_queue = NULL;

    // check statements in this scope
    result = result && check_list(body->stat_list, &check_statement);
    
    DEBUG_CHECK_END
    return result;
}

int check_declaration(void *v_declaration)
{
    DEBUG_CHECK_START(check_declaration)
    declaration_t *declaration = v_declaration;
    
    type_t *s_type;
    array_list *par_list;
    var_type_t *var_type;
    symbol_kind_t s_kind;
    function_t *function;
    int result;

    int i = 0;

    switch(declaration->kind)
    {
        case kind_typedef:
            s_type = declaration->value.assign_type.type;
            s_kind = s_type->kind == kind_t_record ? kind_s_ID : kind_s_type;
            
            PUT_SYMBOL(declaration->value.assign_type.ID, s_type, s_kind, 
                declaration->lineno, current_table)

            al_add(type_queue, &s_type);
            break;
        
        case kind_function:
            function = declaration->value.function;
            
            PUT_SYMBOL(function->head->ID, function, kind_s_function, 
                declaration->lineno, current_table)

            par_list = function->head->par_list; 
            if (par_list != NULL)
            {
                AL_FOR_EACH(var_type_t *, var_type, par_list, i)
                    al_add(type_queue, &var_type->type);
            }

            al_add(type_queue, &function->head->type);

            function->body->table = sym_scope_table(current_table);
            al_add(function_queue, &function);
            break;
        
        case kind_var_list:
            result = check_list(declaration->value.var_list , &check_var_type);
            DEBUG_CHECK_END
            return result;
            break;
        
        default:
            break;  
    }

    DEBUG_CHECK_END
    return 1;
}

int check_statement(void *v_statement)
{
    DEBUG_CHECK_START(check_statement)
    statement_t *statement = v_statement;
    exp_t *exp, *src_index, *dst_index, *count;
    variable_t *variable, *src, *dst;
    symbol_t *symbol;
    int result;

    switch (statement->kind)
    {
        case kind_return:
            exp = statement->value.expression;
            CHECK_ONE(exp, check_exp);

            if (current_function == NULL)
            {
                fprintf(stderr, "main:%d  Found return in main scope.\n", 
                    statement->lineno);

                DEBUG_CHECK_END
                return 0;
            }

            CHECK_INCOMPATIPLE_TYPES("function", current_function->head, 
                    print_head_name, "expression", exp, print_exp, print_type)

            break;

        case kind_write:
            exp = statement->value.expression;
            CHECK_ONE(exp, check_exp);
            CHECK_TYPE_MISMATCH("expression", exp, kind_t_integer, 
            print_exp, "int")
            break;

        case kind_read:
            variable = statement->value.variable;
            CHECK_ONE(variable, check_variable);
            CHECK_TYPE_MISMATCH("variable", variable, kind_t_integer, 
            print_variable, "int")
            break;

        case kind_allocate_record:
            variable = statement->value.variable;
            CHECK_ONE(variable, check_variable)
            CHECK_TYPE_MISMATCH("variable", variable, kind_t_record, 
            print_variable, "record")            
            break;

        case kind_allocate_array:
            variable = statement->value.allocate_array.variable;
            exp = statement->value.allocate_array.expression;
            
            CHECK_ONE(variable, check_variable)
            CHECK_ONE(exp, check_exp)

            CHECK_TYPE_MISMATCH("variable", variable, kind_t_array,
             print_variable, "array")
            CHECK_TYPE_MISMATCH("expression", exp, kind_t_integer,
             print_exp, "number")
            break;

        case kind_assign:
            variable = statement->value.assign.variable;
            exp = statement->value.assign.expression;

            CHECK_ONE(variable, check_variable)
            CHECK_ONE(exp, check_exp)
            
            if (variable->kind == kind_id)
            {
                GET_SYMBOL(symbol, variable->value.ID, variable->lineno,
                 current_table)
                CHECK_SYMBOL("variable", symbol, variable, kind_s_var)
            }

            if (check_is_valid(checks->is_pointer, variable->type) &&
                check_can_cast_enum(exp->type, kind_t_undeclared))
            {
                DEBUG_CHECK_END
                return 1;
            }

            CHECK_INCOMPATIPLE_TYPES("variable", variable, print_variable, 
                    "expression", exp, print_exp, print_type)

            break;

        case kind_if:
            exp = statement->value.if_statement.expression;
            
            CHECK_ONE(exp, check_exp)
            CHECK_ONE(statement->value.if_statement.statement, check_statement)
            CHECK_TYPE_MISMATCH("expression", exp, kind_t_boolean, 
            print_exp, "boolean")
            break;
            
        case kind_if_else:
            exp = statement->value.if_else.expression;
            
            CHECK_ONE(exp, check_exp)
            CHECK_TYPE_MISMATCH("expression", exp, kind_t_boolean, 
            print_exp, "boolean")
            
            result = check_statement(statement->value.if_else.if_statement);
            result = result & check_statement(statement->value.if_else.else_statement);

            DEBUG_CHECK_END
            return result;            
            break;

        case kind_while:
            exp = statement->value.while_statement.expression;
            
            CHECK_ONE(exp, check_exp)
            CHECK_ONE(statement->value.while_statement.statement, check_statement)
            CHECK_TYPE_MISMATCH("expression", exp, kind_t_boolean, 
            print_exp, "boolean")      

            break;

        case kind_statement_list:
            result = check_list(statement->value.statement_list,
             &check_statement);

            DEBUG_CHECK_END
            return result;
            break;

        case kind_copy:
            src = statement->value.copy.src;
            dst = statement->value.copy.dst;

            CHECK_ONE(src, check_variable)
            CHECK_ONE(dst, check_variable)
            CHECK_INCOMPATIPLE_TYPES("variable", src, print_variable, 
                "variable", dst, print_variable, print_type)

            break;

        case kind_copy_from:
            src = statement->value.copy_from.src;
            dst = statement->value.copy_from.dst;
            src_index = statement->value.copy_from.src_index;
            dst_index = statement->value.copy_from.dst_index;
            count = statement->value.copy_from.count;

            CHECK_ONE(src, check_variable)
            CHECK_ONE(dst, check_variable)

            CHECK_INCOMPATIPLE_TYPES("variable", src, print_variable, 
                "variable", dst, print_variable, print_type)

            CHECK_ONE(src_index, check_exp)
            CHECK_ONE(dst_index, check_exp)
            CHECK_ONE(count, check_exp)

            CHECK_TYPE_MISMATCH("expression", src_index, kind_t_integer, 
            print_exp, "int")

            CHECK_TYPE_MISMATCH("expression", dst_index, kind_t_integer,
            print_exp, "int")

            CHECK_TYPE_MISMATCH("expression", count, kind_t_integer, 
            print_exp, "int")
            
            break;

        default:
            break;
    }

    DEBUG_CHECK_END
    return 1;
}

int check_variable(void *v_variable)
{
    DEBUG_CHECK_START(check_variable)
    variable_t *variable = v_variable;

    variable_t *child_variable;
    exp_t *exp;
    symbol_t *symbol;
    char *id;
    int has_field;
    type_t *type_record;

    switch (variable->kind)
    {
        case kind_id:
            GET_SYMBOL(symbol, variable->value.ID, variable->lineno, current_table)            

            CHECK_SYMBOL("variable", symbol, variable, kind_s_var)
            /*
            if (symbol->kind == kind_s_function)
            {
                fprintf(stderr, "main:%d  identifier '%s' is a function and not a variable.\n", 
                    variable->lineno, variable->value.ID);

                DEBUG_CHECK_END
                return 0;
            }
            
            if (symbol->kind == kind_s_type || symbol->kind == kind_s_ID)
            {
                fprintf(stderr, "main:%d  identifier '%s' is a type and not a variable.\n",
                    variable->lineno, variable->value.ID);

                DEBUG_CHECK_END
                return 0;
            }
            */
            DEBUG_CHECK_IS_NULL(symbol->value.type)
            variable->type = symbol->value.type;            
            break;

        case kind_index:
            child_variable = variable->value.get_index.variable;
            exp = variable->value.get_index.exp;
            
            CHECK_ONE(child_variable, check_variable)
            CHECK_ONE(exp, check_exp)

            CHECK_TYPE_MISMATCH("variable",
             child_variable, kind_t_array, print_variable, "array")

            CHECK_TYPE_MISMATCH("expression", exp,
             kind_t_integer, print_exp, "int")

            DEBUG_CHECK_IS_NULL(child_variable->type->value.array_type)
            variable->type = child_variable->type->value.array_type;
            break;

        case kind_field:
            child_variable = variable->value.field.variable;
            CHECK_ONE(child_variable, check_variable)

            if (child_variable->type->kind == kind_t_symbol)
            {
                symbol = child_variable->type->value.symbol;
                type_record = symbol->value.type;
            }
            else
            {
                type_record = child_variable->type;
            }            

            CHECK_TYPE_MISMATCH("variable", child_variable,
             kind_t_record , print_variable, "record")

            id = variable->value.field.ID;

            has_field = sym_get(type_record->value.record.table, 
                id, &symbol, 1) == 0;

            if (!has_field)
            {
                fprintf(stderr, "main:%d  identifer '%s' is not a field of the record ", 
                    variable->lineno, id);

                print_variable(variable->value.field.variable);

                fprintf(stderr, ".\n");
                DEBUG_CHECK_END
                return 0;
            }

            DEBUG_CHECK_IS_NULL(symbol->value.type)
            variable->type = symbol->value.type;
            break;
    }

    DEBUG_CHECK_IS_NULL(variable->type)
    DEBUG_CHECK_END
    return 1;
}

int check_exp(void *v_exp)
{
    DEBUG_CHECK_START(check_exp)
    exp_t *exp = v_exp;

    if (exp->kind == kind_term)
    {
        int result = check_term(exp->value.term);
        exp->type = exp->value.term->type;
        DEBUG_CHECK_IS_NULL(exp->type)
        DEBUG_CHECK_END
        return result;
    }

    exp_t *left, *right;
    left = exp->value.bin_exp.left;
    right = exp->value.bin_exp.right;

    int result1 = check_exp(left);
    int result2 = check_exp(right);

    if (!(result1 || result2))
    {
        DEBUG_CHECK_END
        return 0;
    }

    switch (exp->kind)
    {
        case kind_plus:
            if( left->type->kind == kind_t_array && right->type->kind == kind_t_array ){
                CHECK_INCOMPATIPLE_TYPES("expression", left, print_exp, 
                    "expression", right, print_exp, print_type)
                exp->type = left->type; 
                break;
            }

            CHECK_TYPE_MISMATCH("expression", left,
             kind_t_integer, print_exp, "number");

            CHECK_TYPE_MISMATCH("expression", right,
             kind_t_integer, print_exp, "number");

            exp->type = left->type;
            break;

        case kind_minus:
        case kind_multiply:         
        case kind_divide:  
        case kind_modulo:
            CHECK_TYPE_MISMATCH("expression", left,
             kind_t_integer, print_exp, "number");
            
            CHECK_TYPE_MISMATCH("expression", right,
             kind_t_integer, print_exp, "number");
            exp->type = left->type;
            break;

        case kind_greater:            
        case kind_less:            
        case kind_greater_equal:            
        case kind_less_equal:  
            CHECK_TYPE_MISMATCH("expression", left,
             kind_t_integer, print_exp, "number");

            CHECK_TYPE_MISMATCH("expression", right,
             kind_t_integer, print_exp, "number");

            exp->type = make_type(NULL, kind_t_boolean);
            break;

        case kind_equal:            
        case kind_not_equal:
            result1 = left->type->kind == kind_t_undeclared && 
                check_is_valid(checks->is_pointer, right->type);

            result1 = result1 || (check_is_valid(checks->is_pointer, left->type) &&
                right->type->kind == kind_t_undeclared);

            if (!result1)
                CHECK_INCOMPATIPLE_TYPES("expression", left, print_exp, 
                    "expression", right, print_exp, print_type)

            exp->type = make_type(NULL, kind_t_boolean);
            break;  
            
        case kind_and:            
        case kind_or:
            CHECK_TYPE_MISMATCH("expression", left,
             kind_t_boolean, print_exp, "boolean");

            CHECK_TYPE_MISMATCH("expression", right,
             kind_t_boolean, print_exp, "boolean");
            
            exp->type = left->type;
            break;

        default:
            break;
    }

    DEBUG_CHECK_IS_NULL(exp->type)
    DEBUG_CHECK_END
    return 1;
}

int check_term(term_t *term)
{
    DEBUG_CHECK_START(check_term)
    var_type_t *var_type;
    exp_t *exp;
    variable_t *variable;
    term_t *child_term;
    array_list *act_list;
    symbol_t *symbol;
    int result1, size_A, size_P;

	switch(term->kind)
	{
		case kind_false:
		case kind_true:
			term->type = make_type(NULL, kind_t_boolean);
			break;

		case kind_integer:
			term->type = make_type(NULL, kind_t_integer);
			break;

		case kind_absolute:
            exp = term->value.ptr;
            CHECK_ONE(exp, check_exp)

            if (check_can_cast_enum(exp->type, kind_t_array))
            {
                term->type = make_type(NULL, kind_t_integer);
                DEBUG_CHECK_END
                return 1;
            }

            CHECK_TYPE_MISMATCH("expression", exp,
             kind_t_integer, print_exp, "int")

            term->type = exp->type;
			break;

		case kind_negate:
            child_term = term->value.ptr;
            CHECK_ONE(child_term, check_term)
            CHECK_TYPE_MISMATCH("term", child_term,
             kind_t_boolean, print_term, "boolean")

            term->type = child_term->type;
			break;

		case kind_null:
            term->type = make_type(NULL, kind_t_undeclared);
			break;

		case kind_variable:
            variable = term->value.ptr;
            CHECK_ONE(variable, check_variable)
            term->type = variable->type;            
			break;

		case kind_parenthess:
            exp = term->value.ptr;
            CHECK_ONE(exp, check_exp)

            term->type = exp->type;
            break;

		case kind_method:
            act_list = term->value.method.actions;
            GET_SYMBOL(symbol, term->value.method.ID,
             term->lineno, current_table)
            
            head_t *head = symbol->value.func->head;
            size_A = act_list == NULL ? 0 : act_list->size;
            size_P = head->par_list == NULL ? 0 : head->par_list->size;
                
            if (size_A != size_P)
            {
                fprintf(stderr, "main:%d  Not the right amount of arguments ", 
                    term->lineno);
                fprintf(stderr, "in method call ");
                print_term(term);
                fprintf(stderr, ".\n     Expected %d, but found %d.\n", 
                    size_P, size_A);

                DEBUG_CHECK_END    
                return 0;
            }       
            
            result1 = check_list(act_list, &check_exp);
            if (!result1)
            {
                DEBUG_CHECK_END
                return 0;
            }     

            for (int i = 0; i < size_A; i++)
            {
                AL_GET_PTR(var_type, head->par_list, i);
                AL_GET_PTR(exp, act_list, i);

                CHECK_INCOMPATIPLE_TYPES("argument", var_type,
                 print_var_type, "expression", exp, print_exp, print_type)
            }

            term->type = head->type;
			break;

        case kind_sqrt:
            exp = term->value.ptr;
            CHECK_ONE(exp, check_exp)
            CHECK_TYPE_MISMATCH("expression", exp, kind_t_integer,
             print_exp, "int")

            term->type = exp->type;
            break;

		default:
			fprintf(stderr, "ERROR: unknown term\n");
	} 

    DEBUG_CHECK_IS_NULL(term->type)
    DEBUG_CHECK_END
    return 1;
}
