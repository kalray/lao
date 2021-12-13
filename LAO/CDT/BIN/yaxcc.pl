# -*- perl -*-
#
#    pyacc: Generate tables for YACC parser in Perl
#    Copyright 1995 Mark-Jason Dominus (mjd@pobox.com)
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#

my @program = split /\//, $0;

use Getopt::Std;
my %option;
getopts('hdsa:', \%option);
&usage if $option{h};
delete $option{s} if $option{a};

my $infile = $ARGV[0] or &usage;

sub usage {
  print <<'EOT';

  yaxcc -- Bison-like tool for Perl adapted from M-J. Dominus py code.

USAGE

  yaxcc [-s] [-a parsing-actions-file ] [-d] bison-output-file > perl-parsing-code

  Use Bison 1.875 or higher to generate the bison-output-file (bison -v grammar.y).

  Each parsing action is a single paragraph of Perl code whose first line takes
  the form "#rule grammar-rule". Such code should return the value of the lhs
  grammar rule if any. Within this piece of code, the values of the rhs grammar
  rules are available as $_[0], $_[1]. etc. Example of a parsing action:

  #rule statement -> IF '(' expression ')' block_or_statement ELSE block_or_statement
    return &IF_THEN_ELSE($_[2], $_[4], $_[6]);

  The generated parser calls &yylex to get the next token and reads the
  corresponding token value in $yylval. At the end of input, &yylex must return
  the empty string ''. Usually, the scanner code is supplied in the
  parsing-actions-file so it ends up embedded in the perl-parsing-code.

OPTIONS

  -s Generate a skeleton for the parsing-actions-file.
  -a Specify the parsing-action-file to embed in the perl-parsing-code.
  -d Generate a debug parser whose messages are activated by setting $yydebug=1.

AUTHORS

  Mark-Jason Dominus
  Benoit Dupont de Dinechin
  Christophe Guillon

LICENSE

  GNU GPL 2 or higher.

EOT
  exit 1;
}

unless (open(Y , "< $infile")) {
  die "Couldn\'t open input file $infile for reading: $!";
}
*P = *STDOUT;

$rule[0] = $rhs[0] = "";		# BD3: old bison rules numbers start at 1.
my ($grammar, $oldrhs, $state);
local $; = " ";
while (<Y>) {
  chomp;
  /^\s*$/ and next;
  /^Grammar$/ and $grammar = 1, next;
  /^Grammaire$/ and $grammar = 1, next;
  /^Terminals,/ and $grammar = 0, next;
  /^Terminaux,/ and $grammar = 0, next;
  /^state (\d+)/ and $state = $1, next;
  /^état (\d+)/ and $state = $1, next;
  if ($grammar) {
    # We are reading the grammar rules.
    s/\/\*\s*empty\s*\*\//\$empty/;		# Normalize /* empty */ annotation.
    my ($ruleno, $rhs, $lhs);
    # Rules and reductions are converses, so the
    # rule's left side is the reduction's result.
    # if the rule says
    #  exp -> exp '+' exp
    # that means that 
    #  exp '+' exp
    # can be reduced to exp.  So the RHS of the reduction
    # is on the left here.
    if (/^rule (\d+)\s+(\$?\w+)\s+\-\>\s+(.*)$/) {
      # Old-style Bison grammar rule.
      ($ruleno, $rhs, $lhs) = ($1, $2, $3);
      print "#rule $rhs -> $lhs\n\n" if $option{s};
    } elsif (/^\s+(\d+)\s+(\$?\w+):\s+(.*)$/) {
      # New-style Bison grammar rule.
      ($ruleno, $rhs, $lhs) = ($1, $2, $3);
      print "#rule $rhs -> $lhs\n\n" if $option{s};
    } elsif (/^\s+(\d+)\s+\|\s+(.*)$/) {
      ($ruleno, $rhs, $lhs) = ($1, $oldrhs, $2);
      print "#rule $rhs -> $lhs\n\n" if $option{s};
    } else {
      warn "could not parse $_";
    }
    if (defined $ruleno) {
      my @lhs = grep {!/\$empty/} split ' ', $lhs;
      $rule[$ruleno] = "$rhs -> $lhs";
      $length[$ruleno] = scalar @lhs;
      $rhs[$ruleno] = $oldrhs = $rhs;
    }
    next;
  }
  if (defined $state) {
    # Skip the lines with the location of state in grammar rule.
    next if /^\s+\$?\w+\s+\-\>\s+/;		# Skip lines with ' lhs -> '
    next if /^\s+\d+\s+\$?\w+:\s+/;		# Skip lines with ' n lhs: '
    next if /^\s+\d+\s+\|\s+/;			# Skip lines with ' n    | '
    # We are reading the list of actions for $state.
    # Actions come in two kinds: lookahead actions, 
    # which you perform after reading a token and which say
    # whether to shift the token or reduce according to a rule, 
    # and reduce jumps, which say what state to go into
    # following a reduce.
    my ($lhs, $rhs) = /\s*(\S+)\s+(.*)$/;
    $lhs =~ s/^\'(\\?.)\'$/$1/ ||
        $lhs =~ s/^\"([^"]+)\"$/$1/;		# CG: handling of "..." tokens
    $lhs =~ s/\$défaut/\$default/;
    if ($rhs =~ /^shift, and go to state (\d+)$/) {
      $act{$state,$lhs} = "shift $1";
    } elsif ($rhs =~ /^reduce using rule (\d+)/) {
      $act{$state,$lhs} = "reduce $1";
    } elsif ($rhs =~ /^go to state (\d+)/) {
      $act{$state,$lhs} = "goto $1";
    } elsif ($rhs =~ /^accept$/) {
      $act{$state,$lhs} = "accept";
    } elsif ($rhs =~ /^\[(shift|reduce)/) {
    } elsif ($rhs =~ /^décalage et aller à l'état (\d+)$/) {
      $act{$state,$lhs} = "shift $1";
    } elsif ($rhs =~ /^réduction par utilisation de la règle (\d+)/) {
      $act{$state,$lhs} = "reduce $1";
    } elsif ($rhs =~ /^aller à l'état (\d+)/) {
      $act{$state,$lhs} = "goto $1";
    } elsif ($rhs =~ /^accepter$/) {
      $act{$state,$lhs} = "accept";
    } elsif ($rhs =~ /^\[(décalage|réduction)/) {
    } else {
      warn "Unrecognized RHS in state $state: $rhs\n$_\n";
    }
  }
}
close Y;

my $progbase = $program[-1];
$progbase =~ s/^.*\///;
print P <<"EOT";
# Parse the language described in $infile.
# Perl source code automatically generated by $progbase.
# Source code copyright 1995 M-J. Dominus (mjd\@pobox.com)

EOT

if (defined($option{a})) {
  $actfile = $option{a};
  open (A , "< $actfile") or
    die "Couldn\'t open semantic action file $actfile for reading: $!";
  my %sem = ();
  print P <<"EOT";
# code included from $actbase
EOT
  local $/ = "";
  while (<A>) {
    chomp;
    my ($rule, $code) = split "\n", $_, 2;
    if ($rule =~ /^#rule\s+(.*\s+->\s+.*)/) {
      my $tag = $1;
      $tag =~ s/\s+\/\*\s+empty\s+\*\// \$empty/;
      $sem{$tag} = $code;
    } else {
      print P $_, "\n\n";
    }
  }
  print P <<"EOT";
# generating or using semantic actions from $actbase
\@sem = (
# rule 0
sub { },
EOT
  for (my $r=1; $r <= $#rule; $r++) {
    my $rule = $rule[$r];
    my $code = "  undef;";
    if (defined $sem{$rule}) {
      $code = $sem{$rule};
      $sem{$rule} = undef;
    }
    print P <<"EOT";
# rule $r
# $rule
sub {
$code
},
EOT
  }
  print P <<"EOT";
);
EOT
  while (my ($rule, $code) = each %sem) {
    print STDERR "leftover semantic action $rule\n" if defined $code;
  }
  close (A);
}

print P <<'EOT';

@act = (
EOT

my $state = -1;
foreach my $k (sort { my ($A, $B) = map {/^(\d+)/} ($a, $b); $A <=> $B } (keys %act)) {
  my ($s, $r) = split(' ', $k);
  my ($a, $d) = (split(' ', $act{$k}), 'undef');
  if ($state != $s) {
    die "non-contiguous state numbering\n" if $state+1 != $s;
    print P "},\n" unless $state < 0;
    ++$state;
    print P "{# state $state\n";
  }
  print P "  '$r'=>['$a','$d'],\n";
}

print P <<'EOT';
},
);

@length = (
EOT

foreach my $l (@length) {
  print P ($l||0), ', ';
}

print P ");\n\n\@rhs = (";
foreach my $rhs (@rhs) {
  $rhs =~ s/\'/\\\'/g;
  print P "\'$rhs\', ";
}
print P ");\n\n";

if ($option{d}) {
  print P "\@rule = (\n";
  for (my $r=0; $r <= $#rule; $r++) {
    my $rule = $rule[$r];
    $rule =~ s/\\/\\\\/g;
    $rule =~ s/\'/\\\'/g;
    print P "  \'$rule\',\n";
  }
  print P ");\n\n";
}

{
  local $/ = "\n";
  while (<DATA>) {
    print P if !/\$yydebug\b/ || $option{d};
  }
}

print P "\n1;\n";

close P;

__DATA__
# py-skel.pl: perl yacc parser skeleton
# Copyright 1995 Mark-Jason Dominus (mjd@pobox.com).
# Hacked 2001 Benoit Dupont de Dinechin (benoit@dinechin.org):
#  - use my variables instead of local wherever possible
#  - replace the soft ref ${rule_$d} code by $sem[$d]
#  - filter out the $yydebug lines unless $option{d}
#  - replace %act by @act to avoid split while parsing
# Hacked 2002 Christophe Guillon
#  - handling of empty rules (reduce and go)

sub yyparse {
  my @states = ();			# State stack
  my @values = ();			# Value stack
  my ($state, $nextplease, $lookahead) = (0, 1);
  for (;;) {
    $lookahead = &yylex if $nextplease;
    print STDERR "\nLookahead token is $lookahead($yylval).\n" if $yydebug;
    print STDERR "State is $state.\n" if $yydebug;
    print STDERR "Top value is '$values[$#values]'\n" if @values && $yydebug;
    my $act = ${$act[$state]}{$lookahead} ||
              ${$act[$state]}{'$default'} ||
              ${$act[$state]}{'$end'} ||
              ${$act[$state]}{'$'};
    unless (defined($act)) {		# Wrong token
      print STDERR "No parsing action found.\n" if $yydebug;
      until (${$act[$state]}{'error'}) {
        $state = pop @states || return 1;
        print STDERR "Popping state stack looking for error transition.\n" if $yydebug;
      }
      print STDERR "Error transition found in state $state.\n" if $yydebug;
    }
    my ($a, $d) = @$act;
    if ($a eq 'shift') {
      print STDERR "Shifting $lookahead($yylval).\n" if $yydebug;
      push(@values, $yylval);
      push(@states, $state);
      print STDERR "Value stack is (@values)\n" if $yydebug;
      print STDERR "State stack is (@states)\n" if $yydebug;
      $state = $d;
      print STDERR "New state is $state.\n" if $yydebug;
      $nextplease = 1;
    } elsif ($a eq 'reduce') {
      print STDERR "Reducing by rule $d ($rule[$d]) (length $length[$d])\n" if $yydebug;
      print STDERR "Val stack is (@values)\n" if $yydebug;
      print STDERR "State stack is (@states)\n" if $yydebug;
      my @args = ();
      @args = splice(@values, -$length[$d]) if $length[$d] > 0; # CG: handling of empty rules.
      print STDERR "...Reducing with args (@args)...\n" if $yydebug;
      push (@values, &{$sem[$d]}(@args));
      print STDERR "Val stack is (@values)\n" if $yydebug;
      push(@states, $state);					# CG: always push state.
      splice(@states, -$length[$d]) if $length[$d] > 0;		# CG: handling of empty rules, we stay in the same state.
      $state = $states[$#states];
      print STDERR "Returning to state $state.\n" if $yydebug;
      print STDERR "State stack is (@states)\n" if $yydebug;
      my $rhs = $rhs[$d];
      print STDERR "rhs is \`$rhs\'\n" if $yydebug;
      my $act = ${$act[$state]}{$rhs};
      my ($a, $d) = @$act;
      if ($a eq 'goto') {
        $state = $d;
        print STDERR "Jumping to state $d.\n" if $yydebug;
      } else {
        print STDERR "Reduction to $rhs in state $state: ";
        print STDERR "action was $a, should have been a goto.\n";
        return 1;
      }
      $nextplease = 0;
    } elsif ($a eq 'goto') {
      print STDERR "EOF parsing action is to go to state $d.\n" if $yydebug;
      $state = $d;
      $nextplease = 0;
    } elsif ($a eq 'accept') {
      print STDERR "Accepting.\n" if $yydebug;
      return 0;
    } else {
      die "Unrecognized parsing action $a.\n";
    }
  }
}
#    END py-skel.pl
