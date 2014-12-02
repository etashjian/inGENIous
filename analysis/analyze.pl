#! /usr/bin/perl
use strict;
my $count = 0;
my @frames;
my @times;
my $maxrate = 20;
while(<>){
	#print $_;
	/(\d+.?\d*) (\d+)/;
	#print $1;
	push(@frames,$2);
	#print $2;
	push(@times,$1);
	$count++;
	#$maxrate = $2 / $times[0];
}

use List::Util qw( min max);
# Find maximum frame
my $maxFrame = max @frames;
print "max Frame #: $maxFrame\n";
my $maxTime = max @times;
print "max Time: $maxTime\n";
for (my $j = 10; $j <= 100; $j+=10){
	my $startTime = max ($maxTime - ($maxFrame/$j),0);
	print "$j * (x-$startTime),";
}
print"\n";

