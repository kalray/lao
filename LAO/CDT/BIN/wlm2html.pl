# -*- perl -*-
#
# wlm2html -- Convert Wiki Like Markup to HTML.
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

my $style = <<"STYLE";
  h1,h2,h3,h4,h5,h6 {
    color: #2b5836;
    color: #556f27;
    color: #68892f;
  }
  h1 { font-size: 2.25em; text-align: center; }
  h2 { margin-top: 1.5em; font-size: 1.75em; }
  h3 { margin-top: 2em; font-size: 1.5em; }
  h4 { font-size: 1.25em; }
  h5 { font-size: 1.125em; }
  h6 { font-size: 1em; }
  h2,h3,h4,h5,h6 { margin-left: -5% }
  body {
    background-color: #fff;
    color: #333;
    margin-left: 0%;
    margin-right: 5%;
    font-family: lucida grande, sans-serif;
    font-size: 0.8em;
  }
  blockquote {
    background: #eeeeee;
    padding: 5px 10px 5px 10px;
  }
  pre,code {
    overflow: hidden;
    font-weight: bold;
    white-space: pre; font-family: "Courier New", monospace;
    background: #eee;
  }
  kbd {
    font-family: "Courier New", monospace;
    border: 1px solid #a0a0a0;
  }
  tt { white-space: pre; font-family: "Courier New", monospace; }
  dfn, dt { font-style: italic; font-weight: bold }
  em, var { font-style: italic; }
  div#wrapper{float:right;width:100%;margin-right:-25%}
  div#content{margin-right:25%}
  div#navigate {
    margin-top: 12px;
    float: left;
    clear: left;
    width: 19.9%;
  /*width: 24.9%;*/
    overflow: hidden;
    border: 1px solid #a0a0a0;
    background-color: #f9f9f9;
    padding: 0px;
    font-size: 18px;
  }
  #navigate p {
    text-align: center;
    margin: 12px;
    padding: 0;
  }
  #navigate input {
    color: #2b5836;
    color: #556f27;
    color: #68892f;
    font-size: 1em;
    font-weight: bold;
  }
  #navigate ul {
    list-style-type: none;
    margin: 0px;
    padding: 2px 0px 12px 2px;
  }
  #navigate li {
    padding: 8px 8px 0px 8px;
    font-size: 12px;
    line-height: 12px;
    font-weight: bold;
  }
  div#footer{clear:both;width:100%}
STYLE

print <<"HTML";
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
  <style type="text/css">
$style
  </style>
  <script type="text/javascript">
  function toggleShow(objId)
  {
    var obj = document.getElementById(objId);
    if(obj.style.display == 'none') {
      obj.style.display = '';
    } else {
      obj.style.display = 'none';
    }
  }
  </script>
HTML

my $title;
sub title {
  ($title) = @_;
print <<"HTML";
  <title>$title</title>
HTML
}

##########################################################################################

my %header = (
 '!!!!'=> "h1",
 '!!!'=> "h2",
 '!!'=> "h3",
 '----'=> "h4",
 '---'=> "h5",
 '--'=> "h6",
);

sub header {
  $header{$_[0]} || 'ERROR'
}

sub ID {
  my $ID = $_[0];
  $ID =~ s/\W//g;
  return $ID;
}

my @number = (
 undef,	# h0
 undef,	# h1
 0,	# h2
 0,	# h3
 0,	# h4
 undef,	# h5
 undef,	# h6
);

sub number {
  my $number;
  (my $index = $_[0]) =~ s/\D//g;
  if (defined $number[$index]) {
    ++$number[$index];
    for my $i (1 .. 6) {
      if (defined $number[$i]) {
        if ($i <= $index) {
          $number .= "$number[$i].";
        } else {
          $number[$i] = 0;
        }
      }
    }
  }
  return $number;
}

my %indent = (
 h2=> '',
 h3=> '&nbsp;&nbsp;',
 h4=> '&nbsp;&nbsp;&nbsp;&nbsp;',
);

sub indent {
  return $indent{$_[0]};
}

sub kbd { "<\000kbd>\000$_[0]<\000\/\000kbd>\000" }
sub strong { "<\000strong>\000$_[0]<\000\/\000strong>\000" }
sub code { "<\000code>\000$_[0]<\000\/\000code>\000" }
sub var { "<\000var>\000$_[0]<\000\/\000var>\000" }
sub dfn { "<\000dfn>\000$_[0]<\000\/\000dfn>\000" }
sub em { "<\000em>\000$_[0]<\000\/\000em>\000" }
sub sup { "<\000sup>\000$_[0]<\000\/\000sup>\000" }
sub sub { "<\000sub>\000$_[0]<\000\/\000sub>\000" }
sub img {
  my @text = split ' ', $_[0];
  my $src = shift @text;
  $src =~ s/(\s|[\x21-\x2F]|[\x3A-\x40]|[\x5B-\x60]|[\x7B-\x7E])/$1\000/g;
  my $alt = @text? (join ' ', @text): '';
  "<\000img src=\"\000$src\"\000 alt=\"$alt\"\/>\000"
}
sub math {
  # TODO
  'LaTeX $mathmode$'
}
sub ahref {
  my @text = split ' ', $_[0];
  my $href = pop @text;
  $href =~ s/(\s|[\x21-\x2F]|[\x3A-\x40]|[\x5B-\x60]|[\x7B-\x7E])/$1\000/g;
  my $title = @text? (join ' ', @text): $href;
  "<\000a href=\000\"\000$href\"\000>\000$title<\000\/\000a>\000"
}
sub aname {
  my @text = split ' ', $_[0];
  my $name = pop @text;
  $name =~ s/(\s|[\x21-\x2F]|[\x3A-\x40]|[\x5B-\x60]|[\x7B-\x7E])/$1\000/g;
  my $title = @text? (join ' ', @text): $name;
  "<\000a name=\000\"\000$name\"\000>\000$title<\000\/\000a>\000"
}

my %escape = (
 '&'=> '&amp;',
 '<'=> '&lt;',
 '>'=> '&gt;',
);

my @chunks;
$/ =  "";
while(<>) {
  chomp;
  # Chunks are started by lines with a tab somewhere.
  push @chunks, split /^(?=[^\t].*\t)/m;
}

my @toclines;
my @indexed;
my @buffer;
foreach (@chunks) {
#print STDERR "CHUNK($_)\n";
  my $type = 'p';
  if (/^\t/) {
    # Verbatim
    s/^\t//gm;
    s/([\&\<\>])/$escape{$1}/g;
    # From Perl Cookbook "Expanding and Compressing Tabs"
    my @lines =  split /\n/, $_;
    map {1 while s/\t+/' ' x (length($&) * 8 - length($`) % 8)/e} @lines;
    $_ = join "\n", @lines;
    $type = 'pre';
  } else {
    # Protect backslahed non-word characters, including '_' and ' '.
    s/\\(\s|[\x21-\x2F]|[\x3A-\x40]|[\x5B-\x60]|[\x7B-\x7E])/$1\000/g;
    # Escape XML markup if escaped.
    s/([\&\<\>])(?=[\000])/$escape{$1}/g;
    # Inline WLM, starts with special char with no \S after and ends with no \S before.
    if (s/\<(?![\s\000])(([^\>]|\>\000)+?)(?<=\S)\>(?!\000)/&ahref($1)/ge) { }
    if (s/\|(?![\s\000])(([^\|]|\|\000)+?)(?<=\S)\|(?!\000)/&kbd($1)/ge) { }
    if (s/\!(?![\s\000])(([^\!]|\!\000)+?)(?<=\S)\!(?!\000)/&strong($1)/ge) { }
    if (s/\=(?![\s\000])(([^\=]|\=\000)+?)(?<=\S)\=(?!\000)/&code($1)/ge) { }
    if (s/\@(?![\s\000])(([^\@]|\@\000)+?)(?<=\S)\@(?!\000)/&var($1)/ge) { }
    if (s/\:(?![\s\000])(([^\:]|\:\000)+?)(?<=\S)\:(?!\000)/&dfn($1)/ge) { }
    if (s/\*(?![\s\000])(([^\*]|\*\000)+?)(?<=\S)\*(?!\000)/&em($1)/ge) { }
    if (s/\^(?![\s\000])(([^\^]|\^\000)+?)(?<=\S)\^(?!\000)/&sup($1)/ge) { }
    if (s/\~(?![\s\000])(([^\~]|\~\000)+?)(?<=\S)\~(?!\000)/&sub($1)/ge) { }
    if (s/\%(?![\s\000])(([^\%]|\%\000)+?)(?<=\S)\%(?!\000)/&img($1)/ge) { }
    if (s/\$(?![\s\000])(([^\$]|\$\000)+?)(?<=\S)\$(?!\000)/&math($1)/ge) { }
    if (s/\#(?![\s\000])(([^\#]|\#\000)+?)(?<=\S)\#(?!\000)/&aname($1)/ge) { }
    #if (s/\[(?![\s\000])(([^\]]|\]\000)+?)(?<=\S)\](?!\000)/&bibref($1)/ge) { }
    # Remove non-word character escapes.
    s/\s?\000//g;
    # Process block markup.
    if (s/^\s*((?:[^\s\!]|(?:\![^\s\!]))*)\s*(\!\!+)\t(.*)/$3/s) {
      # Headers h1 to h3.
      my $ID = &ID($1) if defined $1;
      $type = &header($2);
      &title($3) unless $title;
      $_ = "<a name='$ID'>$_<\/a>" if $ID;
      my $number = &number($type);
      if ($number) {
        my $indent = &indent($type);
        (my $id = $number) =~ s/\W/_/g;
        push @toclines, "<li>$indent<a href='#$id'>$number $_</a></li>\n";
        $_ = "<a name='$id'>$number<\/a> $_";
      }
    } elsif (s/^\s*((?:[^\s\-]|(?:\-[^\s\-]))*)\s*(\-\-+)\t(.*)/$3/s) {
      # Headers h4 to h6.
      my $ID = &ID($1) if defined $1;
      $type = &header($2);
      if ($ID) {
        my $ID = &ID($1);
        $_ = "<a name='$ID'>$1&mdash; <\/a> $_";
        push @indexed, "<li><a href='#$ID'>$1</a></li>\n"
      }
    } elsif (s/^\s*([^\:]+?)\s*\:\t(.*)/<dt>$1<\/dt> <dd>$2<\/dd>/s) {
      # Definition list.
      $type = 'dl';
    } elsif (s/^(\s*[\+\#])\t(.*)/<li>$2<\/li>/s) {
      # Numbered list.
      $type = "ol". (' ' x length($1));
    } elsif (s/^(\s*[\-\*])\t(.*)/<li>$2<\/li>/s) {
      # Unordered list.
      $type = "ul". (' ' x length($1));
    } elsif (s/^(\s*\>)\t/<blockquote>/s) {
      # Open block quote.
    } elsif (s/^(\s*\<)\t/<\/blockquote>/s) {
      # Close block quote.
    }
  }
  push @buffer, [ $_, $type ] if defined $type;
}

sub level {
  my ($type) = @_;
  #$type =~ s/^\S+//;
  return length($type);
}

print <<"HTML";
</head>
<body>
<div id="wrapper">
<div id="content">
HTML

my @open;
map {
  my $text = $$_[0];
  my $type = $$_[1];
  my $class = $$_[2];
#print STDERR "TYPE($type) TEXT($text)\n";
  if ($type =~ /^(ul|ol|dl|blockquote)/) {
    while (@open && $open[-1] ne $type &&
           &level($open[-1]) >= &level($type)) {
      my $open = pop @open;
      print "</$open>\n";
    }
    unless (@open && $open[-1] eq $type) {
      print "<$type>\n";
      push @open, $type;
    }
  } else {
    while (@open) {
      my $open = pop @open;
      print "</$open>\n";
    }
    if (length($text)) {
      $text =~ s/^(\s*)/<$type>$1/;
      $text =~ s/(\s*)$/$1<\/$type>/;
    } else {
      $text = "<$type />";
    }
  }
  print $text, "\n";
} @buffer;
while (@open) {
  my $open = pop @open;
  print "</$open>\n\n";
}

print <<"HTML";
</body>
</div>
</div>
HTML

if (@toclines) {
  print <<"HTML";
<div id="navigate">
<p>
<input type="button" value="Contents" onclick="toggleShow('Contents')" />
</p>
<ul id="Contents">
HTML
  print @toclines;
  print <<"HTML";
</ul>
</div>
HTML
}

if (@indexed) {
  print <<"HTML";
<div id="navigate">
<p>
<input type="button" value="Index" onclick="toggleShow('Index')" />
</p>
<ul id="Index">
HTML
  print sort { $a cmp $b } @indexed;
  print <<"HTML";
</ul>
</div>
HTML
}

print <<"HTML";
</html>

HTML

