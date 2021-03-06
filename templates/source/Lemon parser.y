/*==========================================================================*/
/*
@@INCLUDE:$\templates\source\header-base@@
*/
/*--------------------------------------------------------------------------*/
/*
@@INCLUDE:$\templates\source\header-gnu@@
*/
/*--------------------------------------------------------------------------*/
/*
@@INCLUDE:$\templates\source\header-cvs@@
*/
/*==========================================================================*/
/*                             Parser settinds                              */
/*==========================================================================*/
// Parser name prefix
%name @@INPUT:Parser name\:@@
// Parser include directives
%include				{ #include <stdio.h>			}
// Parser stack size
%stack_size 1000
// Parser token prefix
%token_prefix TOKEN_
// Parser extra argument
%extra_argument				{ void* pArg				}
// Type of a terminal symbol
%token_type				{ Token*				}
// Default destructor for terminal symbol
%token_destructor			{ delete $$;				}
// Types of a non terminal symbols
%type expr				{ Expr*					}
// Default destructors for terminal symbols
%destructor expr			{ delete $$;				}
// Parser preceding sequence
%left ADD SUB.
%left MUL DIV MOD.
%right PLUS MINUS.
%nonassoc LBR RBR.
// Parser start symbol
%start_symbol start
/*==========================================================================*/
/*                          Parser grammar rules                            */
/*==========================================================================*/
@@HERE@@start		::= expr.
expr		::= NUMBER.
expr		::= LBR expr RBR.
expr		::= ADD expr. [PLUS]
expr		::= SUB expr. [MINUS]
expr		::= expr MUL expr.
expr		::= expr DIV expr.
expr		::= expr MOD expr.
expr		::= expr ADD expr.
expr		::= expr SUB expr.
/*==========================================================================*/
/*                          Parser error handling                           */
/*==========================================================================*/
// Accept directive specifies a block of C code that is executed whenever the parser accepts its input string
%parse_accept				{ printf("Parsing complete!\n");	}
// Failure directive specifies a block of C code that is executed whenever the parser fails complete
%parse_failure				{ printf("Parsing failed!\n");		}
// Stack overflow directive specifies a block of C code that is executed if the parser's internal stack ever overflows
%stack_overflow				{ printf("Stack overflow!\n");		}
/*==========================================================================*/
