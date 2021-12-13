#!/usr/bin/perl -w

use strict;

my ($basename) = split /\./, (split '/', $ARGV[0])[-1];
die "Cannot find basename for $ARGV[0]" unless defined $basename && $basename =~ /^\w+$/;
my $ruler = '/' x 100;

$/ = '';
while (<>) {
  chomp;
  # Normalize rulers.
  s/^\/\/\/[\/]*$/$ruler/mo;
  # Normalize leading TABs.
  1 while s/^\t+/' ' x (length($&)*8)/me;
  # Process XCC paragraphs
  if (s/^((\/\/)|(\/\*))\@XCC(.*)/$1/i) {
    (my $xcctag = $4) =~ s/\W/_/g;
    $xcctag = "\$XCC". $xcctag;
    if (s/^\/\//#ifdef $xcctag/) { }
    elsif (s/^\/\*/#ifdef $xcctag\n\/\*/) { }
    print;
    print "\n#endif//$xcctag";
  } else { print }
  print "\n\n";
}

