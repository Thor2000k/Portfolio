/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "parser.y" /* yacc.c:339  */

#include <stdio.h>
#include "tree.h"
#include "array_list.h"

extern char *yytext;
extern body_t *asm_tree;
extern int lineCount;
extern int yylex();
extern int yyparse();
 
void yyerror() {
   fprintf(stderr, "syntax error in line %d before %s.\n", lineCount, yytext); 
}

#line 82 "parser.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.tab.h".  */
#ifndef YY_YY_INCLUDE_PARSER_TAB_H_INCLUDED
# define YY_YY_INCLUDE_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 18 "parser.y" /* yacc.c:355  */

#include "tree.h"

#line 116 "parser.tab.c" /* yacc.c:355  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    FUNC = 258,
    RETURN = 259,
    END = 260,
    TYPE = 261,
    VAR = 262,
    INT = 263,
    BOOL = 264,
    ARRAY_OF = 265,
    RECORD_OF = 266,
    ALLOCATE = 267,
    LENGTH = 268,
    WRITE = 269,
    READ = 270,
    IF = 271,
    THEN = 272,
    ELSE = 273,
    WHILE = 274,
    DO = 275,
    COPY = 276,
    FOR = 277,
    TRUE = 278,
    FALSE = 279,
    NIL = 280,
    ID = 281,
    NUM = 282,
    ABS = 283,
    NEG = 284,
    SQRT = 285,
    PLUS = 286,
    MINUS = 287,
    MULT = 288,
    DIV = 289,
    EQUAL = 290,
    NEQ = 291,
    LEQ = 292,
    GEQ = 293,
    GE = 294,
    LE = 295,
    OR = 296,
    AND = 297,
    MODULO = 298,
    LCURL = 299,
    RCURL = 300,
    LPAREN = 301,
    RPAREN = 302,
    LKPAREN = 303,
    RKPAREN = 304,
    ASSIGNMENT = 305,
    DOT = 306,
    COMMA = 307,
    COLON = 308,
    SEMICOLON = 309
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 26 "parser.y" /* yacc.c:355  */

    int intconst;
    char *str;
    array_list *list;
    function_t *function;
    head_t *head;
    type_t *type;
    var_type_t *var_type;
    body_t *body;
    declaration_t *declaration;
    statement_t *statement;
    variable_t *variable;
    exp_t *exp;
    term_t *term;

#line 199 "parser.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_INCLUDE_PARSER_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 216 "parser.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   433

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  55
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  22
/* YYNRULES -- Number of rules.  */
#define YYNRULES  75
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  162

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   309

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   144,   144,   148,   151,   154,   157,   159,   161,   163,
     165,   168,   171,   173,   175,   178,   181,   184,   187,   189,
     191,   193,   196,   198,   201,   203,   206,   208,   210,   212,
     214,   216,   218,   220,   222,   232,   243,   245,   247,   249,
     251,   267,   269,   272,   274,   276,   278,   281,   283,   285,
     287,   289,   291,   293,   295,   297,   299,   301,   303,   305,
     307,   315,   318,   320,   322,   324,   326,   328,   330,   332,
     334,   336,   339,   342,   344,   346
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "\"func\"", "\"return\"", "\"end\"",
  "\"type\"", "\"var\"", "\"int\"", "\"bool\"", "\"array of\"",
  "\"record of\"", "\"allocate\"", "\"of length\"", "\"write\"",
  "\"read\"", "\"if\"", "\"then\"", "\"else\"", "\"while\"", "\"do\"",
  "\"copy\"", "\"for\"", "\"true\"", "\"false\"", "\"null\"",
  "\"identifier\"", "NUM", "\"|\"", "\"!\"", "\"sqrt\"", "\"+\"", "\"-\"",
  "\"*\"", "\"/\"", "\"==\"", "\"!=\"", "\"<=\"", "\">=\"", "\">\"",
  "\"<\"", "\"||\"", "\"&&\"", "\"%\"", "\"{\"", "\"}\"", "\"(\"", "\")\"",
  "\"[\"", "\"]\"", "\"=\"", "\".\"", "\",\"", "\":\"", "\";\"", "$accept",
  "program", "function", "head", "tail", "type", "par_list", "var_list",
  "var_type", "body", "decl_list", "declaration", "statement_list",
  "statement_sub", "statement_line", "statement_compl", "statement_opt",
  "variable", "exp", "term", "act_list", "exp_list", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309
};
# endif

#define YYPACT_NINF -75

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-75)))

#define YYTABLE_NINF -33

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-33)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -75,     3,   -75,    86,   -75,    13,   227,    14,    18,    22,
     227,    22,   227,   227,    22,   154,   -75,   141,   -75,   -75,
     -75,   141,   -75,   -37,   -75,   133,     7,   -75,   -75,   -75,
       8,   -75,   227,   238,    11,   238,   227,   -16,   364,   -75,
      25,    27,    -9,   -75,    43,   364,   -16,   169,   193,    63,
     141,   -75,    41,    15,    91,   -75,   -75,    72,    74,   227,
     227,    78,    18,   227,   351,   -75,   227,   -75,   317,   227,
     227,   227,   227,   227,   227,   227,   227,   227,   227,   227,
     227,   227,    12,    12,    18,   -75,   227,   141,   141,   227,
     105,   227,   -75,    92,   -75,   -75,   -75,   298,   364,   -75,
      75,    71,   364,    88,    84,   -75,   334,   -75,   -18,   -18,
     -75,   -75,   156,   156,   156,   156,   156,   156,   377,   390,
     -75,   -75,   -75,    12,    93,   -75,    85,   -75,   -75,   364,
     120,   -75,    62,   254,   -75,   206,   -75,   -75,    87,   -75,
     227,   -75,   -75,    18,   -75,   141,    22,   154,    12,   364,
     -19,   -75,    77,   121,   -75,   -75,   227,   141,   276,   -75,
     227,   364
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      18,     0,     2,     0,     1,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    43,    44,     0,    20,    18,
      17,    16,    23,     0,    25,     0,     0,    68,    69,    70,
      44,    67,     0,     0,     0,     0,     0,    62,    26,    61,
       0,     0,     0,    14,    29,    27,    28,     0,     0,     0,
       0,    41,     0,     0,     0,    22,    24,     0,     0,     0,
       0,     0,    12,    73,     0,    65,     0,    60,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    21,     0,     0,     0,     0,
       0,     0,    36,     0,     3,    34,    35,     0,    31,    46,
       0,    11,    75,     0,    72,    66,     0,    64,    47,    48,
      49,    50,    55,    56,    54,    53,    51,    52,    58,    57,
      59,     7,     8,     0,     0,     6,     0,    15,    13,    30,
      37,    39,    62,     0,    42,     0,     5,    45,     0,    63,
       0,    71,     9,     0,    19,     0,     0,    43,     0,    74,
       0,    38,     0,     0,     4,    10,     0,     0,     0,    40,
       0,    33
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -75,   -75,   -75,   -75,   -75,   -74,   -75,   -55,    67,   128,
     -75,   -75,    -4,   -11,   -14,   -75,    23,    -3,    -8,    64,
     -75,   -75
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,    18,    19,    94,   126,   100,    42,    43,     2,
       3,    20,    21,    22,    23,    24,    52,    37,    38,    39,
     103,   104
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      25,    51,    45,     4,    47,    48,    44,   101,    46,   127,
      55,    49,    25,    53,    25,    71,    72,    56,    25,     6,
     121,   122,   123,   124,    64,    81,   155,     9,    68,    10,
      11,    12,    59,    84,    13,    61,    14,    15,   125,    26,
      40,    16,    55,    84,    41,    85,    90,    25,    16,   142,
      25,    97,    98,    62,    63,   102,    86,    66,   106,    17,
      92,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   154,    82,   130,   131,   129,    55,
      83,   133,   -32,   135,    25,    25,   132,    25,   150,     5,
       6,    59,     7,     8,    61,    91,    93,    65,     9,    67,
      10,    11,    12,    95,    99,    13,    96,    14,    15,     6,
      59,    59,    16,    61,    61,    89,   -32,     9,   136,    10,
      11,    12,   138,    84,    13,    59,    14,    15,    61,   156,
      17,    16,   149,    51,   151,   139,   140,   143,   145,   144,
     148,   157,    25,   152,    25,     6,   159,    54,   158,    17,
     134,   128,   161,     9,    25,    10,    11,    12,     6,     0,
      13,     0,    14,    15,    57,    58,     9,    16,    10,    11,
     153,     0,     0,     0,     0,    14,     0,     0,     0,     0,
      16,    59,     0,    60,    61,    17,    87,    69,    70,    71,
      72,   -33,   -33,   -33,   -33,   -33,   -33,     0,    50,    81,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      27,    28,    29,    30,    31,    32,    33,    34,     0,    35,
     147,    27,    28,    29,    30,    31,    32,    33,    34,     0,
       0,     0,     0,    36,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    36,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,     0,     0,
       0,     0,     0,     0,     0,     0,   146,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
       0,     0,     0,     0,     0,     0,     0,     0,   160,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,     0,     0,     0,     0,     0,   137,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,     0,     0,     0,   107,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,     0,   105,
       0,   141,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,     0,    80,
      81,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,     0,     0,    81
};

static const yytype_int16 yycheck[] =
{
       3,    15,    10,     0,    12,    13,     9,    62,    11,    83,
      21,    14,    15,    17,    17,    33,    34,    54,    21,     4,
       8,     9,    10,    11,    32,    43,    45,    12,    36,    14,
      15,    16,    48,    52,    19,    51,    21,    22,    26,    26,
      26,    26,    53,    52,    26,    54,    50,    50,    26,   123,
      53,    59,    60,    46,    46,    63,    13,    46,    66,    44,
      45,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,   148,    50,    87,    88,    86,    90,
      53,    89,    20,    91,    87,    88,    89,    90,   143,     3,
       4,    48,     6,     7,    51,    54,     5,    33,    12,    35,
      14,    15,    16,    31,    26,    19,    32,    21,    22,     4,
      48,    48,    26,    51,    51,    52,    54,    12,    26,    14,
      15,    16,    47,    52,    19,    48,    21,    22,    51,    52,
      44,    26,   140,   147,   145,    47,    52,    44,    18,    54,
      53,    20,   145,   146,   147,     4,   157,    19,   156,    44,
      45,    84,   160,    12,   157,    14,    15,    16,     4,    -1,
      19,    -1,    21,    22,    31,    32,    12,    26,    14,    15,
     147,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,
      26,    48,    -1,    50,    51,    44,    17,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    44,    43,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      54,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    46,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    52,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    -1,    -1,    -1,    -1,    -1,    49,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    -1,    -1,    -1,    47,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    -1,    28,
      -1,    47,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    -1,    42,
      43,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    56,    64,    65,     0,     3,     4,     6,     7,    12,
      14,    15,    16,    19,    21,    22,    26,    44,    57,    58,
      66,    67,    68,    69,    70,    72,    26,    23,    24,    25,
      26,    27,    28,    29,    30,    32,    46,    72,    73,    74,
      26,    26,    62,    63,    72,    73,    72,    73,    73,    72,
      44,    69,    71,    67,    64,    68,    54,    31,    32,    48,
      50,    51,    46,    46,    73,    74,    46,    74,    73,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    50,    53,    52,    54,    13,    17,    20,    52,
      67,    54,    45,     5,    59,    31,    32,    73,    73,    26,
      61,    62,    73,    75,    76,    28,    73,    47,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      73,     8,     9,    10,    11,    26,    60,    60,    63,    73,
      68,    68,    72,    73,    45,    73,    26,    49,    47,    47,
      52,    47,    60,    44,    54,    18,    52,    54,    53,    73,
      62,    68,    72,    71,    60,    45,    52,    20,    73,    68,
      52,    73
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    55,    56,    57,    58,    59,    60,    60,    60,    60,
      60,    61,    61,    62,    62,    63,    64,    65,    65,    66,
      66,    66,    67,    67,    68,    68,    69,    69,    69,    69,
      69,    69,    69,    69,    69,    69,    70,    70,    70,    70,
      70,    71,    71,    71,    72,    72,    72,    73,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    73,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    75,    75,    76,    76
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     3,     7,     2,     1,     1,     1,     2,
       4,     1,     0,     3,     1,     3,     2,     2,     0,     5,
       1,     3,     2,     1,     2,     1,     2,     2,     2,     2,
       4,     3,     4,    10,     3,     3,     3,     4,     6,     4,
       8,     1,     3,     0,     1,     4,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     1,     1,     4,     3,     2,     3,     1,     1,     1,
       1,     4,     1,     0,     3,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 145 "parser.y" /* yacc.c:1646  */
    { asm_tree = (yyvsp[0].body);}
#line 1461 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 149 "parser.y" /* yacc.c:1646  */
    { (yyval.function) = make_function((yyvsp[-2].head), (yyvsp[-1].body), (yyvsp[0].str)); }
#line 1467 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 152 "parser.y" /* yacc.c:1646  */
    { (yyval.head) = make_head((yyvsp[-5].str), (yyvsp[-3].list), (yyvsp[0].type)); }
#line 1473 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 155 "parser.y" /* yacc.c:1646  */
    { (yyval.str) = (yyvsp[0].str); }
#line 1479 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 158 "parser.y" /* yacc.c:1646  */
    { (yyval.type) = make_type((yyvsp[0].str), kind_t_identifier); }
#line 1485 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 160 "parser.y" /* yacc.c:1646  */
    { (yyval.type) = make_type(NULL, kind_t_integer); }
#line 1491 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 162 "parser.y" /* yacc.c:1646  */
    { (yyval.type) = make_type(NULL, kind_t_boolean); }
#line 1497 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 164 "parser.y" /* yacc.c:1646  */
    { (yyval.type) = make_type((yyvsp[0].type), kind_t_array); }
#line 1503 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 166 "parser.y" /* yacc.c:1646  */
    { (yyval.type) = make_type((yyvsp[-1].list), kind_t_record); }
#line 1509 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 169 "parser.y" /* yacc.c:1646  */
    { (yyval.list) = (yyvsp[0].list); }
#line 1515 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 171 "parser.y" /* yacc.c:1646  */
    { (yyval.list) = NULL; }
#line 1521 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 174 "parser.y" /* yacc.c:1646  */
    { (yyval.list) = append_list((yyvsp[-2].list), (yyvsp[0].var_type)); }
#line 1527 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 176 "parser.y" /* yacc.c:1646  */
    { (yyval.list) = append_list(NULL, (yyvsp[0].var_type)); }
#line 1533 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 179 "parser.y" /* yacc.c:1646  */
    { (yyval.var_type) = make_var_type((yyvsp[-2].str), (yyvsp[0].type)); }
#line 1539 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 182 "parser.y" /* yacc.c:1646  */
    { (yyval.body) = make_body((yyvsp[-1].list), (yyvsp[0].list)); }
#line 1545 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 185 "parser.y" /* yacc.c:1646  */
    { (yyval.list) = append_list((yyvsp[-1].list), (yyvsp[0].declaration)); }
#line 1551 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 187 "parser.y" /* yacc.c:1646  */
    { (yyval.list) = NULL; }
#line 1557 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 190 "parser.y" /* yacc.c:1646  */
    { (yyval.declaration) = make_declaration((yyvsp[-3].str), (yyvsp[-1].type), kind_typedef); }
#line 1563 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 192 "parser.y" /* yacc.c:1646  */
    { (yyval.declaration) = make_declaration((yyvsp[0].function), NULL, kind_function); }
#line 1569 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 194 "parser.y" /* yacc.c:1646  */
    { (yyval.declaration) = make_declaration((yyvsp[-1].list), NULL, kind_var_list); }
#line 1575 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 197 "parser.y" /* yacc.c:1646  */
    { (yyval.list) = append_list((yyvsp[-1].list), (yyvsp[0].statement)); }
#line 1581 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 199 "parser.y" /* yacc.c:1646  */
    { (yyval.list) = append_list(NULL, (yyvsp[0].statement)); }
#line 1587 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 202 "parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[-1].statement); }
#line 1593 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 204 "parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].statement); }
#line 1599 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 207 "parser.y" /* yacc.c:1646  */
    { (yyval.statement) = make_statement(kind_return, 1, (yyvsp[0].exp)); }
#line 1605 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 209 "parser.y" /* yacc.c:1646  */
    { (yyval.statement) = make_statement(kind_write, 1, (yyvsp[0].exp)); }
#line 1611 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 211 "parser.y" /* yacc.c:1646  */
    { (yyval.statement) = make_statement(kind_read, 1, (yyvsp[0].variable)); }
#line 1617 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 213 "parser.y" /* yacc.c:1646  */
    { (yyval.statement) = make_statement(kind_allocate_record, 1, (yyvsp[0].variable)); }
#line 1623 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 215 "parser.y" /* yacc.c:1646  */
    { (yyval.statement) = make_statement(kind_allocate_array, 2, (yyvsp[-2].variable), (yyvsp[0].exp)); }
#line 1629 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 217 "parser.y" /* yacc.c:1646  */
    { (yyval.statement) = make_statement(kind_assign, 2, (yyvsp[-2].variable), (yyvsp[0].exp)); }
#line 1635 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 219 "parser.y" /* yacc.c:1646  */
    { (yyval.statement) = make_statement(kind_copy, 2, (yyvsp[-2].variable), (yyvsp[0].variable)); }
#line 1641 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 221 "parser.y" /* yacc.c:1646  */
    { (yyval.statement) = make_statement(kind_copy_from, 5, (yyvsp[-8].variable), (yyvsp[-6].exp), (yyvsp[-4].variable), (yyvsp[-2].exp), (yyvsp[0].exp)); }
#line 1647 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 223 "parser.y" /* yacc.c:1646  */
    {  
                        int i = 1; 
                        term_t *term_one = make_term(&i, NULL, kind_integer);
                        term_t *term_var = make_term((yyvsp[-2].variable), NULL, kind_variable);
                        exp_t *left = make_exp(term_var, NULL, kind_term);
                        exp_t *right = make_exp(term_one, NULL, kind_term);
                        exp_t *res = make_exp(left, right, kind_plus);
                        (yyval.statement) = make_statement(kind_assign, 2, (yyvsp[-2].variable), res);
                    }
#line 1661 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 233 "parser.y" /* yacc.c:1646  */
    {
                        int i = 1; 
                        term_t *term_one = make_term(&i, NULL, kind_integer);
                        term_t *term_var = make_term((yyvsp[-2].variable), NULL, kind_variable);
                        exp_t *left = make_exp(term_var, NULL, kind_term);
                        exp_t *right = make_exp(term_one, NULL, kind_term);
                        exp_t *res = make_exp(left, right, kind_minus);
                        (yyval.statement) = make_statement(kind_assign, 2, (yyvsp[-2].variable), res);
                    }
#line 1675 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 244 "parser.y" /* yacc.c:1646  */
    { (yyval.statement) = make_statement(kind_statement_list, 1, (yyvsp[-1].list)); }
#line 1681 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 246 "parser.y" /* yacc.c:1646  */
    { (yyval.statement) = make_statement(kind_if, 2, (yyvsp[-2].exp), (yyvsp[0].statement)); }
#line 1687 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 248 "parser.y" /* yacc.c:1646  */
    { (yyval.statement) = make_statement(kind_if_else, 3, (yyvsp[-4].exp), (yyvsp[-2].statement), (yyvsp[0].statement)); }
#line 1693 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 250 "parser.y" /* yacc.c:1646  */
    { (yyval.statement) = make_statement(kind_while, 2, (yyvsp[-2].exp), (yyvsp[0].statement)); }
#line 1699 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 252 "parser.y" /* yacc.c:1646  */
    {
                        array_list *list_inner = AL_INIT_PTR(2);
                        array_list *list_outer = AL_INIT_PTR(2);

                        al_add(list_inner, &(yyvsp[0].statement));
                        al_add(list_inner, &(yyvsp[-2].statement));
                        statement_t *inner_statement = make_statement(kind_statement_list, 1, list_inner);
                        statement_t *while_statement = make_statement(kind_while, 2, (yyvsp[-4].exp), inner_statement);

                        al_add(list_outer, &(yyvsp[-6].statement));
                        al_add(list_outer, &while_statement);

                        (yyval.statement) = make_statement(kind_statement_list, 1, list_outer);
                    }
#line 1718 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 268 "parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].statement); }
#line 1724 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 270 "parser.y" /* yacc.c:1646  */
    { (yyval.statement) = make_statement(kind_statement_list, 1, (yyvsp[-1].list)); }
#line 1730 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 272 "parser.y" /* yacc.c:1646  */
    { (yyval.statement) = NULL; }
#line 1736 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 275 "parser.y" /* yacc.c:1646  */
    { (yyval.variable) = make_variable((yyvsp[0].str), NULL, kind_id); }
#line 1742 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 277 "parser.y" /* yacc.c:1646  */
    { (yyval.variable) = make_variable((yyvsp[-3].variable), (yyvsp[-1].exp), kind_index); }
#line 1748 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 279 "parser.y" /* yacc.c:1646  */
    { (yyval.variable) = make_variable((yyvsp[-2].variable), (yyvsp[0].str), kind_field); }
#line 1754 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 282 "parser.y" /* yacc.c:1646  */
    { (yyval.exp) = make_exp((yyvsp[-2].exp), (yyvsp[0].exp), kind_plus); }
#line 1760 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 284 "parser.y" /* yacc.c:1646  */
    { (yyval.exp) = make_exp((yyvsp[-2].exp), (yyvsp[0].exp), kind_minus); }
#line 1766 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 286 "parser.y" /* yacc.c:1646  */
    { (yyval.exp) = make_exp((yyvsp[-2].exp), (yyvsp[0].exp), kind_multiply); }
#line 1772 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 288 "parser.y" /* yacc.c:1646  */
    { (yyval.exp) = make_exp((yyvsp[-2].exp), (yyvsp[0].exp), kind_divide); }
#line 1778 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 290 "parser.y" /* yacc.c:1646  */
    { (yyval.exp) = make_exp((yyvsp[-2].exp), (yyvsp[0].exp), kind_greater); }
#line 1784 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 292 "parser.y" /* yacc.c:1646  */
    { (yyval.exp) = make_exp((yyvsp[-2].exp), (yyvsp[0].exp), kind_less); }
#line 1790 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 294 "parser.y" /* yacc.c:1646  */
    { (yyval.exp) = make_exp((yyvsp[-2].exp), (yyvsp[0].exp), kind_greater_equal); }
#line 1796 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 296 "parser.y" /* yacc.c:1646  */
    { (yyval.exp) = make_exp((yyvsp[-2].exp), (yyvsp[0].exp), kind_less_equal); }
#line 1802 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 298 "parser.y" /* yacc.c:1646  */
    { (yyval.exp) = make_exp((yyvsp[-2].exp), (yyvsp[0].exp), kind_equal); }
#line 1808 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 300 "parser.y" /* yacc.c:1646  */
    { (yyval.exp) = make_exp((yyvsp[-2].exp), (yyvsp[0].exp), kind_not_equal); }
#line 1814 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 302 "parser.y" /* yacc.c:1646  */
    { (yyval.exp) = make_exp((yyvsp[-2].exp), (yyvsp[0].exp), kind_and); }
#line 1820 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 304 "parser.y" /* yacc.c:1646  */
    { (yyval.exp) = make_exp((yyvsp[-2].exp), (yyvsp[0].exp), kind_or); }
#line 1826 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 306 "parser.y" /* yacc.c:1646  */
    { (yyval.exp) = make_exp((yyvsp[-2].exp), (yyvsp[0].exp), kind_modulo); }
#line 1832 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 308 "parser.y" /* yacc.c:1646  */
    { 
                        int i = 0;
                        term_t *term = make_term(&i, NULL, kind_integer);
                        exp_t *left = make_exp(term, NULL, kind_term);
                        exp_t *right = make_exp((yyvsp[0].term), NULL, kind_term);
                        (yyval.exp) = make_exp(left, right, kind_minus);
                    }
#line 1844 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 316 "parser.y" /* yacc.c:1646  */
    { (yyval.exp) = make_exp((yyvsp[0].term), NULL, kind_term); }
#line 1850 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 319 "parser.y" /* yacc.c:1646  */
    { (yyval.term) = make_term((yyvsp[0].variable), NULL, kind_variable); }
#line 1856 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 321 "parser.y" /* yacc.c:1646  */
    { (yyval.term) = make_term((yyvsp[-3].str), (yyvsp[-1].list), kind_method); }
#line 1862 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 323 "parser.y" /* yacc.c:1646  */
    { (yyval.term) = make_term((yyvsp[-1].exp), NULL, kind_parenthess); }
#line 1868 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 325 "parser.y" /* yacc.c:1646  */
    { (yyval.term) = make_term((yyvsp[0].term), NULL, kind_negate); }
#line 1874 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 327 "parser.y" /* yacc.c:1646  */
    { (yyval.term) = make_term((yyvsp[-1].exp), NULL, kind_absolute); }
#line 1880 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 329 "parser.y" /* yacc.c:1646  */
    { (yyval.term) = make_term(&(yyvsp[0].intconst), NULL, kind_integer); }
#line 1886 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 331 "parser.y" /* yacc.c:1646  */
    { (yyval.term) = make_term(NULL, NULL, kind_true); }
#line 1892 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 333 "parser.y" /* yacc.c:1646  */
    { (yyval.term) = make_term(NULL, NULL, kind_false); }
#line 1898 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 335 "parser.y" /* yacc.c:1646  */
    { (yyval.term) = make_term(NULL, NULL, kind_null); }
#line 1904 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 337 "parser.y" /* yacc.c:1646  */
    { (yyval.term) = make_term((yyvsp[-1].exp), NULL, kind_sqrt); }
#line 1910 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 340 "parser.y" /* yacc.c:1646  */
    { (yyval.list) = (yyvsp[0].list); }
#line 1916 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 342 "parser.y" /* yacc.c:1646  */
    { (yyval.list) = NULL; }
#line 1922 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 345 "parser.y" /* yacc.c:1646  */
    { (yyval.list) = append_list((yyvsp[-2].list), (yyvsp[0].exp)); }
#line 1928 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 347 "parser.y" /* yacc.c:1646  */
    { (yyval.list) = append_list(NULL, (yyvsp[0].exp)); }
#line 1934 "parser.tab.c" /* yacc.c:1646  */
    break;


#line 1938 "parser.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 350 "parser.y" /* yacc.c:1906  */


