#include "pretty_printer.h"
#include "tree.h"
#include "array_list.h"

#include <stdio.h>

int indent_increment = 4;
int indent_size = 0;

int show_newlines = 1;
int show_extra_types = 0;
int current_line;

void print_settings(int p_show_new_lines, int p_show_types)
{
    current_line = 0;
    show_newlines = p_show_new_lines;
    show_extra_types = p_show_types;
}

void print_indent()
{  
    PRINT_DEBUG(print_indent)

    if (indent_size > 0 && show_newlines)
        fprintf(stderr, "%*s", indent_size, "");
    else if (!show_newlines)
        fprintf(stderr, " ");
}

void print_newline()
{
    if (show_newlines)
    {
        fprintf(stderr, "\n");
    }
}

void print_extra_types(type_t* type)
{
    if (show_extra_types)
    {            
        fprintf(stderr, "<");
        print_type(type);
        fprintf(stderr, ">");
    }
}

void print_if_not_term(exp_t *exp)
{
    if (exp->kind == kind_term && exp->value.term->kind == kind_parenthess)
    {
        print_extra_types(exp->type);
    }
    else if (exp->kind != kind_term)
    {
        print_extra_types(exp->type);
    }
}

// can be used by all lists
void print_list(array_list *list, char *delim, void (*print_f)(void *node))
{
    PRINT_DEBUG(print_list)

    if (list == NULL)
    {
        return;
    }

    int size = list->size;
    void *ptr;

    for (int i = 0; i < size; i++)
    {
        AL_GET_PTR(ptr, list, i);
        print_f(ptr);
        if (i + 1 < size)
        {
            fprintf(stderr, "%s", delim);   
        } 
    }
}

void print_function(void *v_function)
{
    PRINT_DEBUG(print_function)
    if (v_function == NULL)
    {        
        fprintf(stderr, "NULL function");
        print_newline();
        print_newline();
        return;
    }

    function_t *function = v_function;

    print_newline();
    print_indent();

    fprintf(stderr, "(");
    print_head(function->head);

    NEXT_SCOPE(print_body(function->body))

    print_indent();
    fprintf(stderr, "end %s", function->head->ID);    
    print_newline();
    fprintf(stderr, ")");
    print_newline(); 
}

void print_head(void *v_head)
{
    PRINT_DEBUG(print_head)
    if (v_head == NULL)
    {        
        fprintf(stderr, "NULL head");
        print_newline();
        return;
    }

    head_t *head = v_head;

    fprintf(stderr,"%s(", head->ID);
    print_list(head->par_list, ", ",  &print_var_type);
    fprintf(stderr, "):");
    print_type(head->type);
    print_newline();
}

void print_type(void *v_type)
{
    PRINT_DEBUG(print_type)
    if (v_type == NULL)
    {        
        fprintf(stderr, "NULL type");
        return;
    }

    type_t *type = v_type;
    symbol_t *symbol;

	switch(type->kind)
    {
		case kind_t_integer:
			fprintf(stderr, "int");
            break;

		case kind_t_boolean:
			fprintf(stderr, "bool");
            break;
            
		case kind_t_array:
			fprintf(stderr, "array of ");
            print_type(type->value.array_type);
            break;
            
		case kind_t_record:
			fprintf(stderr, "record of { ");
            print_list(type->value.record.var_list, ", ", &print_var_type);
			fprintf(stderr, " }");
            break;
            
		case kind_t_identifier:
			fprintf(stderr, "%s", (char *)type->value.ID);
            break;
            
        case kind_t_undeclared:
            fprintf(stderr, "null pointer");
            break;

        case kind_t_symbol:
            symbol = type->value.symbol;
            fprintf(stderr, "sym: '%s:%d'", symbol->name, symbol->value.type->lineno);
            break;

		default:
			fprintf(stderr, "ERROR: unknown type");
            print_newline();	
            break;            	
    }
}

void print_var_type(void *v_var_type)
{
    PRINT_DEBUG(print_var_type)
    if (v_var_type == NULL)
    {        
        fprintf(stderr, "NULL var_type");
        return;
    }

    var_type_t *var_type = v_var_type;

    fprintf(stderr,"%s:", var_type->ID);
    print_type(var_type->type);
}

void print_body(void *v_body)
{
    PRINT_DEBUG(print_body)
    if (v_body == NULL)
    {        
        fprintf(stderr, "NULL body");
        return;
    }

    body_t *body = v_body;

    print_list(body->decl_list, "", &print_declaration);
    print_list(body->stat_list, "", &print_statement);
}

void print_declaration(void *v_declaration)
{
    PRINT_DEBUG(print_declaration)
    if (v_declaration == NULL)
    {        
        fprintf(stderr, "NULL declaration\n");
        return;
    }

    declaration_t *declaration = v_declaration;
    
    PREV_SCOPE(print_indent())
    
    switch(declaration->kind)
    {
        case kind_typedef:
            fprintf(stderr, "type %s = ", (char *)declaration->value.assign_type.ID);
            print_type(declaration->value.assign_type.type);
            fprintf(stderr, ";");
            print_newline();
            
            break;
        
        case kind_function:
            print_function(declaration->value.function);
            break;
        
        case kind_var_list:
            fprintf(stderr, "var ");
            print_list(declaration->value.var_list , ", ", &print_var_type );
            fprintf(stderr, ";");
            print_newline();
            break;
        
        default:
			fprintf(stderr, "ERROR: unknown declaration");
            print_newline();
            break;  
    }
}

void print_statement(void *v_statement)
{
    PRINT_DEBUG(print_statement)
    if (v_statement == NULL)
    {        
        fprintf(stderr, "NULL statement");
        print_newline();
        return;
    }

    statement_t *statement = v_statement;

    if (statement->kind == kind_statement_list)
    {
        PREV_SCOPE(print_indent())
    }
    else
    {
        print_indent();
    }

    switch (statement->kind)
    {
        case kind_return:
            fprintf(stderr, "return ");  
            print_if_not_term(statement->value.expression);          
            print_exp(statement->value.expression);
            fprintf(stderr, ";");
            print_newline();
            break;

        case kind_write:
            fprintf(stderr, "write ");
            print_if_not_term(statement->value.expression);
            print_exp(statement->value.expression);
            fprintf(stderr, ";");
            print_newline();
            break;

        case kind_allocate_record:
            fprintf(stderr, "allocate ");
            print_variable(statement->value.variable);
            fprintf(stderr, ";");
            print_newline();
            break;

        case kind_allocate_array:
            fprintf(stderr, "allocate ");
            print_variable(statement->value.allocate_array.variable);

            fprintf(stderr, " of length ");
            print_if_not_term(statement->value.allocate_array.expression);
            print_exp(statement->value.allocate_array.expression);

            fprintf(stderr, ";");
            print_newline();
            break;

        case kind_assign:
            print_variable(statement->value.assign.variable);
            fprintf(stderr, " = ");
            print_if_not_term(statement->value.assign.expression);
            print_exp(statement->value.assign.expression);

            fprintf(stderr, ";");
            print_newline();
            break;

        case kind_if:
            fprintf(stderr, "(if ");
            print_if_not_term(statement->value.if_statement.expression);
            print_exp(statement->value.if_statement.expression);
            fprintf(stderr, " then");
            print_newline();
            
            NEXT_SCOPE(print_statement(statement->value.if_statement.statement))
            
            print_indent();
            fprintf(stderr, ")");
            print_newline();
            break;
            
        case kind_if_else:
            fprintf(stderr, "(if ");
            print_if_not_term(statement->value.if_else.expression);
            print_exp(statement->value.if_else.expression);
            fprintf(stderr, " then");
            print_newline();

            NEXT_SCOPE(print_statement(statement->value.if_else.if_statement))

            print_indent();
            fprintf(stderr, "else");
            print_newline();
            
            NEXT_SCOPE(print_statement(statement->value.if_else.else_statement))

            print_indent();
            fprintf(stderr, ")");
            print_newline();
            break;

        case kind_while:
            fprintf(stderr, "(while ");
            print_if_not_term(statement->value.while_statement.expression);
            print_exp(statement->value.while_statement.expression);
            fprintf(stderr, " do");
            print_newline();
            
            NEXT_SCOPE(print_statement(statement->value.while_statement.statement))
            
            print_indent();
            fprintf(stderr, ")");
            print_newline();
            break;

        case kind_statement_list:
            fprintf(stderr, "{");
            print_newline();    

            if (indent_size == 0)
                indent_size += indent_increment;   

            print_list(statement->value.statement_list, "", &print_statement);
        
            if (indent_size != indent_increment)
            {
                PREV_SCOPE(print_indent())
            }
            
            fprintf(stderr, "}");
            print_newline();

            break;

        default:
			fprintf(stderr, "ERROR: unknown statement");
            print_newline();
            break;
    }
}

void print_variable(void *v_variable)
{
    PRINT_DEBUG(print_variable)
    if (v_variable == NULL)
    {        
        fprintf(stderr, "NULL variable");
        return;
    }

    variable_t *variable = v_variable;

    print_extra_types(variable->type);

    if (variable->kind == kind_id)
    {
        fprintf(stderr, "%s", (char *)variable->value.ID);
        return;
    }

    fprintf(stderr, "(");
    switch (variable->kind)
    {
        case kind_index:
            print_variable(variable->value.get_index.variable);
            fprintf(stderr, "[");
            print_exp(variable->value.get_index.exp);
            fprintf(stderr, "]");
            break;

        case kind_field:
            print_variable(variable->value.field.variable);
            fprintf(stderr, ".%s", (char *)variable->value.field.ID);
            break;
    
        default:
			fprintf(stderr, "ERROR: unknown variable");
            print_newline();
            break;
    }

    fprintf(stderr, ")");
}

void print_exp(void *v_expression)
{
    PRINT_DEBUG(print_exp)
    if (v_expression == NULL)
    {        
        fprintf(stderr, "NULL expression");
        return;
    }

    exp_t *expression = v_expression;

    if (expression->kind == kind_term)
    {        
        print_term(expression->value.term);
        return;
    }

    fprintf(stderr, "(");
    switch (expression->kind)
    {
        case kind_plus:
            print_exp(expression->value.bin_exp.left);
            fprintf(stderr, " + ");
            print_exp(expression->value.bin_exp.right);
            break;

        case kind_minus:
            print_exp(expression->value.bin_exp.left);
            fprintf(stderr, " - ");
            print_exp(expression->value.bin_exp.right);
            break;

        case kind_multiply:
            print_exp(expression->value.bin_exp.left);
            fprintf(stderr, " * ");
            print_exp(expression->value.bin_exp.right);
            break;
            
        case kind_divide:
            print_exp(expression->value.bin_exp.left);
            fprintf(stderr, " / ");
            print_exp(expression->value.bin_exp.right);
            break;
            
        case kind_greater:
            print_exp(expression->value.bin_exp.left);
            fprintf(stderr, " > ");
            print_exp(expression->value.bin_exp.right);
            break;
            
        case kind_less:
            print_exp(expression->value.bin_exp.left);
            fprintf(stderr, " < ");
            print_exp(expression->value.bin_exp.right);
            break;
            
        case kind_greater_equal:
            print_exp(expression->value.bin_exp.left);
            fprintf(stderr, " >= ");
            print_exp(expression->value.bin_exp.right);
            break;
            
        case kind_less_equal:
            print_exp(expression->value.bin_exp.left);
            fprintf(stderr, " <= ");
            print_exp(expression->value.bin_exp.right);
            break;
            
        case kind_equal:
            print_exp(expression->value.bin_exp.left);
            fprintf(stderr, " == ");
            print_exp(expression->value.bin_exp.right);
            break;
            
        case kind_not_equal:
            print_exp(expression->value.bin_exp.left);
            fprintf(stderr, " != ");
            print_exp(expression->value.bin_exp.right);
            break;
            
        case kind_and:
            print_exp(expression->value.bin_exp.left);
            fprintf(stderr, " && ");
            print_exp(expression->value.bin_exp.right);
            break;
            
        case kind_or:
            print_exp(expression->value.bin_exp.left);
            fprintf(stderr, " || ");
            print_exp(expression->value.bin_exp.right);
            break;            

        case kind_modulo:
            print_exp(expression->value.bin_exp.left);
            fprintf(stderr, " %% ");
            print_exp(expression->value.bin_exp.right);
            break;       

        default:
			fprintf(stderr, "ERROR: unknown expression");
            print_newline();
            break;
    }

    fprintf(stderr, ")");
}

void print_term(void *v_term)
{
    PRINT_DEBUG(print_term)
    if (v_term == NULL)
    {        
        fprintf(stderr, "NULL term");
        return;
    }

    term_t *term = v_term;

	switch(term->kind)
	{
		case kind_false:
			fprintf(stderr, "false");
			break;

		case kind_true:
			fprintf(stderr, "true");
			break;

		case kind_integer:
			fprintf(stderr, "%d", term->value.num);
			break;

		case kind_absolute:
			fprintf(stderr, "|");
            print_exp(term->value.ptr);
            fprintf(stderr, "|");
			break;

		case kind_negate:
			fprintf(stderr, "(!");
            print_term(term->value.ptr);
            fprintf(stderr, ")");
			break;

		case kind_null:
			fprintf(stderr, "null");
			break;

		case kind_variable:
			print_variable(term->value.ptr);
			break;

		case kind_parenthess:
			fprintf(stderr, "("); 
            print_exp(term->value.ptr);
            fprintf(stderr, ")");
            break;

		case kind_method:
            print_extra_types(term->type);
            fprintf(stderr, "%s(", (char *)term->value.method.ID);
            print_list(term->value.method.actions, ", ", &print_exp);
            fprintf(stderr, ")");
			break;

        case kind_sqrt:
            fprintf(stderr, "sqrt(");
            print_exp(term->value.ptr);
            fprintf(stderr, ")");
            break;

		default:
			fprintf(stderr, "ERROR: unknown term");
            print_newline();
	}	      
}

