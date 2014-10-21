#! /usr/bin/perl
$count = 0;
while(<>){
	print $_;
	/(\d+) (\d+)/;
	#print $1;
	push(@frames,$1);
	#print $2;
	push(@times,$2);
	$count++;
}
print @frames; print "\n";
print @times; print "\n";
$wait = 0;
$rate = 1;
for (my $i = 0; $i < 10; $i++){
	#print $i;
	$wait = $i;
	for (my $j = .5; $j <= 3; $j+=.5){
		#print " $j\n";
		$rate = $j;
		#print "$rate(x - $wait)\n";
		$test = 0;
		for (my $k = 0;$k < $count; $k++){
			if ($rate*($times[$k] - $wait) > $frames[$k]){
				#print "found one\n";
				#print "$rate(x - $wait)\n";
				#break;
				$test = 1;
				#last;
			}
		}
		if ($test == 1){
			last;
		}
		print "$rate*(x - $wait),";
	}
}
print "\n";