#! /usr/bin/perl
use strict;

my %frame_map;

open(my $IN, "<", $ARGV[0]);
open(my $OUT, ">", "$ARGV[0]_final");

while(<$IN>)
{
	/(\d+.?\d*) (\d+)/;
  if(!(exists $frame_map{$2}))
  {
    $frame_map{$2}=1;
    print $OUT "$_";
  }
}

close($IN);
close($OUT);

`mv $ARGV[0]_final $ARGV[0]`;
