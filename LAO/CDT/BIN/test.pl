#
# Test for Declare parser
#

use Declare;

$test = <<'EOT';
struct Test_ {
  signed char c;
  unsigned short int h;
  long long int ll;
  unsigned long long int ull;
  void Action(Collision toCollision[Action_COUNT], char *alias);
  void Datum(DataRsrc datarsrc, int number);
  unsigned int *(*cmp)(void *, void *);
  unsigned int (**cmp)(void *, void *);
  unsigned int **cmp(void *, void *);
  unsigned int **cmp;
  int (*vararg)(char *, ...);
  void Table(CallB callb, int (*cmp)(void *a, void *b));
  ptrdiff_t Table_Entry_CMP(Table_Entry this, ptrdiff_t key, int (*cmp)(void *, void *));
  ptrdiff_t Table_Entry_CMP(Table_Entry this, int (*cmp)(void *, void *), ptrdiff_t key);
  struct List_Item_ *succ;
  const struct List_Item_ *const succ;
  const Bytes String_Entry_bytes(String_Entry this);
  char abc[10][20][30];
  char (*tabp)[20];
  char *tabp[SIZE];
  char (*t[20])[10];
  int a, b, *c;
};
EOT

$test2 = <<'EOT';
struct KO_ {
  void (*OP_update)(Operation, int, TempName[], int, TempName[], CGIR_OP);
  union {
    int a;
    void *p;
  } u1
};
EOT

&Declare::yyin($test);
&Declare::yylval(16);
&Declare::yydebug(1);
if (my $yyparse = &Declare::yyparse()) {
  my $text = join (' ', @Declare::yylval);
  print "syntax error: $text\n";
} else {
  my @struct = &Declare::struct();
  my $structname = shift @struct;
  print "STRUCT($structname)\n";
  foreach my $struct (@struct) {
    my $declaration = $$struct{DECLARATION};
    my $declarators = $$struct{DECLARATORS};
    foreach my $declarator (@{$declarators}) {
      my ($member, $typeop) = ($declarator, $declaration);
      $member =
	($typeop = '[]', $$member{ARRAY}) ||
	($typeop = '->', $$member{POINTER}) ||
	($typeop = '()', $$member{FUNCTION}) ||
	die "fatal error: unknown type operator\n"
	  while (ref $member);
      print "($member)$typeop\t";
      print "$declaration:\t", &Declare::pretty($declarator), "\n";
    }
  }
  print "ACCEPT\n";
}

