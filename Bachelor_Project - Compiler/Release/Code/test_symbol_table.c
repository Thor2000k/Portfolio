#include "symbol.h"
#include <stdio.h>

int scopeTest(){
    
    fprintf(stderr, "Testing scope_symbol\n");
    //creating root scope
    symbol_table *tbl = init_symbol_table();

    //creating leaf scopes and testing if succussful
    symbol_table *child1 = scope_symbol_table(tbl);
    symbol_table *child2 = scope_symbol_table(tbl);

    if(child1->parent != tbl)
        fprintf(stderr, "scope_symbol_table failed creating child1:\n ");
    else    
        fprintf(stderr, "child1 created \n");
    if(child2->parent != tbl)
        fprintf(stderr, "scope_symbol_table failed creating child2:\n ");
    else    
        fprintf(stderr, "child2 created \n");

    fprintf(stderr, "\n");
    fprintf(stderr, "starting put/get test on multiple tables \n");

    // inserting values in root table
    symbol *x = put_symbol(tbl,"x",10);

    //symbol *q = put_symbol(tbl,"q",9);
    put_symbol(tbl,"q",9);

    // inserting values in child1
    symbol *x1 = put_symbol(child1,"x",8);
    
    fprintf(stderr, "Show that both symbols are in the expected scopes \n");
    dump_symbol_table(child1);

    // inserting values in child2
    put_symbol(child2,"y",7);
    symbol *z = put_symbol(child2,"z",6);

    // looking for symbol x from child1 
    fprintf(stderr, "Get identifier x from child1 \n");    
    symbol *x2 = get_symbol(child1,"x");
    if(x2 == x1)
        fprintf(stderr, "x found in child1: \n");
    else
        fprintf(stderr, "x NOT found in child 1: \n");

    // looking for x from child2
    fprintf(stderr, "Get identifier x in parent \n");
    symbol *y2 = get_symbol(child2,"x");
    if(y2 == x)
        fprintf(stderr, "x found in root node from child2: \n");
    else
        fprintf(stderr, "x NOT found from child2: \n");
    
    // looking for z from child1 in child2. Test should fail (out of scope)
    
    fprintf(stderr, "Try to get identifier from a different child scope \n");
    symbol *z1 = get_symbol(child1, "z");
    if(z1 == z)
        fprintf(stderr, "z found from child1: (test failed) \n");
    else
        fprintf(stderr, "z  NOT found from child1: (test success) \n");

    fprintf(stderr, "\n");
    fprintf(stderr, "------------------\n");

    return 0;
}

void dubplicateTest()
{ 
    // test putting same identifier
    fprintf(stderr, "Testing put existing identifier\n");
    symbol_table *tbl = init_symbol_table();
    put_symbol(tbl,"abc",10);
    put_symbol(tbl,"abc",1);

    // test changing value of identifier
    fprintf(stderr, "Testing assign \n");
    sym_assign(tbl,"abc",5);
    dump_symbol_table(tbl);
}

void largeTest()
{
    // Complex table
    fprintf(stderr, "\n=======================\n");
    fprintf(stderr, "Testing complex table\n");
    symbol_table *root = init_symbol_table();
    symbol_table *depth1[3];
    symbol_table *depth2[4];
    symbol_table *depth3[2];

    depth1[0] = scope_symbol_table(root);
    depth1[1] = scope_symbol_table(root);
    depth1[2] = scope_symbol_table(root);
    
    depth2[0] = scope_symbol_table(depth1[0]);
    depth2[1] = scope_symbol_table(depth1[1]);
    depth2[2] = scope_symbol_table(depth1[1]);
    depth2[3] = scope_symbol_table(depth1[1]);
    
    depth3[0] = scope_symbol_table(depth2[2]);
    depth3[1] = scope_symbol_table(depth2[2]);

    // try to put a symbol with a negative value
    fprintf(stderr, "put negative value\n");
    symbol *a = put_symbol(root, "a", -2);
    dump_symbol_table(root);

    // put a symbol with same name start as another
    put_symbol(depth2[2], "aa", 5);

    symbol *bb = put_symbol(root, "bb", 100);
    put_symbol(depth2[1], "b", 3);

    symbol *ret;

    fprintf(stderr, "traverse empty tables\n");
    ret = get_symbol(depth3[0], "a");
    if(ret == a)
        fprintf(stderr, "a found in root node from depth3[0]: \n");
    else
        fprintf(stderr, "a NOT found from depth3[0]: \n");    
    
    fprintf(stderr, "get symbol with substring id of another symbol\n");
    ret = get_symbol(depth2[2], "a");
    if(ret == a)
        fprintf(stderr, "a found in root node from depth2[2]: \n");
    else
        fprintf(stderr, "a NOT found from depth2[2]: \n");

    fprintf(stderr, "get symbol with superstring id of another symbol \n");
    ret = get_symbol(depth2[1], "bb");
    if(ret == bb)
        fprintf(stderr, "bb found in root node from depth2[1]: \n");
    else
        fprintf(stderr, "bb NOT found from depth2[1]: \n");
}

int main()
{
    // Testing put_symbol() and get_symbol();
    fprintf(stderr, "Testing put in single table\n");
    symbol_table *tbl = init_symbol_table();
    put_symbol(tbl, "abc", 10);

    dump_symbol_table(tbl);

    fprintf(stderr, "Testing get in single table\n");
    symbol *ptr = get_symbol(tbl, "abc");

    if (ptr == NULL)
        fprintf(stderr, "symbol: NULL\n");
    else
        fprintf(stderr, "symbol: (%s, %d)\n", ptr->name, ptr->value);


    fprintf(stderr, "Testing put on child table\n");
    symbol_table *tbl2 = scope_symbol_table(tbl);
    put_symbol(tbl2, "abc", 2);

    dump_symbol_table(tbl2);

    scopeTest();
    dubplicateTest();
    largeTest();
    return 0; 
}

