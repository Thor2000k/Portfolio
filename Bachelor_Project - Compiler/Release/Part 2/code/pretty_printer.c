#include "pretty_printer.h"
#include "tree.h"
#include "array_list.h"

#include <stdio.h>

int indent_increment = 4;
int indent_size = 0;

void print_indent()
{  
    PRINT_DEBUG(print_indent)

    if (indent_size > 0)
        fprintf(stderr, "%*s", indent_size, "");
}

// Kan benyttes af alle lister
void print_list(array_list *list, char *delim, void (*print_f)(void *node))
{
    PRINT_DEBUG(print_list)

    if (list == NULL)
    {
        return;
    }

    int size = list->size;
    void *ptr;

    for (int i = size - 1; i >= 0; i--)
    {
        AL_GET(void *, ptr, list, i);
        print_f(ptr);
        if (i > 0)
        {
            fprintf(stderr, "%s", delim);   
        } 
    }
}

void print_function(void *v_function)
{
    PRINT_DEBUG(print_function)
    function_t *function = v_function;

    fprintf(stderr, "\n");
    print_indent();

    fprintf(stderr, "(");
    print_head(function->head);

    NEXT_SCOPE(print_body(function->body))

    print_indent();
    fprintf(stderr, "end %s\n)\n", function->head->ID); 
}

void print_head(void *v_head)
{
    PRINT_DEBUG(print_head)
    head_t *head = v_head;

    fprintf(stderr,"%s(", head->ID);
    print_list(head->par_list, ", ",  &print_var_type );
    fprintf(stderr, "):");
    print_type(head->return_type);
    fprintf(stderr, "\n");
}

void print_type(void *v_type)
{
    PRINT_DEBUG(print_type)
    type_t *type = v_type;

	switch(type->kind){
		case kind_t_integer:
			fprintf(stderr, "int");
            break;

		case kind_t_boolean:
			fprintf(stderr, "bool");
            break;
            
		case kind_t_array:
			fprintf(stderr, "array of ");
            print_type(type->reference);
            break;
            
		case kind_t_record:
			fprintf(stderr, "record of { ");
            print_list(type->reference, ", ", &print_var_type);
			fprintf(stderr, " }");
            break;
            
		case kind_t_identifier:
			fprintf(stderr, "%s", (char *)type->reference);
            break;
            
		default:
			fprintf(stderr, "ERROR: unknown type\n");	
            break;            	
  }
}

void print_var_type(void *v_var_type)
{
    PRINT_DEBUG(print_var_type)
    var_type_t *var_type = v_var_type;

    fprintf(stderr,"%s:", var_type->ID);
    print_type(var_type->type);
}

// Eksempel på udskriving af lister
void print_body(void *v_body)
{
    PRINT_DEBUG(print_body)
    body_t *body = v_body;

    if (body == NULL)
        return;

    print_list(body->decl_list, "", &print_declaration);
    print_list(body->stat_list, "", &print_statement);
}

void print_declaration(void *v_declaration)
{
    PRINT_DEBUG(print_declaration)
    declaration_t *declaration = v_declaration;
    
    PREV_SCOPE(print_indent())
    
    switch(declaration->kind)
    {
        case kind_typedef:
            fprintf(stderr, "type %s = ", (char *)declaration->value.assign_type.ID);
            print_type(declaration->value.assign_type.type);
            fprintf(stderr, ";\n");
            
            break;
        
        case kind_function:
            print_function(declaration->value.function);
            break;
        
        case kind_var_list:
            fprintf(stderr, "var ");
            print_list(declaration->value.var_list , ", ", &print_var_type );
            fprintf(stderr, ";\n");
            break;
        
        default:
			fprintf(stderr, "ERROR: unknown declaration\n");
            break;  
    }
}

void print_statement(void *v_statement)
{
    PRINT_DEBUG(print_statement)
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
            print_exp(statement->value.expression);
            fprintf(stderr, ";\n");
            break;

        case kind_write:
            fprintf(stderr, "write ");
            print_exp(statement->value.expression);
            fprintf(stderr, ";\n");
            break;

        case kind_allocate_record:
            fprintf(stderr, "allocate ");
            print_variable(statement->value.variable);
            fprintf(stderr, ";\n");
            break;

        case kind_allocate_array:
            fprintf(stderr, "allocate ");
            print_variable(statement->value.allocate_array.variable);

            fprintf(stderr, " of length ");
            print_exp(statement->value.allocate_array.expression);

            fprintf(stderr, ";\n");
            break;

        case kind_assign:
            print_variable(statement->value.assign.variable);
            fprintf(stderr, " = ");
            print_exp(statement->value.assign.expression);

            fprintf(stderr, ";\n");
            break;

        case kind_if:
            fprintf(stderr, "(if ");
            print_exp(statement->value.if_statement.expression);
            fprintf(stderr, " then\n");
            
            NEXT_SCOPE(print_statement(statement->value.if_statement.statement))
            
            print_indent();
            fprintf(stderr, ")\n");
            break;
            
        case kind_if_else:
            fprintf(stderr, "(if ");
            print_exp(statement->value.if_else.expression);
            fprintf(stderr, " then\n");

            NEXT_SCOPE(print_statement(statement->value.if_else.if_statement))

            print_indent();
            fprintf(stderr, "else\n");
            
            NEXT_SCOPE(print_statement(statement->value.if_else.else_statement))

            print_indent();
            fprintf(stderr, ")\n");
            break;

        case kind_while:
            fprintf(stderr, "(while ");
            print_exp(statement->value.while_statement.expression);
            fprintf(stderr, " do\n");
            
            NEXT_SCOPE(print_statement(statement->value.while_statement.statement))
            
            print_indent();
            fprintf(stderr, ")\n");
            break;

        case kind_statement_list:
            fprintf(stderr, "{\n");    

            if (indent_size == 0)
                indent_size += indent_increment;   

            print_list(statement->value.statement_list, "", &print_statement);
        
            if (indent_size != indent_increment)
            {
                PREV_SCOPE(print_indent())
            }
            
            fprintf(stderr, "}\n");

            break;

        default:
			fprintf(stderr, "ERROR: unknown statement\n");
            break;
    }
}

// Eksempel på udskrivining
void print_variable(void *v_variable)
{
    PRINT_DEBUG(print_variable)
    variable_t *variable = v_variable;

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
			fprintf(stderr, "ERROR: unknown variable\n");
            break;
    }

    fprintf(stderr, ")");
}

void print_exp(void *v_expression)
{
    PRINT_DEBUG(print_exp)

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

        default:
			fprintf(stderr, "ERROR: unknown expression\n");
            break;
    }

    fprintf(stderr, ")");
}

void print_term(void *v_term)
{
    PRINT_DEBUG(print_term)
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
            fprintf(stderr, "%s(", (char *)term->value.method.ID);
            print_list(term->value.method.actions, ", ", &print_exp);
            fprintf(stderr, ")");
			break;

		default:
			fprintf(stderr, "ERROR: unknown term\n");
	}	      
}

