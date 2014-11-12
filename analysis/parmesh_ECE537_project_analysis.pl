#! /usr/bin/perl
use strict;
my $count = 0;
my @frames;
my @times;
# Read all lines from the input file
# Expect input to be in <time> <frame> pairs
while(<>){
	/(\d+.?\d*) (\d+)/;
	push(@times,$1);
	push(@frames,$2);
	$count++;
}
use List::Util qw( min max);
# Find maximum time, because for loop should stop before there
my $max = max @times;
use POSIX qw (ceil);
$max = ceil($max);
print "Data Points: "; print scalar @times; print "\n";
my $wait = 1;
my @waits;
my $rate = 100;
for (my $i = 0; $i < $max; $i+=1){
	$wait = $i;
	my $test = 0;
	# look at all of the data points and make sure the current line is to the 
	# right of all of them
	for (my $k = 0;$k < $count; $k++){
		# check to see if current data point is to the right of the line
		if ($rate*($times[$k] - $wait) > $frames[$k]){
			$test = 1;
			last; # Already found one bad point, so exit inner loop
		}
	}
	if ($test == 0){
		push(@waits,$wait);
	}
}
print "Solutions: "; print scalar @waits; print "\n";
# This next bit finds the best solution according to the "User Satisfaction" 
# equation given in the Project Specification
# Not being used right now
=pod
my $alpha = 10;
my $betta = .001;
my $solutions = scalar @waits;
my $best = 0;
my $which = 0;
for (my $i = 0; $i < $solutions; $i++){
	my $tempwait = $waits[$i];
	#my $temprate = $rates[$i];
	my $temprate = $rate;
	#my $cost = $temprate / $tempwait;
	my $cost = (exp (-$alpha*$tempwait))*(1 - exp (-$betta*$temprate));
	#my $first = exp (-$alpha*$tempwait);
	#my $last = 1-exp(-$betta*$temprate);
	#print "$tempwait,$temprate,$first,$last,$cost\n";
	if ($cost > $best){
		$best = $cost;
		$which = $i;
	}
}

print "\nUser Satisfaction: $best\n";
=cut

# All we want for right now is the minimum wait time
my $min = min @waits;
print "Rate: $rate\n";
#print "Best Wait time: $waits[$which]\n";
#print "$rate*(x - $waits[$which])";
print "Lowest Wait time: $min\n";
print "$rate*(x - $min)";
print "\n";
