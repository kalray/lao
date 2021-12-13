# -*- perl -*-
#
# xccgen -- Generate multiple 'source' files from a single file.
#
# Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com)
#
# Copyright 2002 - 2006 STMicroelectronics.
# Copyright 1995 - 1998 Commissariat a l'Energie Atomique.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

use Declare;

use Getopt::Long qw(:config no_ignore_case);
use File::Basename;
my $always_generate;    # always regenerate the output files even if same contents.
my $doc_suffix;         # emit block comment text in documentation file with given suffix.
my $filtered;           # emit filtered non-comment text in documentation file.
my $line_directives;    # generates #line directives except in .h and .H files.
my $line_directives_all;# generates #line directives in all derived files.
my $restrict_flag;      # allows to use "restrict" in generated code.

#TODO: tidy up options, only -a -l -L & -r are actually used...

GetOptions (
  "always"    => \$always_generate,
  "doc=s"     => \$doc_suffix,
  "filtered"  => \$filtered,
  "line"      => \$line_directives,
  "Line"      => \$line_directives_all,
  "restrict"  => \$restrict_flag
);

my $restrict = $restrict_flag? "restrict": "";
my $restrict_ = $restrict_flag? "restrict_": "";

my ($CTOR, $COPY, $DTOR, $SIZE) = ("_Ctor", "_Copy", "_Dtor", "_Size");

my %outfile;
my $stem = fileparse($ARGV[0], ".c");

#
# Parse @lines and build @paragraphs.
#
my @paragraphs;
my $paragraph = undef;

while (<>) {
  # start new XCC paragraph
  if(/^\#if(def)?\s+(\$XCC(_*)_([ch]))\n$/) {
    # Compute the $suffix.
    my $suffix = $3 . '.' . $4 ;
    my $outname = $stem . $suffix;
    $outfile{$outname} = { STEM=> $stem, SUFFIX=> $suffix, };
    # Create a new paragraph.
    my $source = "\n#line " . ($.+1) . " \"$ARGV\"\n"; # Current line & filename
    $source = "\n" unless
    defined $line_directives_all ||
    defined $line_directives && $suffix !~ /\.[hH]$/;
    push @paragraphs, {
      STEM=> $stem,
      SUFFIX=> $suffix,
      OUTNAME=> $outname,
      SOURCE=> $source,
      LINES=> [ ]
    };
    $paragraph = $paragraphs[-1];
  }
  # end an XCC paragraph
  elsif(/^\#endif\s*\/\/\s*(\$\w+)(_\w)$/) {
    $paragraph = undef;
  }
  else {
    if(not defined $paragraph) {
      next if /^$/; # skip empty lines
      push @paragraphs, {
        LINES=> [ $_ ]
      };
      $paragraph = $paragraphs[-1];
    } 
    else {
      # Grow previous paragraph.
      push @{$$paragraph{LINES}}, $_; 
    }
  }
}

#die 42;

#
# Header and trailer emit functions.
#

my %header;
sub header {
  my ($stem, $suffix, $handle) = @_;
  return unless $suffix =~ /\.[ch]$/;
  my $included = &included($stem, $suffix);
  print $handle "#ifndef $included\n#define $included\n";
  my $header = $header{$suffix};
  print $handle @{$header} if defined $header;
}

my %trailer;
sub trailer {
  my ($stem, $suffix, $handle) = @_;
  return unless $suffix =~ /\.[ch]$/;
  my $trailer = $trailer{$suffix};
  print $handle @{$trailer} if defined $trailer;
  my $included = &included($stem, $suffix);
  print $handle "\n\n#endif//$included\n";
}

sub included {
  my ($stem, $suffix) = @_;
  $suffix =~ s/\W/_/g;
  "_INCLUDED_$stem${suffix}"
}

use File::Copy;
use File::Compare;

my $TMPDIR;

if (exists $ENV{"TMPDIR"}) {
    $TMPDIR = $ENV{"TMPDIR"}
} else {
    $TMPDIR = "/tmp"
}

#
# Main processing
#

# Open all the output files. Add header text if required.
foreach my $outname (keys %outfile) {
  my $handle = undef;
  my $stem = $outfile{$outname}{STEM};
  my $suffix = $outfile{$outname}{SUFFIX};
  my $tmpfile = "$TMPDIR/$$.$outname";
  open ($handle, ">".$tmpfile) || die "can't open $tmpfile: $!\n";
  $outfile{$outname}{HANDLE} = $handle;
  &header($stem, $suffix, $handle);
}

# Process the @paragraphs.
foreach my $paragraph (@paragraphs) {
  # Dispatch for further paragraph processing.
  &paragraph($paragraph);
}

# Close all the output files. Add trailer text if required.
foreach my $outname (keys %outfile) {
  my $tmpfile = "$TMPDIR/$$.$outname";
  my $stem = $outfile{$outname}{STEM};
  my $suffix = $outfile{$outname}{SUFFIX};
  my $handle = $outfile{$outname}{HANDLE};
  &trailer($stem, $suffix, $handle);
  close ($handle) || die "can't close $tmpfile: $!\n";
  if (!-e $outname) {
    move($tmpfile, $outname) || die "can't create $outname: $!\n";
  } elsif ($always_generate || compare($tmpfile, $outname)) {
    chmod 0666, $outname;
    move($tmpfile, $outname) || die "can't update $outname: $!\n";
  } else {
    unlink($tmpfile);
  }
  chmod 0444, $outname;
}

#
# Paragraph processing.
#
sub paragraph {
  my ($paragraph) = @_;
  my $stem = $$paragraph{STEM};
  return unless defined $stem;
  my $suffix = $$paragraph{SUFFIX};
  my $outname = $$paragraph{OUTNAME};
  my $source = $$paragraph{SOURCE};
  my $lines = $$paragraph{LINES};
  my $handle = $outfile{$outname}{HANDLE};
  print $handle $source, @{$lines};
  # Build a @buffer with the C block comments removed.
  my (@buffer, @comment, $comment, $struct, $typename, $char);
  foreach my $line (@{$lines}) {
    my $buffer = $line;
    if ($comment) {
      if ($buffer =~ s!(.*?)\*/!!) {
        push @comment, $1;
        $comment = 0;
      } else {
        push @comment, $buffer;
      }
      $buffer = "\n";
    } else {
      $buffer =~ s!/\*.*?\*/!!;
      if ($buffer =~ s!/\*(.*)!!) {
        push @comment, $1;
        $comment = 1;
      }
    }
    push @buffer, $buffer;
    # $struct is the first $buffer line with "struct".
    if (!$struct &&
        $buffer =~ /^\s*struct\s+(\w+)_\s*([;{])/) {
      ($struct, $typename, $char) = ($buffer, $1, $2);
    }
  }
  if (defined $struct) {
    # Processing "struct Foo_ ..."
    &structure($paragraph, $typename, $char, \@buffer);
  } else {
    # No special processing.
  }
}

#
# Structure processing.
#
my %args;
my %typedef;
sub structure {
  my ($paragraph, $typename, $char, $buffer) = @_;
  my $stem = $$paragraph{STEM};
  my $suffix = $$paragraph{SUFFIX};
  my $outname = $$paragraph{OUTNAME};
  my $source = $$paragraph{SOURCE};
  my $lines = $$paragraph{LINES};
  my $handle = $outfile{$outname}{HANDLE};
  my @tags = grep { /\/\/\W*\@[a-z]+\b/i } @{$lines};
  map { chomp } @tags;
  my @args = grep { s/^.*\/\/\@args\b\s*//i } @tags;
  my $args = join (" ", @args);
  if (@args) {
    $args{$typename} = $args;
  } else {
    $args = $args{$typename};
  }
  # Emit typedef for the structure and its pointer.
  unless (exists $typedef{$typename}) {
    print $handle "typedef struct ${typename}_ ${typename}_, *$typename;\n";
    print $handle "typedef const struct ${typename}_ *const_$typename;\n";
    print $handle "typedef struct ${typename}_ *$restrict restrict_$typename;\n";
    $typedef{$typename} = undef;
  }
  if (defined $args{$typename}) {
    # Emit the automatic function declarations.
    my %flags = ( TYPEDEF=>1, DECLARE=>1, QUALIFIER=>"extern ", STEM=>$stem );
    print $handle
          grep {defined}
          "\n", &CTOR($typename, [], $args, \%flags),
          "\n", &COPY($typename, [], \%flags),
          "\n", &DTOR($typename, [], \%flags),
          "\n", &SIZE($typename, [], $args, \%flags),
  }
  # Grab the user-defined accessor and mutator macros like '//@access FOO foo()'.
  my %access = map { split ' ', $_, 2 } grep { s/^.*\/\/\@access\b\s?//i } @tags;
  my %mutate = map { split ' ', $_, 2 } grep { s/^.*\/\/\@mutate\b\s?//i } @tags;
  if ($char eq ';') {
    # Processing 'struct Foo_;'.
    # Emit the user-defined accessor and mutator macros.
    my ($structname, @members) = ("${typename}_");
    push @members, map { "\n#define ${structname}$_(this) ($access{$_})" } keys %access;
    push @members, map { "\n#define ${structname}_$_(this) ($mutate{$_})" } keys %mutate;
    print $handle @members;
  } elsif ($char eq '{') {
    # Processing 'struct Foo_ { ... };'.
    # Parse the $buffer and emit the accessor and mutator macros.
    print $handle &members($buffer, \%access, \%mutate);
    if (0 && defined $args{$typename}) {
      # Collect the code contributions to @decl @ctor @copy @dtor @size.
      my @decl = @{$buffer}; map { s/^.*\/\/\@decl\b//i or m/^\#/ or $_ = "\n" } @decl;
      my @ctor = @{$buffer}; map { s/^.*\/\/\@ctor\b//i or m/^\#/ or $_ = "\n" } @ctor;
      my @copy = @{$buffer}; map { s/^.*\/\/\@copy\b//i or m/^\#/ or $_ = "\n" } @copy;
      my @dtor = @{$buffer}; map { s/^.*\/\/\@dtor\b//i or m/^\#/ or $_ = "\n" } @dtor;
      my @size = @{$buffer}; map { s/^.*\/\/\@size\b//i or m/^\#/ or $_ = "\n" } @size;
      # Emit the automatic function definitions.
      my %flags = ( SOURCE=>$source, TYPEDEF=>1, QUALIFIER=>"", STEM=>$stem );
      print $handle
           grep {defined}
           "\n", &DECL($typename, \@decl, \%flags),
           "\n", &CTOR($typename, \@ctor, $args, \%flags),
           "\n", &COPY($typename, \@copy, \%flags),
           "\n", &DTOR($typename, \@dtor, \%flags),
           "\n", &SIZE($typename, \@size, $args, \%flags),
    }
  } else {
    # No further structure processing.
  }
}

#
# Parsing of C structures to extract members.
#

sub members {
  my @members = ();
  my ($buffer, $access, $mutate) = @_;
  &Declare::yyin(@{$buffer});
  &Declare::yylval(16);
  &Declare::yyparse() && print STDERR "syntax error: ", &Declare::yylval(), "\n";
  my @struct = &Declare::struct();
  my $structname = shift @struct;
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
      if ($$access{$member}) {
        push @members, "\n#define ${structname}$member(this) ($$access{$member})";
        delete $$access{$member};
      } else {
        push @members, "\n#define ${structname}$member(this) ((this)->$member)";
      }
      if ($$mutate{$member}) {
        push @members, "\n#define ${structname}_$member(this) ($$mutate{$member})";
        delete $$mutate{$member};
      } else {
        push @members, "\n#define ${structname}_$member(this) (&(this)->$member)";
      }
    }
  }
  push @members, map { "\n#define ${structname}$_(this) ($$access{$_})" } keys %$access;
  push @members, map { "\n#define ${structname}_$_(this) ($$mutate{$_})" } keys %$mutate;
  @members, "\n\n";
}

sub DECL {
  my ($typename, $decl, $flags) = @_;
  my $source = $$flags{SOURCE};
  my $provided = grep {!/^(#.*|\s*)$/} @$decl;
  $decl = $line_directives? join ("", @$decl): join ("", (grep {/\S/} @$decl));
  $decl = $source. $decl if $provided;
  return $decl;
}

sub CTOR {
  my ($typename, $ctor, $args, $flags) = @_;
  my $qualifier = $$flags{QUALIFIER};
  my $thistype = $$flags{TYPEDEF}?
                 "$restrict_${typename}":
                 "struct ${typename}_*$restrict";
  my $returntype = $$flags{TYPEDEF}?
                   ${typename}:
                   "struct ${typename}_*";
  my $included = &included($$flags{STEM}, ".c");
  my $provided = grep {!/^(#.*|\s*)$/} @$ctor;
  my $cargs = "$thistype this";
  $cargs .= ", $args" if $args;
  return <<"EOC" if $$flags{DECLARE};
${qualifier}$returntype
${typename}${CTOR}($cargs);
EOC
  return undef unless grep {/\S/ && !/^#/} @$ctor;
  my $source = $$flags{SOURCE};
  $ctor = $line_directives? join ("", @$ctor): join ("", (grep {/\S/} @$ctor));
  return <<"EOC";
#ifdef $included
${qualifier}$returntype
${typename}${CTOR}($cargs) {$source$ctor
\treturn this;
}
#endif//$included
EOC
}

sub COPY {
  my ($typename, $copy, $flags) = @_;
  my $qualifier = $$flags{QUALIFIER};
  my $thistype = $$flags{TYPEDEF}?
                 "$restrict_${typename}":
                 "struct ${typename}_*$restrict";
  my $thattype = $$flags{TYPEDEF}?
                 "const_${typename}":
                 "const struct ${typename}_*$restrict";
  my $returntype = $$flags{TYPEDEF}?
                   ${typename}:
                   "struct ${typename}_*";
  my $included = &included($$flags{STEM}, ".c");
  my $provided = grep {!/^(#.*|\s*)$/} @$copy;
  return <<"EOC" if $$flags{DECLARE};
${qualifier}$returntype
${typename}${COPY}($thistype this, $thattype that);
EOC
  return undef unless grep {/\S/ && !/^#/} @$copy;
  my $source = $$flags{SOURCE};
  $copy = $line_directives? join ("", @$copy): join ("", (grep {/\S/} @$copy));
  return <<"EOC";
#ifdef $included
${qualifier}$returntype
${typename}${COPY}($thistype this, $thattype that) {$source$copy
\treturn this;
}
#endif//$included
EOC
}

sub DTOR {
  my ($typename, $dtor, $flags) = @_;
  my $qualifier = $$flags{QUALIFIER};
  my $thistype = $$flags{TYPEDEF}?
                 "$restrict_${typename}":
                 "struct ${typename}_*$restrict";
  my $included = &included($$flags{STEM}, ".c");
  my $provided = grep {!/^(#.*|\s*)$/} @$dtor;
  return <<"EOC" if $$flags{DECLARE};
extern void
${typename}${DTOR}($thistype this);
EOC
  return undef unless grep {/\S/ && !/^#/} @$dtor;
  my $source = $$flags{SOURCE};
  $dtor = $line_directives? join ("", @$dtor): join ("", (grep {/\S/} @$dtor));
  return <<"EOC";
#ifdef $included
void
${typename}${DTOR}($thistype this) {$source$dtor
}
#endif//$included
EOC
}

sub SIZE {
  my ($typename, $size, $args, $flags) = @_;
  my $qualifier = $$flags{QUALIFIER};
  my $thistype = $$flags{TYPEDEF}?
                 "$restrict_${typename}":
                 "struct ${typename}_*$restrict";
  my $included = &included($$flags{STEM}, ".c");
  my $provided = grep {!/^(#.*|\s*)$/} @$size;
  my $sargs = $args || "void";
  return <<"EOC" if $$flags{DECLARE};
${qualifier}size_t
${typename}${SIZE}($sargs);
EOC
  return undef unless grep {/\S/ && !/^#/} @$size;
  my $source = $$flags{SOURCE};
  $size = $line_directives? join ("", @$size): join ("", (grep {/\S/} @$size));
  return <<"EOC" if $provided;
#ifdef $included
${qualifier}size_t
${typename}${SIZE}($sargs) {
\treturn$source$size;
}
#endif//$included
EOC
  undef
}

