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
	$maxrate = $2 / $times[0];
}
#print "maxrate: $maxrate\n";
#print "$times[0]\n";
#print @frames; print "\n";
#print @times; print "\n";
my $wait = 1;
my @waits;
my $rate = 1;
my @rates;
#for (my $i = $times[0]; $i < 5; $i+=.1){
for (my $i = .1; $i < 2; $i+=.1){
	#print $i;
	$wait = $i;
	for (my $j = 1; $j <= $maxrate; $j+=10){
		#print " $j\n";
		$rate = $j;
		#print "$rate(x - $wait)\n";
		my $test = 0;
		for (my $k = 0;$k < $count; $k++){
			#if ($rate*($times[$k] - $wait) > $frames[$k]){
			if ($rate*($times[$k] - $wait) > $frames[$k]){
				#print "found one\n";
				#print "$rate(x - $wait)\n";
				#break;
				$test = 1;
				last;
			}
		}
		if ($test == 1){
			last;
		}
		#print "$rate*(x - $wait),";
		push(@rates,$rate);
		push(@waits,$wait);
	}
}
#print "\n";
# now pick best rate
#print "\n"; print @rates;
#print "\n"; print @waits;
print "Solutions: "; print scalar @rates; print "\n";
my $alpha = 10;
my $betta = .001;
my $solutions = scalar @rates;
my $best = 0;
my $which = 0;
for (my $i = 0; $i < $solutions; $i++){
	my $tempwait = $waits[$i];
	my $temprate = $rates[$i];
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
print "Best Rate: $rates[$which]\n";
print "Best Wait: $waits[$which]\n";
print "$rates[$which]*(x - $waits[$which])";
print "\n";
