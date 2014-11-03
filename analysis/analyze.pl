#! /usr/bin/perl
$count = 0;
while(<>){
	print $_;
	/(\d+.\d+) (\d+)/;
	#print $1;
	push(@frames,$2);
	#print $2;
	push(@times,$1);
	$count++;
}
#print @frames; print "\n";
#print @times; print "\n";
$wait = 1;
$rate = 1;
for (my $i = $times[0]; $i < 5; $i+=.5){
	#print $i;
	$wait = $i;
	for (my $j = 100; $j <= 10000; $j+=100){
		#print " $j\n";
		$rate = $j;
		#print "$rate(x - $wait)\n";
		$test = 0;
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
print "\n";
# now pick best rate
#print "\n"; print @rates;
#print "\n"; print @waits;
#print "Solutions: "; print scalar @rates; print "\n";
$alpha = 1;
$betta = 1;
$solutions = scalar @rates;
my $best = 0;
my $which = 0;
for (my $i = 0; $i < $solutions; $i++){
	my $tempwait = $waits[$i];
	my $temprate = $rates[$i];
	#my $cost = $temprate / $tempwait;
	my $cost = (exp (-$alpha*$tempwait))*(1 - exp (-$betta*$temprate));
	#print "$cost,";
	if ($cost > $best){
		$best = $cost;
		$which = $i;
	}
}
print "\nCost: $best\n";
print "Best Rate: $rates[$which]\n";
print "Best Wait: $waits[$which]\n";
print "$rates[$which]*(x - $waits[$which])";
print "\n";
