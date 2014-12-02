#! /usr/bin/perl
use strict;
my $count = 0;
my @frames;
my @times;
my $minTime = 0;
while(<>){
	#print $_;
	/(\d+.?\d*) (\d+)/;
	#print $1;
	push(@frames,$2);
	#print $2;
	push(@times,$1);
	$count++;
	if ($2 == 0){
		$minTime = $1;
	}
	#$maxrate = $2 / $times[0];
}
use List::Util qw( min max);
# Find maximum frame
my $maxFrame = max @frames;
print "max Frame #: $maxFrame\n";
my $maxTime = max @times;
print "max Time: $maxTime\n";
print "min Time: $minTime\n";
use POSIX qw (ceil);
$minTime = ceil($minTime);
for (my $j = 10; $j <= 100; $j+=10){
	my $rate = $j;
	my $startTime = max ($maxTime - (($maxFrame+1)/$rate),$minTime);
	# make sure line is valid
	my $test = 1;
	while($test){
		$test = 0;
		for (my $k = 0; $k < $count; $k ++){
			# check to see if proposed line is right of all points
			if($rate*($times[$k] - $startTime) > $frames[$k]){
				# found one point that is after line
				$test = 1;
				last;
			}
		}
		# check to see if all points were to the left of proposed line
		if($test == 0){
			last;
		}
		else{
			$startTime += 1;
		}
	}
	
	# find bmax
	my @buffSizes;
	for (my $k = 0; $k < $count; $k ++){
		my $b = $k+1 - $rate * ($times[$k] - $startTime);
		push (@buffSizes,$b);
	}
	#print @buffSizes;
	my $bmax = max(@buffSizes);
	print "bmax for $rate * (x-$startTime) is $bmax\n";
	#print "$rate * (x-$startTime),";
}
print"\n";

