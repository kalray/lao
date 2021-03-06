# Parse the language described in Declare.output.
# Perl source code automatically generated by yaxcc.pl.
# Source code copyright 1995 M-J. Dominus (mjd@pobox.com)

# code included from 
#
#	Semantic actions for the XCC C declaration parser.
#
#	This program is free software; you can redistribute it and/or modify
#	it under the terms of the GNU Lesser General Public License (LGPL)
#	version 2 as published by the Free Software Foundation.

package Declare;

#
# Scanner code: export &yydebug, &yylval, &yyin, &yylex.
#
$yydebug = 0;
sub yydebug {
  $yydebug = shift;
}
$yylval = undef;
@yylval = ();
sub yylval {
  $yylkeep =  shift;
  if (defined $yylkeep) {
    shift @yylval while @yylval > $yylkeep;
  }
  @yylval;
}
@yyin = ();
sub yyin {
  my @buffer = @_;
  my $buffer = join("\n", @buffer);
  # strip C comments from $buffer
  $buffer =~ s/\/\*.*?\*\///sg;
  $buffer =~ s/\/\/.*//g;
  $buffer =~ s/^\#.*//mg;
  push @yyin, grep { length($_) } split '\b|\s+', $buffer;
}
sub yylex {
  my $token;
  return '' unless (@yyin);
  my $value = shift @yyin;
  if ($value =~ s/^(\.\.\.)//) {
    ($token, $yylval) = ('ELLIPSIS', $1);
    unshift @yyin, $value if length($value);
  } elsif ($value =~ s/^(\W)//) {
    unshift @yyin, $value if length($value);
    ($token, $yylval) = ($1, $1);
  } elsif ($value =~ s/^(\d+)//) {
    unshift @yyin, $value if length($value);
    ($token, $yylval) = ('INT_LITERAL', $1);
  } elsif ($value =~ /^(char)$/) {
    ($token, $yylval) = (uc($1), $1);
  } elsif ($value =~ /^(short)$/) {
    ($token, $yylval) = (uc($1), $1);
  } elsif ($value =~ /^(int)$/) {
    ($token, $yylval) = (uc($1), $1);
  } elsif ($value =~ /^(long)$/) {
    ($token, $yylval) = (uc($1), $1);
  } elsif ($value =~ /^(signed)$/) {
    ($token, $yylval) = (uc($1), $1);
  } elsif ($value =~ /^(unsigned)$/) {
    ($token, $yylval) = (uc($1), $1);
  } elsif ($value =~ /^(float)$/) {
    ($token, $yylval) = (uc($1), $1);
  } elsif ($value =~ /^(double)$/) {
    ($token, $yylval) = (uc($1), $1);
  } elsif ($value =~ /^(const)$/) {
    ($token, $yylval) = (uc($1), $1);
  } elsif ($value =~ /^(volatile)$/) {
    ($token, $yylval) = (uc($1), $1);
  } elsif ($value =~ /^(void)$/) {
    ($token, $yylval) = (uc($1), $1);
  } elsif ($value =~ /^(struct)$/) {
    ($token, $yylval) = (uc($1), $1);
  } elsif ($value =~ /^(union)$/) {
    ($token, $yylval) = (uc($1), $1);
  } elsif ($value =~ /^(enum)$/) {
    ($token, $yylval) = (uc($1), $1);
  } elsif ($value =~ /^(typedef)$/) {
    ($token, $yylval) = (uc($1), $1);
  } else {
    ($token, $yylval) = ('IDENTIFIER', $value);
  }
  push @yylval, $yylval if $yylkeep;
  shift @yylval if @yylval > $yylkeep;
  $token;
}

sub pretty {
  my $type = shift;
  if (ref $type eq 'HASH') {
    return "ARRAY[".$$type{DIMENSION}."](".&pretty($$type{ARRAY}).")" if $$type{ARRAY};
    return "POINTER{".$$type{QUALIFIER}."}(".&pretty($$type{POINTER}).")" if $$type{POINTER};
    return "FUNCTION(".&pretty($$type{FUNCTION}).")" if $$type{FUNCTION};
  }
  return $type;
}

@struct = ();
sub struct {
  @struct;
}

#
# Begin semantic actions
#

# generating or using semantic actions from 
@sem = (
# rule 0
sub { },
# rule 1
# parse_struct_definition -> struct_introduction '{' struct_declaration_list '}' ';'
sub {
  @struct = ($_[0], @{$_[2]});
  undef;
},
# rule 2
# parse_struct_definition -> struct_introduction '{' struct_declaration_list '}' init_declarator_list ';'
sub {
  @struct = ($_[0], @{$_[2]});
  undef;
},
# rule 3
# struct_introduction -> STRUCT IDENTIFIER
sub {
  $_[1];
},
# rule 4
# struct_introduction -> TYPEDEF STRUCT IDENTIFIER
sub {
  $_[2];
},
# rule 5
# init_declarator_list -> init_declarator
sub {
  undef;
},
# rule 6
# init_declarator_list -> init_declarator_list ',' init_declarator
sub {
  undef;
},
# rule 7
# init_declarator -> declarator
sub {
  undef;
},
# rule 8
# struct_declaration_list -> struct_declaration
sub {
  [ $_[0] ];
},
# rule 9
# struct_declaration_list -> struct_declaration_list struct_declaration
sub {
  [ @{$_[0]}, $_[1] ];
},
# rule 10
# struct_declaration -> specifier_qualifier_list struct_declarator_list ';'
sub {
  { DECLARATION => $_[0], DECLARATORS => $_[1] };
},
# rule 11
# struct_declaration -> identifier_qualifier_list struct_declarator_list ';'
sub {
  { DECLARATION => $_[0], DECLARATORS => $_[1] };
},
# rule 12
# specifier_qualifier_list -> type_specifier
sub {
  $_[0];
},
# rule 13
# specifier_qualifier_list -> type_specifier specifier_qualifier_list
sub {
  "$_[0] $_[1]";
},
# rule 14
# specifier_qualifier_list -> type_qualifier specifier_qualifier_list
sub {
  "$_[0] $_[1]";
},
# rule 15
# identifier_qualifier_list -> IDENTIFIER
sub {
  $_[0];
},
# rule 16
# identifier_qualifier_list -> type_qualifier identifier_qualifier_list
sub {
  "$_[0] $_[1]";
},
# rule 17
# struct_declarator_list -> struct_declarator
sub {
  [ $_[0] ];
},
# rule 18
# struct_declarator_list -> struct_declarator_list ',' struct_declarator
sub {
  [ @{$_[0]}, $_[2] ];
},
# rule 19
# struct_declarator -> declarator
sub {
  $_[0];
},
# rule 20
# struct_declarator -> ':' constant_expression
sub {
  undef;
},
# rule 21
# struct_declarator -> declarator ':' constant_expression
sub {
  $_[0];
},
# rule 22
# declarator -> pointer direct_declarator
sub {
  my $pointer = $_[0];
  $pointer = $$pointer{POINTER} while ($$pointer{POINTER});
  $$pointer{POINTER} = $_[1];
  $_[0];
},
# rule 23
# declarator -> direct_declarator
sub {
  $_[0];
},
# rule 24
# pointer -> '*'
sub {
  { POINTER => '', QUALIFIER => '' };
},
# rule 25
# pointer -> '*' type_qualifier_list
sub {
  { POINTER => '', QUALIFIER => $_[1] };
},
# rule 26
# pointer -> '*' pointer
sub {
  { POINTER => $_[1], QUALIFIER => '' }
},
# rule 27
# pointer -> '*' type_qualifier_list pointer
sub {
  { POINTER => $_[2], QUALIFIER => $_[1] }
},
# rule 28
# type_qualifier_list -> type_qualifier
sub {
  $_[0];
},
# rule 29
# type_qualifier_list -> type_qualifier_list type_qualifier
sub {
  "$_[0] $_[1]";
},
# rule 30
# type_qualifier -> CONST
sub {
  $_[0];
},
# rule 31
# type_qualifier -> VOLATILE
sub {
  $_[0];
},
# rule 32
# direct_declarator -> IDENTIFIER
sub {
  $_[0];
},
# rule 33
# direct_declarator -> '(' declarator ')'
sub {
  $_[1];
},
# rule 34
# direct_declarator -> direct_declarator '[' constant_expression ']'
sub {
  { ARRAY => $_[0], DIMENSION => $_[2] };
},
# rule 35
# direct_declarator -> direct_declarator '[' ']'
sub {
  { ARRAY => $_[0] };
},
# rule 36
# direct_declarator -> direct_declarator '(' parameter_type_list ')'
sub {
  { FUNCTION => $_[0] };
},
# rule 37
# direct_declarator -> direct_declarator '(' ')'
sub {
  { FUNCTION => $_[0] };
},
# rule 38
# parameter_type_list -> parameter_list
sub {
  undef;
},
# rule 39
# parameter_type_list -> parameter_list ',' ELLIPSIS
sub {
  undef;
},
# rule 40
# parameter_list -> parameter_declaration
sub {
  undef;
},
# rule 41
# parameter_list -> parameter_list ',' parameter_declaration
sub {
  undef;
},
# rule 42
# parameter_declaration -> declaration_specifiers declarator
sub {
  undef;
},
# rule 43
# parameter_declaration -> declaration_specifiers abstract_declarator
sub {
  undef;
},
# rule 44
# parameter_declaration -> declaration_specifiers
sub {
  undef;
},
# rule 45
# parameter_declaration -> identifier_specifier declarator
sub {
  undef;
},
# rule 46
# parameter_declaration -> identifier_specifier abstract_declarator
sub {
  undef;
},
# rule 47
# declaration_specifiers -> type_specifier
sub {
  $_[0];
},
# rule 48
# declaration_specifiers -> type_specifier declaration_specifiers
sub {
  "$_[0] $_[1]";
},
# rule 49
# declaration_specifiers -> type_qualifier declaration_specifiers
sub {
  "$_[0] $_[1]";
},
# rule 50
# identifier_specifier -> IDENTIFIER
sub {
  $_[0];
},
# rule 51
# identifier_specifier -> type_qualifier identifier_specifier
sub {
  "$_[0] $_[1]";
},
# rule 52
# type_specifier -> VOID
sub {
  $_[0];
},
# rule 53
# type_specifier -> CHAR
sub {
  $_[0];
},
# rule 54
# type_specifier -> SHORT
sub {
  $_[0];
},
# rule 55
# type_specifier -> INT
sub {
  $_[0];
},
# rule 56
# type_specifier -> LONG
sub {
  $_[0];
},
# rule 57
# type_specifier -> FLOAT
sub {
  $_[0];
},
# rule 58
# type_specifier -> DOUBLE
sub {
  $_[0];
},
# rule 59
# type_specifier -> SIGNED
sub {
  $_[0];
},
# rule 60
# type_specifier -> UNSIGNED
sub {
  $_[0];
},
# rule 61
# type_specifier -> struct_or_union_specifier
sub {
  $_[0];
},
# rule 62
# type_specifier -> enum_specifier
sub {
  $_[0];
},
# rule 63
# type_specifier -> TYPE_NAME
sub {
  $_[0];
},
# rule 64
# abstract_declarator -> pointer
sub {
  undef;
},
# rule 65
# abstract_declarator -> direct_abstract_declarator
sub {
  undef;
},
# rule 66
# abstract_declarator -> pointer direct_abstract_declarator
sub {
  undef;
},
# rule 67
# direct_abstract_declarator -> '(' abstract_declarator ')'
sub {
  undef;
},
# rule 68
# direct_abstract_declarator -> '[' ']'
sub {
  undef;
},
# rule 69
# direct_abstract_declarator -> '[' constant_expression ']'
sub {
  undef;
},
# rule 70
# direct_abstract_declarator -> direct_abstract_declarator '[' ']'
sub {
  undef;
},
# rule 71
# direct_abstract_declarator -> direct_abstract_declarator '[' constant_expression ']'
sub {
  undef;
},
# rule 72
# direct_abstract_declarator -> '(' ')'
sub {
  undef;
},
# rule 73
# direct_abstract_declarator -> '(' parameter_type_list ')'
sub {
  undef;
},
# rule 74
# direct_abstract_declarator -> direct_abstract_declarator '(' ')'
sub {
  undef;
},
# rule 75
# direct_abstract_declarator -> direct_abstract_declarator '(' parameter_type_list ')'
sub {
  undef;
},
# rule 76
# struct_or_union_specifier -> struct_or_union IDENTIFIER
sub {
  "$_[0] $_[1]";
},
# rule 77
# struct_or_union -> STRUCT
sub {
  $_[0];
},
# rule 78
# struct_or_union -> UNION
sub {
  $_[0];
},
# rule 79
# enum_specifier -> ENUM IDENTIFIER
sub {
  "$_[0] $_[1]";
},
# rule 80
# constant_expression -> INT_LITERAL
sub {
  $_[0];
},
# rule 81
# constant_expression -> IDENTIFIER
sub {
  $_[0];
},
# rule 82
# constant_expression -> IDENTIFIER '+' INT_LITERAL
sub {
  "$_[0]+$_[2]";
},
# rule 83
# constant_expression -> IDENTIFIER '-' INT_LITERAL
sub {
  "$_[0]-$_[2]";
},
);

@act = (
{# state 0
  'STRUCT'=>['shift','2'],
  'struct_introduction'=>['goto','4'],
  'parse_struct_definition'=>['goto','3'],
  'TYPEDEF'=>['shift','1'],
},
{# state 1
  'STRUCT'=>['shift','5'],
},
{# state 2
  'IDENTIFIER'=>['shift','6'],
},
{# state 3
  '$end'=>['shift','7'],
},
{# state 4
  '{'=>['shift','8'],
},
{# state 5
  'IDENTIFIER'=>['shift','9'],
},
{# state 6
  '$default'=>['reduce','3'],
},
{# state 7
  '$default'=>['accept','undef'],
},
{# state 8
  'CONST'=>['shift','20'],
  'UNION'=>['shift','24'],
  'type_qualifier'=>['goto','30'],
  'INT'=>['shift','14'],
  'enum_specifier'=>['goto','34'],
  'SIGNED'=>['shift','16'],
  'struct_or_union_specifier'=>['goto','32'],
  'CHAR'=>['shift','12'],
  'STRUCT'=>['shift','23'],
  'ENUM'=>['shift','25'],
  'SHORT'=>['shift','13'],
  'VOLATILE'=>['shift','21'],
  'UNSIGNED'=>['shift','17'],
  'specifier_qualifier_list'=>['goto','28'],
  'TYPE_NAME'=>['shift','11'],
  'struct_or_union'=>['goto','33'],
  'identifier_qualifier_list'=>['goto','29'],
  'LONG'=>['shift','15'],
  'VOID'=>['shift','22'],
  'struct_declaration_list'=>['goto','26'],
  'FLOAT'=>['shift','18'],
  'DOUBLE'=>['shift','19'],
  'IDENTIFIER'=>['shift','10'],
  'struct_declaration'=>['goto','27'],
  'type_specifier'=>['goto','31'],
},
{# state 9
  '$default'=>['reduce','4'],
},
{# state 10
  '$default'=>['reduce','15'],
},
{# state 11
  '$default'=>['reduce','63'],
},
{# state 12
  '$default'=>['reduce','53'],
},
{# state 13
  '$default'=>['reduce','54'],
},
{# state 14
  '$default'=>['reduce','55'],
},
{# state 15
  '$default'=>['reduce','56'],
},
{# state 16
  '$default'=>['reduce','59'],
},
{# state 17
  '$default'=>['reduce','60'],
},
{# state 18
  '$default'=>['reduce','57'],
},
{# state 19
  '$default'=>['reduce','58'],
},
{# state 20
  '$default'=>['reduce','30'],
},
{# state 21
  '$default'=>['reduce','31'],
},
{# state 22
  '$default'=>['reduce','52'],
},
{# state 23
  '$default'=>['reduce','77'],
},
{# state 24
  '$default'=>['reduce','78'],
},
{# state 25
  'IDENTIFIER'=>['shift','35'],
},
{# state 26
  'CONST'=>['shift','20'],
  'STRUCT'=>['shift','23'],
  'CHAR'=>['shift','12'],
  'SIGNED'=>['shift','16'],
  'SHORT'=>['shift','13'],
  'identifier_qualifier_list'=>['goto','29'],
  'type_specifier'=>['goto','31'],
  'struct_or_union'=>['goto','33'],
  'type_qualifier'=>['goto','30'],
  '}'=>['shift','36'],
  'struct_declaration'=>['goto','37'],
  'IDENTIFIER'=>['shift','10'],
  'INT'=>['shift','14'],
  'LONG'=>['shift','15'],
  'VOID'=>['shift','22'],
  'enum_specifier'=>['goto','34'],
  'struct_or_union_specifier'=>['goto','32'],
  'UNSIGNED'=>['shift','17'],
  'TYPE_NAME'=>['shift','11'],
  'UNION'=>['shift','24'],
  'ENUM'=>['shift','25'],
  'specifier_qualifier_list'=>['goto','28'],
  'VOLATILE'=>['shift','21'],
  'FLOAT'=>['shift','18'],
  'DOUBLE'=>['shift','19'],
},
{# state 27
  '$default'=>['reduce','8'],
},
{# state 28
  '('=>['shift','41'],
  'IDENTIFIER'=>['shift','38'],
  'direct_declarator'=>['goto','46'],
  ':'=>['shift','39'],
  '*'=>['shift','40'],
  'pointer'=>['goto','45'],
  'struct_declarator_list'=>['goto','42'],
  'struct_declarator'=>['goto','43'],
  'declarator'=>['goto','44'],
},
{# state 29
  ':'=>['shift','39'],
  'IDENTIFIER'=>['shift','38'],
  '*'=>['shift','40'],
  '('=>['shift','41'],
  'struct_declarator'=>['goto','43'],
  'declarator'=>['goto','44'],
  'struct_declarator_list'=>['goto','47'],
  'pointer'=>['goto','45'],
  'direct_declarator'=>['goto','46'],
},
{# state 30
  'struct_or_union_specifier'=>['goto','32'],
  'type_qualifier'=>['goto','30'],
  'IDENTIFIER'=>['shift','10'],
  'DOUBLE'=>['shift','19'],
  'LONG'=>['shift','15'],
  'SIGNED'=>['shift','16'],
  'FLOAT'=>['shift','18'],
  'TYPE_NAME'=>['shift','11'],
  'ENUM'=>['shift','25'],
  'UNSIGNED'=>['shift','17'],
  'CHAR'=>['shift','12'],
  'VOLATILE'=>['shift','21'],
  'specifier_qualifier_list'=>['goto','48'],
  'INT'=>['shift','14'],
  'VOID'=>['shift','22'],
  'SHORT'=>['shift','13'],
  'CONST'=>['shift','20'],
  'enum_specifier'=>['goto','34'],
  'struct_or_union'=>['goto','33'],
  'type_specifier'=>['goto','31'],
  'identifier_qualifier_list'=>['goto','49'],
  'STRUCT'=>['shift','23'],
  'UNION'=>['shift','24'],
},
{# state 31
  'DOUBLE'=>['shift','19'],
  'STRUCT'=>['shift','23'],
  'CONST'=>['shift','20'],
  'INT'=>['shift','14'],
  'UNSIGNED'=>['shift','17'],
  'LONG'=>['shift','15'],
  'UNION'=>['shift','24'],
  'specifier_qualifier_list'=>['goto','50'],
  'SIGNED'=>['shift','16'],
  'FLOAT'=>['shift','18'],
  'SHORT'=>['shift','13'],
  'enum_specifier'=>['goto','34'],
  'struct_or_union'=>['goto','33'],
  'VOID'=>['shift','22'],
  'type_specifier'=>['goto','31'],
  'ENUM'=>['shift','25'],
  'type_qualifier'=>['goto','51'],
  '$default'=>['reduce','12'],
  'TYPE_NAME'=>['shift','11'],
  'struct_or_union_specifier'=>['goto','32'],
  'VOLATILE'=>['shift','21'],
  'CHAR'=>['shift','12'],
},
{# state 32
  '$default'=>['reduce','61'],
},
{# state 33
  'IDENTIFIER'=>['shift','52'],
},
{# state 34
  '$default'=>['reduce','62'],
},
{# state 35
  '$default'=>['reduce','79'],
},
{# state 36
  'direct_declarator'=>['goto','46'],
  'IDENTIFIER'=>['shift','38'],
  '*'=>['shift','40'],
  'init_declarator'=>['goto','55'],
  ';'=>['shift','53'],
  'pointer'=>['goto','45'],
  'declarator'=>['goto','56'],
  'init_declarator_list'=>['goto','54'],
  '('=>['shift','41'],
},
{# state 37
  '$default'=>['reduce','9'],
},
{# state 38
  '$default'=>['reduce','32'],
},
{# state 39
  'INT_LITERAL'=>['shift','58'],
  'IDENTIFIER'=>['shift','57'],
  'constant_expression'=>['goto','59'],
},
{# state 40
  'VOLATILE'=>['shift','21'],
  '*'=>['shift','40'],
  'CONST'=>['shift','20'],
  'type_qualifier'=>['goto','62'],
  'pointer'=>['goto','60'],
  'type_qualifier_list'=>['goto','61'],
  '$default'=>['reduce','24'],
},
{# state 41
  '('=>['shift','41'],
  'direct_declarator'=>['goto','46'],
  'pointer'=>['goto','45'],
  'declarator'=>['goto','63'],
  '*'=>['shift','40'],
  'IDENTIFIER'=>['shift','38'],
},
{# state 42
  ','=>['shift','65'],
  ';'=>['shift','64'],
},
{# state 43
  '$default'=>['reduce','17'],
},
{# state 44
  ':'=>['shift','66'],
  '$default'=>['reduce','19'],
},
{# state 45
  '('=>['shift','41'],
  'direct_declarator'=>['goto','67'],
  'IDENTIFIER'=>['shift','38'],
},
{# state 46
  '['=>['shift','69'],
  '('=>['shift','68'],
  '$default'=>['reduce','23'],
},
{# state 47
  ';'=>['shift','70'],
  ','=>['shift','65'],
},
{# state 48
  '$default'=>['reduce','14'],
},
{# state 49
  '$default'=>['reduce','16'],
},
{# state 50
  '$default'=>['reduce','13'],
},
{# state 51
  'struct_or_union'=>['goto','33'],
  'INT'=>['shift','14'],
  'UNION'=>['shift','24'],
  'DOUBLE'=>['shift','19'],
  'CHAR'=>['shift','12'],
  'ENUM'=>['shift','25'],
  'LONG'=>['shift','15'],
  'FLOAT'=>['shift','18'],
  'TYPE_NAME'=>['shift','11'],
  'enum_specifier'=>['goto','34'],
  'CONST'=>['shift','20'],
  'SHORT'=>['shift','13'],
  'STRUCT'=>['shift','23'],
  'VOLATILE'=>['shift','21'],
  'type_specifier'=>['goto','31'],
  'type_qualifier'=>['goto','51'],
  'UNSIGNED'=>['shift','17'],
  'struct_or_union_specifier'=>['goto','32'],
  'SIGNED'=>['shift','16'],
  'VOID'=>['shift','22'],
  'specifier_qualifier_list'=>['goto','48'],
},
{# state 52
  '$default'=>['reduce','76'],
},
{# state 53
  '$default'=>['reduce','1'],
},
{# state 54
  ','=>['shift','72'],
  ';'=>['shift','71'],
},
{# state 55
  '$default'=>['reduce','5'],
},
{# state 56
  '$default'=>['reduce','7'],
},
{# state 57
  '-'=>['shift','74'],
  '+'=>['shift','73'],
  '$default'=>['reduce','81'],
},
{# state 58
  '$default'=>['reduce','80'],
},
{# state 59
  '$default'=>['reduce','20'],
},
{# state 60
  '$default'=>['reduce','26'],
},
{# state 61
  'VOLATILE'=>['shift','21'],
  'CONST'=>['shift','20'],
  '*'=>['shift','40'],
  'type_qualifier'=>['goto','76'],
  '$default'=>['reduce','25'],
  'pointer'=>['goto','75'],
},
{# state 62
  '$default'=>['reduce','28'],
},
{# state 63
  ')'=>['shift','77'],
},
{# state 64
  '$default'=>['reduce','10'],
},
{# state 65
  'struct_declarator'=>['goto','78'],
  ':'=>['shift','39'],
  'IDENTIFIER'=>['shift','38'],
  '('=>['shift','41'],
  'pointer'=>['goto','45'],
  'declarator'=>['goto','44'],
  '*'=>['shift','40'],
  'direct_declarator'=>['goto','46'],
},
{# state 66
  'IDENTIFIER'=>['shift','57'],
  'constant_expression'=>['goto','79'],
  'INT_LITERAL'=>['shift','58'],
},
{# state 67
  '('=>['shift','68'],
  '['=>['shift','69'],
  '$default'=>['reduce','22'],
},
{# state 68
  'struct_or_union'=>['goto','33'],
  'parameter_list'=>['goto','84'],
  'DOUBLE'=>['shift','19'],
  'CONST'=>['shift','20'],
  'VOID'=>['shift','22'],
  'INT'=>['shift','14'],
  'parameter_declaration'=>['goto','85'],
  'type_qualifier'=>['goto','82'],
  'IDENTIFIER'=>['shift','80'],
  'UNION'=>['shift','24'],
  'declaration_specifiers'=>['goto','86'],
  'FLOAT'=>['shift','18'],
  'LONG'=>['shift','15'],
  'enum_specifier'=>['goto','34'],
  'struct_or_union_specifier'=>['goto','32'],
  'parameter_type_list'=>['goto','83'],
  'ENUM'=>['shift','25'],
  'type_specifier'=>['goto','88'],
  'VOLATILE'=>['shift','21'],
  ')'=>['shift','81'],
  'identifier_specifier'=>['goto','87'],
  'UNSIGNED'=>['shift','17'],
  'SHORT'=>['shift','13'],
  'STRUCT'=>['shift','23'],
  'SIGNED'=>['shift','16'],
  'CHAR'=>['shift','12'],
  'TYPE_NAME'=>['shift','11'],
},
{# state 69
  'IDENTIFIER'=>['shift','57'],
  'constant_expression'=>['goto','90'],
  ']'=>['shift','89'],
  'INT_LITERAL'=>['shift','58'],
},
{# state 70
  '$default'=>['reduce','11'],
},
{# state 71
  '$default'=>['reduce','2'],
},
{# state 72
  'IDENTIFIER'=>['shift','38'],
  '('=>['shift','41'],
  'declarator'=>['goto','56'],
  'direct_declarator'=>['goto','46'],
  '*'=>['shift','40'],
  'init_declarator'=>['goto','91'],
  'pointer'=>['goto','45'],
},
{# state 73
  'INT_LITERAL'=>['shift','92'],
},
{# state 74
  'INT_LITERAL'=>['shift','93'],
},
{# state 75
  '$default'=>['reduce','27'],
},
{# state 76
  '$default'=>['reduce','29'],
},
{# state 77
  '$default'=>['reduce','33'],
},
{# state 78
  '$default'=>['reduce','18'],
},
{# state 79
  '$default'=>['reduce','21'],
},
{# state 80
  '$default'=>['reduce','50'],
},
{# state 81
  '$default'=>['reduce','37'],
},
{# state 82
  'VOID'=>['shift','22'],
  'type_specifier'=>['goto','88'],
  'TYPE_NAME'=>['shift','11'],
  'enum_specifier'=>['goto','34'],
  'FLOAT'=>['shift','18'],
  'UNSIGNED'=>['shift','17'],
  'CONST'=>['shift','20'],
  'CHAR'=>['shift','12'],
  'INT'=>['shift','14'],
  'SHORT'=>['shift','13'],
  'type_qualifier'=>['goto','82'],
  'DOUBLE'=>['shift','19'],
  'LONG'=>['shift','15'],
  'UNION'=>['shift','24'],
  'struct_or_union_specifier'=>['goto','32'],
  'SIGNED'=>['shift','16'],
  'STRUCT'=>['shift','23'],
  'IDENTIFIER'=>['shift','80'],
  'declaration_specifiers'=>['goto','94'],
  'ENUM'=>['shift','25'],
  'VOLATILE'=>['shift','21'],
  'struct_or_union'=>['goto','33'],
  'identifier_specifier'=>['goto','95'],
},
{# state 83
  ')'=>['shift','96'],
},
{# state 84
  '$default'=>['reduce','38'],
  ','=>['shift','97'],
},
{# state 85
  '$default'=>['reduce','40'],
},
{# state 86
  '$default'=>['reduce','44'],
  'IDENTIFIER'=>['shift','38'],
  '['=>['shift','99'],
  'pointer'=>['goto','101'],
  'abstract_declarator'=>['goto','102'],
  'direct_abstract_declarator'=>['goto','103'],
  '*'=>['shift','40'],
  '('=>['shift','98'],
  'direct_declarator'=>['goto','46'],
  'declarator'=>['goto','100'],
},
{# state 87
  'IDENTIFIER'=>['shift','38'],
  'direct_abstract_declarator'=>['goto','103'],
  'direct_declarator'=>['goto','46'],
  'pointer'=>['goto','101'],
  '('=>['shift','98'],
  '['=>['shift','99'],
  'abstract_declarator'=>['goto','105'],
  '*'=>['shift','40'],
  'declarator'=>['goto','104'],
},
{# state 88
  'UNSIGNED'=>['shift','17'],
  'struct_or_union'=>['goto','33'],
  'type_qualifier'=>['goto','106'],
  'VOID'=>['shift','22'],
  'DOUBLE'=>['shift','19'],
  'LONG'=>['shift','15'],
  'declaration_specifiers'=>['goto','107'],
  'CONST'=>['shift','20'],
  'TYPE_NAME'=>['shift','11'],
  'UNION'=>['shift','24'],
  'STRUCT'=>['shift','23'],
  'FLOAT'=>['shift','18'],
  'struct_or_union_specifier'=>['goto','32'],
  'SIGNED'=>['shift','16'],
  '$default'=>['reduce','47'],
  'INT'=>['shift','14'],
  'ENUM'=>['shift','25'],
  'enum_specifier'=>['goto','34'],
  'SHORT'=>['shift','13'],
  'VOLATILE'=>['shift','21'],
  'CHAR'=>['shift','12'],
  'type_specifier'=>['goto','88'],
},
{# state 89
  '$default'=>['reduce','35'],
},
{# state 90
  ']'=>['shift','108'],
},
{# state 91
  '$default'=>['reduce','6'],
},
{# state 92
  '$default'=>['reduce','82'],
},
{# state 93
  '$default'=>['reduce','83'],
},
{# state 94
  '$default'=>['reduce','49'],
},
{# state 95
  '$default'=>['reduce','51'],
},
{# state 96
  '$default'=>['reduce','36'],
},
{# state 97
  'IDENTIFIER'=>['shift','80'],
  'parameter_declaration'=>['goto','110'],
  'CONST'=>['shift','20'],
  'FLOAT'=>['shift','18'],
  'enum_specifier'=>['goto','34'],
  'struct_or_union'=>['goto','33'],
  'UNSIGNED'=>['shift','17'],
  'ELLIPSIS'=>['shift','109'],
  'SHORT'=>['shift','13'],
  'CHAR'=>['shift','12'],
  'DOUBLE'=>['shift','19'],
  'VOLATILE'=>['shift','21'],
  'SIGNED'=>['shift','16'],
  'ENUM'=>['shift','25'],
  'type_qualifier'=>['goto','82'],
  'INT'=>['shift','14'],
  'identifier_specifier'=>['goto','87'],
  'UNION'=>['shift','24'],
  'declaration_specifiers'=>['goto','86'],
  'type_specifier'=>['goto','88'],
  'LONG'=>['shift','15'],
  'STRUCT'=>['shift','23'],
  'TYPE_NAME'=>['shift','11'],
  'struct_or_union_specifier'=>['goto','32'],
  'VOID'=>['shift','22'],
},
{# state 98
  'VOID'=>['shift','22'],
  '('=>['shift','98'],
  'parameter_type_list'=>['goto','113'],
  'declarator'=>['goto','63'],
  'STRUCT'=>['shift','23'],
  'UNION'=>['shift','24'],
  'pointer'=>['goto','101'],
  'CONST'=>['shift','20'],
  'VOLATILE'=>['shift','21'],
  'ENUM'=>['shift','25'],
  'TYPE_NAME'=>['shift','11'],
  'direct_declarator'=>['goto','46'],
  'UNSIGNED'=>['shift','17'],
  'INT'=>['shift','14'],
  ')'=>['shift','112'],
  'FLOAT'=>['shift','18'],
  'direct_abstract_declarator'=>['goto','103'],
  'declaration_specifiers'=>['goto','86'],
  'IDENTIFIER'=>['shift','111'],
  'DOUBLE'=>['shift','19'],
  'struct_or_union_specifier'=>['goto','32'],
  'parameter_list'=>['goto','84'],
  'struct_or_union'=>['goto','33'],
  '*'=>['shift','40'],
  '['=>['shift','99'],
  'SIGNED'=>['shift','16'],
  'parameter_declaration'=>['goto','85'],
  'SHORT'=>['shift','13'],
  'LONG'=>['shift','15'],
  'enum_specifier'=>['goto','34'],
  'identifier_specifier'=>['goto','87'],
  'CHAR'=>['shift','12'],
  'type_qualifier'=>['goto','82'],
  'type_specifier'=>['goto','88'],
  'abstract_declarator'=>['goto','114'],
},
{# state 99
  'IDENTIFIER'=>['shift','57'],
  ']'=>['shift','115'],
  'constant_expression'=>['goto','116'],
  'INT_LITERAL'=>['shift','58'],
},
{# state 100
  '$default'=>['reduce','42'],
},
{# state 101
  'direct_declarator'=>['goto','67'],
  'direct_abstract_declarator'=>['goto','117'],
  '$default'=>['reduce','64'],
  'IDENTIFIER'=>['shift','38'],
  '('=>['shift','98'],
  '['=>['shift','99'],
},
{# state 102
  '$default'=>['reduce','43'],
},
{# state 103
  '['=>['shift','119'],
  '$default'=>['reduce','65'],
  '('=>['shift','118'],
},
{# state 104
  '$default'=>['reduce','45'],
},
{# state 105
  '$default'=>['reduce','46'],
},
{# state 106
  'VOID'=>['shift','22'],
  'CONST'=>['shift','20'],
  'SIGNED'=>['shift','16'],
  'UNSIGNED'=>['shift','17'],
  'SHORT'=>['shift','13'],
  'struct_or_union_specifier'=>['goto','32'],
  'VOLATILE'=>['shift','21'],
  'CHAR'=>['shift','12'],
  'INT'=>['shift','14'],
  'TYPE_NAME'=>['shift','11'],
  'FLOAT'=>['shift','18'],
  'DOUBLE'=>['shift','19'],
  'enum_specifier'=>['goto','34'],
  'type_qualifier'=>['goto','106'],
  'STRUCT'=>['shift','23'],
  'declaration_specifiers'=>['goto','94'],
  'ENUM'=>['shift','25'],
  'UNION'=>['shift','24'],
  'LONG'=>['shift','15'],
  'struct_or_union'=>['goto','33'],
  'type_specifier'=>['goto','88'],
},
{# state 107
  '$default'=>['reduce','48'],
},
{# state 108
  '$default'=>['reduce','34'],
},
{# state 109
  '$default'=>['reduce','39'],
},
{# state 110
  '$default'=>['reduce','41'],
},
{# state 111
  '('=>['reduce','32'],
  'IDENTIFIER'=>['reduce','50'],
  '$default'=>['reduce','32'],
  '['=>['reduce','32'],
  '*'=>['reduce','50'],
},
{# state 112
  '$default'=>['reduce','72'],
},
{# state 113
  ')'=>['shift','120'],
},
{# state 114
  ')'=>['shift','121'],
},
{# state 115
  '$default'=>['reduce','68'],
},
{# state 116
  ']'=>['shift','122'],
},
{# state 117
  '('=>['shift','118'],
  '['=>['shift','119'],
  '$default'=>['reduce','66'],
},
{# state 118
  'UNION'=>['shift','24'],
  'enum_specifier'=>['goto','34'],
  'IDENTIFIER'=>['shift','80'],
  'VOLATILE'=>['shift','21'],
  'parameter_list'=>['goto','84'],
  'ENUM'=>['shift','25'],
  ')'=>['shift','123'],
  'CHAR'=>['shift','12'],
  'declaration_specifiers'=>['goto','86'],
  'type_qualifier'=>['goto','82'],
  'identifier_specifier'=>['goto','87'],
  'parameter_type_list'=>['goto','124'],
  'UNSIGNED'=>['shift','17'],
  'TYPE_NAME'=>['shift','11'],
  'SIGNED'=>['shift','16'],
  'DOUBLE'=>['shift','19'],
  'parameter_declaration'=>['goto','85'],
  'VOID'=>['shift','22'],
  'struct_or_union'=>['goto','33'],
  'FLOAT'=>['shift','18'],
  'type_specifier'=>['goto','88'],
  'INT'=>['shift','14'],
  'struct_or_union_specifier'=>['goto','32'],
  'STRUCT'=>['shift','23'],
  'CONST'=>['shift','20'],
  'SHORT'=>['shift','13'],
  'LONG'=>['shift','15'],
},
{# state 119
  ']'=>['shift','125'],
  'constant_expression'=>['goto','126'],
  'IDENTIFIER'=>['shift','57'],
  'INT_LITERAL'=>['shift','58'],
},
{# state 120
  '$default'=>['reduce','73'],
},
{# state 121
  '$default'=>['reduce','67'],
},
{# state 122
  '$default'=>['reduce','69'],
},
{# state 123
  '$default'=>['reduce','74'],
},
{# state 124
  ')'=>['shift','127'],
},
{# state 125
  '$default'=>['reduce','70'],
},
{# state 126
  ']'=>['shift','128'],
},
{# state 127
  '$default'=>['reduce','75'],
},
{# state 128
  '$default'=>['reduce','71'],
},
);

@length = (
2, 5, 6, 2, 3, 1, 3, 1, 1, 2, 3, 3, 1, 2, 2, 1, 2, 1, 3, 1, 2, 3, 2, 1, 1, 2, 2, 3, 1, 2, 1, 1, 1, 3, 4, 3, 4, 3, 1, 3, 1, 3, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 2, 1, 1, 2, 1, 1, 3, 3, );

@rhs = ('$accept', 'parse_struct_definition', 'parse_struct_definition', 'struct_introduction', 'struct_introduction', 'init_declarator_list', 'init_declarator_list', 'init_declarator', 'struct_declaration_list', 'struct_declaration_list', 'struct_declaration', 'struct_declaration', 'specifier_qualifier_list', 'specifier_qualifier_list', 'specifier_qualifier_list', 'identifier_qualifier_list', 'identifier_qualifier_list', 'struct_declarator_list', 'struct_declarator_list', 'struct_declarator', 'struct_declarator', 'struct_declarator', 'declarator', 'declarator', 'pointer', 'pointer', 'pointer', 'pointer', 'type_qualifier_list', 'type_qualifier_list', 'type_qualifier', 'type_qualifier', 'direct_declarator', 'direct_declarator', 'direct_declarator', 'direct_declarator', 'direct_declarator', 'direct_declarator', 'parameter_type_list', 'parameter_type_list', 'parameter_list', 'parameter_list', 'parameter_declaration', 'parameter_declaration', 'parameter_declaration', 'parameter_declaration', 'parameter_declaration', 'declaration_specifiers', 'declaration_specifiers', 'declaration_specifiers', 'identifier_specifier', 'identifier_specifier', 'type_specifier', 'type_specifier', 'type_specifier', 'type_specifier', 'type_specifier', 'type_specifier', 'type_specifier', 'type_specifier', 'type_specifier', 'type_specifier', 'type_specifier', 'type_specifier', 'abstract_declarator', 'abstract_declarator', 'abstract_declarator', 'direct_abstract_declarator', 'direct_abstract_declarator', 'direct_abstract_declarator', 'direct_abstract_declarator', 'direct_abstract_declarator', 'direct_abstract_declarator', 'direct_abstract_declarator', 'direct_abstract_declarator', 'direct_abstract_declarator', 'struct_or_union_specifier', 'struct_or_union', 'struct_or_union', 'enum_specifier', 'constant_expression', 'constant_expression', 'constant_expression', 'constant_expression', );

# py-skel.pl: perl yacc parser skeleton
# Copyright 1995 Mark-Jason Dominus (mjd@pobox.com).
# Hacked 2001 Benoit Dupont de Dinechin (benoit@dinechin.org):
#  - use my variables instead of local wherever possible
#  - replace the soft ref ${rule_$d} code by $sem[$d]
#  - replace %act by @act to avoid split while parsing
# Hacked 2002 Christophe Guillon
#  - handling of empty rules (reduce and go)

sub yyparse {
  my @states = ();			# State stack
  my @values = ();			# Value stack
  my ($state, $nextplease, $lookahead) = (0, 1);
  for (;;) {
    $lookahead = &yylex if $nextplease;
    my $act = ${$act[$state]}{$lookahead} ||
	${$act[$state]}{'$default'} ||
	${$act[$state]}{'$end'} ||
	${$act[$state]}{'$'};
    unless (defined($act)) {		# Wrong token
      until (${$act[$state]}{'error'}) {
	$state = pop @states || return 1;
      }
    }
    my ($a, $d) = @$act;
    if ($a eq 'shift') {
      push(@values, $yylval);
      push(@states, $state);
      $state = $d;
      $nextplease = 1;
    } elsif ($a eq 'reduce') {
      my @args = ();
      @args = splice(@values, -$length[$d]) if $length[$d] > 0; # CG: handling of empty rules.
      push (@values, &{$sem[$d]}(@args));
      push(@states, $state);					# CG: always push state.
      splice(@states, -$length[$d]) if $length[$d] > 0;		# CG: handling of empty rules, we stay in the same state.
      $state = $states[$#states];
      my $rhs = $rhs[$d];
      my $act = ${$act[$state]}{$rhs};
      my ($a, $d) = @$act;
      if ($a eq 'goto') {
	$state = $d;
      } else {
	print STDERR "Reduction to $rhs in state $state: ";
	print STDERR "action was $a, should have been a goto.\n";
	return 1;
      }
      $nextplease = 0;
    } elsif ($a eq 'goto') {
      $state = $d;
      $nextplease = 0;
    } elsif ($a eq 'accept') {
      return 0;
    } else {
      die "Unrecognized parsing action $a.\n";
    }
  }
}
#    END py-skel.pl

1;
