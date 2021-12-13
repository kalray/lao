%token IDENTIFIER TYPE_NAME INT_LITERAL
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token TYPEDEF STRUCT UNION ENUM ELLIPSIS

%%

parse_struct_definition						/* BD3	*/
	: struct_introduction '{' struct_declaration_list '}' ';'
	| struct_introduction '{' struct_declaration_list '}' init_declarator_list ';'
	;

struct_introduction						/* BD3	*/
	: STRUCT IDENTIFIER
	| TYPEDEF STRUCT IDENTIFIER
	;

init_declarator_list
	: init_declarator
	| init_declarator_list ',' init_declarator
	;

init_declarator
	: declarator
/*	| declarator '=' initializer					*/
	;

struct_declaration_list
	: struct_declaration
	| struct_declaration_list struct_declaration
	;

struct_declaration
	: specifier_qualifier_list struct_declarator_list ';'
	| identifier_qualifier_list struct_declarator_list ';'	/* BD3	*/
	;

specifier_qualifier_list
	: type_specifier
	| type_specifier specifier_qualifier_list
/*	| type_qualifier						*/
	| type_qualifier specifier_qualifier_list
	;

identifier_qualifier_list					/* BD3	*/
	: IDENTIFIER
	| type_qualifier identifier_qualifier_list
	;

struct_declarator_list
	: struct_declarator
	| struct_declarator_list ',' struct_declarator
	;

struct_declarator
	: declarator
	| ':' constant_expression
	| declarator ':' constant_expression
	;

declarator
	: pointer direct_declarator
	| direct_declarator
	;

pointer
	: '*'
	| '*' type_qualifier_list
	| '*' pointer
	| '*' type_qualifier_list pointer
	;

type_qualifier_list
	: type_qualifier
	| type_qualifier_list type_qualifier
	;

type_qualifier
	: CONST
	| VOLATILE
	;

direct_declarator
	: IDENTIFIER
	| '(' declarator ')'
	| direct_declarator '[' constant_expression ']'
	| direct_declarator '[' ']'
	| direct_declarator '(' parameter_type_list ')'
/*	| direct_declarator '(' identifier_list ')'			*/
	| direct_declarator '(' ')'
	;

parameter_type_list
	: parameter_list
	| parameter_list ',' ELLIPSIS
	;

parameter_list
	: parameter_declaration
	| parameter_list ',' parameter_declaration
	;

parameter_declaration
	: declaration_specifiers declarator
	| declaration_specifiers abstract_declarator
	| declaration_specifiers
	| identifier_specifier declarator			/* BD3	*/
	| identifier_specifier abstract_declarator		/* BD3	*/
	;

declaration_specifiers
	: type_specifier
	| type_specifier declaration_specifiers
/*	| type_qualifier						*/
	| type_qualifier declaration_specifiers	
/*	| storage_class_specifier					*/
/*	| storage_class_specifier declaration_specifiers		*/
	;

identifier_specifier						/* BD3	*/
	: IDENTIFIER
	| type_qualifier identifier_specifier
	;

type_specifier
	: VOID
	| CHAR
	| SHORT
	| INT
	| LONG
	| FLOAT
	| DOUBLE
	| SIGNED
	| UNSIGNED
	| struct_or_union_specifier
	| enum_specifier
	| TYPE_NAME
	;

abstract_declarator
	: pointer
	| direct_abstract_declarator
	| pointer direct_abstract_declarator
	;

direct_abstract_declarator
	: '(' abstract_declarator ')'
	| '[' ']'
	| '[' constant_expression ']'
	| direct_abstract_declarator '[' ']'
	| direct_abstract_declarator '[' constant_expression ']'
	| '(' ')'
	| '(' parameter_type_list ')'
	| direct_abstract_declarator '(' ')'
	| direct_abstract_declarator '(' parameter_type_list ')'
	;

struct_or_union_specifier
	: struct_or_union IDENTIFIER
/*	| struct_or_union IDENTIFIER '{' struct_declaration_list '}'	*/
/*	| struct_or_union '{' struct_declaration_list '}'		*/
	;

struct_or_union
	: STRUCT
	| UNION
	;

enum_specifier
	: ENUM IDENTIFIER
/*	| ENUM IDENTIFIER '{' enumerator_list '}'			*/
/*	| ENUM '{' enumerator_list '}'					*/
	;

constant_expression						/* BD3 */
	: INT_LITERAL
	| IDENTIFIER
	| IDENTIFIER '+' INT_LITERAL
	| IDENTIFIER '-' INT_LITERAL
	;

%%
