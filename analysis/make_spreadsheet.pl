#! /usr/bin/perl

open (sim_ticks_file, 'sorted/sim_ticks.sorted') or die "Failed to open sim_ticks file\n";
open (sim_seconds_file, 'sorted/sim_seconds.sorted') or die "Failed to open sim_seconds file\n";
open (host_seconds_file, 'sorted/host_seconds.sorted') or die "Failed to open host_seconds file\n";
open (l3_miss_rate_file, 'sorted/l3_miss_rate.sorted') or die "Failed to open l3_miss_rate file\n";
open (l2_miss_rate_file, 'sorted/l2_miss_rate.sorted') or die "Failed to open l2_miss_rate file\n";
open (area_file, 'sorted/Area.sorted') or die "Failed to open Area file\n";
open (peak_power_file, 'sorted/Peak_Power.sorted') or die "Failed to open Peak Power file\n";
open (runtime_dynamic_file, 'sorted/Runtime_Dynamic.sorted') or die "Failed to open Runtime Dynamic file\n";
open (total_leakage_file, 'sorted/Total_Leakage.sorted') or die "Failed to open Total Leakage file\n";
open spreadsheet, '>', 'spreadsheet.csv';
$total = 0;

# process sim ticks
#numBigCores_0.numLittleCores_8.clusters_1.l3size_16MB.bigl2size_2MB.littlel2size_1MB.littlel1size_16kB.bench_SpaceDwarf_conv.stats.txt:sim_ticks                                 93076874000                       # Number of ticks simulated
while(<sim_ticks_file>){
	#/numBigCores_(\d)\.numLittleCores_(\d)\.clusters_(\d).l3size_(\w+).bigl2size_(\w+|N-A).littlel2size_(\w+|N-A).littlel1size_(\w+|N-A).bench_SpaceDwarf_(\w+).stats.txt:sim_ticks *(\d+)/;
	#/numBigCores_(\d)\.numLittleCores_(\d)\.clusters_(\d).l3size_(\w+).bigl2size_(\w+|N-A).littlel2size_(\w+|N-A).littlel1size_(\w+|N-A).bench_WES_(\w+).size_(\d+).stats.txt:sim_ticks *(\d+)/;
	#/numBigCores_(\d)\.numLittleCores_(\d)\.clusters_(\d).l3size_(\w+).bigl2size_(\w+|N-A).littlel2size_(\w+|N-A).littlel1size_(\w+|N-A).bench_OPIR_(\w+).stats.txt:sim_ticks *(\d+)/;
	#/numBigCores_(\d)\.numLittleCores_(\d)\.clusters_(\d).l3size_(\w+).bigl2size_(\w+|N-A).littlel2size_(\w+|N-A).littlel1size_(\w+|N-A).bench_ASPEN_(\w+).stats.txt:sim_ticks *(\d+)/;
	/numBigCores_(\d)\.numLittleCores_(\d)\.clusters_(\d).l3size_(\w+).bigl2size_(\w+|N-A).littlel2size_(\w+|N-A).littlel1size_(\w+|N-A).bench_HPFEC-Synthetic-1.stats.txt:sim_ticks *(\d+)/;
	#print "$_,";
	#print "$9,";
	push(@numBigCores, $1);
	push(@numLittleCores,$2);
	push(@clusters,$3);
	push(@l3size,$4);
	push(@bigl2size,$5);
	push(@littlel2size,$6);
	push(@littlel1size,$7);
	#push(@bench,$8);
	#push(@input_size,$9);
	push(@sim_ticks,$8);
	$total++;
}
#print @numBigCores;
#print "@sim_ticks\n";
# process aggregate l2 size
for (my $i=0; $i < $total; $i++){
	my $bigL2 = 0;
	my $littleL2 = 0;
	if ($numBigCores[$i] == 0){
		push(@numBigL2, 0);
		push(@numLittleL2, $clusters[$i]);
		$littleL2 = $clusters[$i];
	}
	elsif ($numBigCores[$i] == 2){
		push(@numBigL2,1);
		$bigL2 = 1;
		push(@numLittleL2,3);
		$littleL2 = 3;
	}
	elsif ($numBigCores[$i] == 4){
		push(@numBigL2,$clusters[$i]/2);
		$bigL2 = $clusters[$i]/2;
		push(@numLittleL2,$clusters[$i]/2);
		$littleL2 = $clusters[$i]/2;
	}
	elsif ($numBigCores[$i] == 6){
		push(@numBigL2,3);
		$bigL2 = 3;
		push(@numLittleL2,1);
		$littleL2 = 1;
	}
	else{
		push(@numBigL2,$clusters[$i]);
		$bigL2 = $clusters[$i];
		push(@numLittleL2,0);
	}
	my $sizeBig = 0;
	my $sizeLittle = 0;
	if ($bigl2size[$i] == '256kB'){
		$sizeBig = 256;
	}
	if ($bigl2size[$i] == '512kB'){
		$sizeBig = 512;
	}
	if ($bigl2size[$i] == '1MB'){
		$sizeBig = 1024;
	}
	if ($bigl2size[$i] == '2MB'){
		$sizeBig = 2048;
	}
	#$sizeBig = $sizeBig * $bigL2;
	push(@totBigL2Size,$sizeBig * $bigL2);
	if ($littlel2size[$i] == '256kB'){
		$sizeLittle = 256;
	}
	if ($littlel2size[$i] == '512kB'){
		$sizeLittle = 512;
	}
	if ($littlel2size[$i] == '1MB'){
		$sizeLittle = 1024;
	}
	if ($littlel2size[$i] == '2MB'){
		$sizeLittle = 2048;
	}
	#$sizeLittle = $sizeLittle * $littleL2;
	push(@totLittleL2Size,$sizeLittle * $littleL2);
}
#print "@numBigL2\n";
#print "@bigl2size\n";
#print "@totBigL2Size\n";
#print "@numLittleL2\n";
#print "@totLittleL2Size\n";

# do some label changing for special cases
for (my $i=0; $i < $total; $i++){
	if ($numLittleCores[$i] == 0){
		$littlel2size[$i] = 'N/A';
		$littlel1size[$i] = 'N/A';
	}
	if ($numLittleCores[$i] != 8 && $clusters[$i] == 1){
		$littlel2size[$i] = 'N/A';
	}
	elsif ($numLittleCores[$i] == 2 && $clusters[$i] == 2){
		$littlel2size[$i] = 'N/A';
	}
	
	if ($numBigCores[$i] == 0){
		$bigl2size[$i] = 'N/A';
	}
	#print "$littlel2size[$i],";
}
#process sim seconds
while(<sim_seconds_file>){
	/stats.txt:sim_seconds *(\d+[\.\d+]*)/;
	push(@sim_seconds,$1);
}
#print @sim_seconds;
#print "$sim_seconds[0]\n";
#print "$sim_seconds[1]\n";
#process host seconds
while(<host_seconds_file>){
	/stats.txt:host_seconds *(\d+[\.\d+]*)/;
	push(@host_seconds,$1);
}
#process l3 miss rate
#numBigCores_0.numLittleCores_8.clusters_1.l3size_16MB.bigl2size_2MB.littlel2size_1MB.littlel1size_16kB.bench_SpaceDwarf_conv.stats.txt:host_seconds                                   455.94                       # Real time elapsed on the host
while(<l3_miss_rate_file>){
	/system.l3.overall_miss_rate::total *(\d+[\.\d+]*)/;
	push(@l3_miss_rate,$1);
}
#process l2 miss rates
#numBigCores_0.numLittleCores_8.clusters_1.l3size_16MB.bigl2size_2MB.littlel2size_1MB.littlel1size_16kB.bench_SpaceDwarf_conv.stats.txt:host_seconds                                   455.94                       # Real time elapsed on the host
while(<l2_miss_rate_file>){
	/.*clusters_(\d).*system.l20.overall_miss_rate::total *(\d+[\.\d+]*)/;
	if($1 == 2){
		#push(@l2_miss_rate,$2);
		#print "$_\n";
		#print "$2,";
		push(@l20_miss_rate,$2);
		my $l20 = $2;
		$_ = <l2_miss_rate_file>;
		#print "$_\n";
		/.system.l21.overall_miss_rate::total *(\d+[\.\d+]*)/;
		#print "$1\n";
		push(@l21_miss_rate,$1);
		my $l21 = $1;
		push(@l22_miss_rate,'N-A');
		push(@l23_miss_rate,'N-A');
		push(@l2_arith_avg_miss_rate,($l20 + $l21)/2);
		push(@l2_harm_avg_miss_rate,(2/(1/$l20 + 1/$l21)));
	}
	else{
		#print "$2,";
		push(@l20_miss_rate,$2);
		my $l20 = $2;
		$_ = <l2_miss_rate_file>;
		/.system.l21.overall_miss_rate::total *(\d+[\.\d+]*)/;
		#print "$1,";
		push(@l21_miss_rate,$1);
		my $l21 = $1;
		$_ = <l2_miss_rate_file>;
		/.system.l22.overall_miss_rate::total *(\d+[\.\d+]*)/;
		#print "$1,";
		push(@l22_miss_rate,$1);
		my $l22 = $1;
		$_ = <l2_miss_rate_file>;
		/.system.l23.overall_miss_rate::total *(\d+[\.\d+]*)/;
		#print "$1\n";
		push(@l23_miss_rate,$1);
		my $l23 = $1;
		push(@l2_arith_avg_miss_rate,($l20 + $l21 + $l22 + $l23)/4);
		push(@l2_harm_avg_miss_rate,(4/(1/$l20 + 1/$l21 + 1/$l22 + 1/$l23)));
	}
	#push(@l2_miss_rate,$1);
	#/system.l21.overall_miss_rate::total *(\d+[/;
}
#print "@l20_miss_rate\n";
#print "\n\n";
#print @l21_miss_rate;
#print "\n\n";
#print @l2_avg_miss_rate;
#$total2 = 0;
#process Area
#numBigCores_0.numLittleCores_8.clusters_1.l3size_16MB.bigl2size_2MB.littlel2size_1MB.littlel1size_16kB.bench_SpaceDwarf_conv.xml.output:  Area = 91.5231 mm^2
while(<area_file>){
	/Area = (\d+[\.\d+]*)/;
	push(@area,$1);
}
#process Peak Power
#numBigCores_0.numLittleCores_8.clusters_1.l3size_16MB.bigl2size_2MB.littlel2size_1MB.littlel1size_16kB.bench_SpaceDwarf_conv.xml.output:  Peak Power = 12.3374 W
while(<peak_power_file>){
	/Peak Power = (\d+[\.\d+]*)/;
	push(@peak_power,$1);
}
#process Runtime Dynamic
#numBigCores_0.numLittleCores_8.clusters_1.l3size_16MB.bigl2size_2MB.littlel2size_1MB.littlel1size_16kB.bench_SpaceDwarf_conv.xml.output:  Runtime Dynamic = 0.547096 W
while(<runtime_dynamic_file>){
	/Runtime Dynamic = (\d+[\.\d+]*)/;
	push(@runtime_dynamic,$1);
}
#process Peak Power
#numBigCores_0.numLittleCores_8.clusters_1.l3size_16MB.bigl2size_2MB.littlel2size_1MB.littlel1size_16kB.bench_SpaceDwarf_conv.xml.output:  Total Leakage = 2.51235 W
while(<total_leakage_file>){
	/Total Leakage = (\d+[\.\d+]*)/;
	push(@total_leakage,$1);
}

#print "@peak_power\n";
=c
#process Area.Summary
#numBigCores_0.numLittleCores_8.clusters_1.l3size_16MB.bigl2size_2MB.littlel2size_1MB.littlel1size_16kB.output:  Area = 78.3584 mm^2
while(<area_file>){
	/numBigCores_(\d)\.numLittleCores_(\d)\.clusters_(\d).l3size_(\w+).bigl2size_(\w+).littlel2size_(\w+).littlel1size_(\w+).output:  Area = (\d+[\.\d+]*)/;
	if($1 == 0 || (($1 == 2 || $1 == 4) && $3 != 1) || ($1 == 6 && $3 == 4)){
		if($6 == '256kB' && $7 == '32kB'){
			next;
		}
		elsif($6 == '2MB' && $7 == '16kB'){
			next;
		}
	}
	push(@area,$8);
	push(@area,$8);
	push(@area,$8);
	$total2++;
}
#print "@area\n";

#process Peak_Power.Summary
while(<peak_power_file>){
	/numBigCores_(\d)\.numLittleCores_(\d)\.clusters_(\d).l3size_(\w+).bigl2size_(\w+).littlel2size_(\w+).littlel1size_(\w+).output:  Peak Power = (\d+[\.\d+]*)/;
	if($1 == 0 || (($1 == 2 || $1 == 4) && $3 != 1) || ($1 == 6 && $3 == 4)){
		if($6 == '256kB' && $7 == '32kB'){
			next;
		}
		elsif($6 == '2MB' && $7 == '16kB'){
			next;
		}
	}
	push(@peak_power,$8);
	push(@peak_power,$8);
	push(@peak_power,$8);
}
#print "@peak_power\n";
=cut

print "processed ", $total, "\n";
#print "processed ", $total2, " mcpat\n";
# add data points to spreadsheet.csv
$count=0;
print spreadsheet "Number of Big Cores";$count++;
print spreadsheet ",Number of Little Cores";$count++;
print spreadsheet ",Clusters";$count++;
print spreadsheet ",L3 Size";$count++;
print spreadsheet ",Big L2 Size";$count++;
print spreadsheet ",Total Big L2 Size (kB)";$count++;
print spreadsheet ",Little L2 Size";$count++;
print spreadsheet ",Total Little L2Size (kB)";$count++;
print spreadsheet ",Aggregate L2 Size (kB)";$count++;
print spreadsheet ",Little L1 Size";$count++;
print spreadsheet ",Benchmark";$count++;
#print spreadsheet ",mark";$count++;
#print spreadsheet ",Input Size";$count++;
print spreadsheet ",Sim Ticks";$count++;
print spreadsheet ",Sim Seconds";$count++;
print spreadsheet ",Host Seconds";$count++;
print spreadsheet ",L3 Miss Rate";$count++;
print spreadsheet ",L20 Miss Rate";$count++;
print spreadsheet ",L21 Miss Rate";$count++;
print spreadsheet ",L22 Miss Rate";$count++;
print spreadsheet ",L23 Miss Rate";$count++;
print spreadsheet ",L2 Arithmetic Mean Miss Rate";$count++;
print spreadsheet ",L2 Harmonic Mean Miss Rate";$count++;
print spreadsheet ",Area";$count++;
print spreadsheet ",TDP";$count++;
print spreadsheet ",TDP Energy (power * time)";$count++;
#print spreadsheet ",TDP EDP (power * time^2)";$count++;
#print spreadsheet ",TDP EDAP (power * area * time ^ 2)";$count++;
#print spreadsheet ",TDP EDA^2P (power * area^2 * time ^ 2)";$count++;
print spreadsheet ",Average Power";$count++;
print spreadsheet ",Average Energy (power * time)";$count++;
#print spreadsheet ",Average EDP (power * time^2)";$count++;
#print spreadsheet ",Average EDAP (power * area * time ^ 2)";$count++;
#print spreadsheet ",Average EDA^2P (power * area^2 * time ^ 2)";$count++;
#print spreadsheet ",Total Leakage";$count++;
print spreadsheet "\n";
for (my $i=0; $i < $total; $i++){
	# skip the ones that didn't finish for whatever reason
	#if ($sim_ticks[$i] == 313327965500 || $sim_ticks[$i] == 0){
	#	next;
	#}
	printf spreadsheet "%s,"x$count."\n",
		$numBigCores[$i],
		$numLittleCores[$i],
		$clusters[$i],
		$l3size[$i],
		$bigl2size[$i],
		$totBigL2Size[$i],
		$littlel2size[$i],
		$totLittleL2Size[$i],
		$totBigL2Size[$i] + $totLittleL2Size[$i],
		$littlel1size[$i],
		"HPFEC-1",
		#$bench[$i],
		#"CHTIptomp",
		#$input_size[$i],
		$sim_ticks[$i],
		$sim_seconds[$i],
		$host_seconds[$i],
		$l3_miss_rate[$i],
		$l20_miss_rate[$i],
		$l21_miss_rate[$i],
		$l22_miss_rate[$i],
		$l23_miss_rate[$i],
		$l2_arith_avg_miss_rate[$i],
		$l2_harm_avg_miss_rate[$i],
		$area[$i],
		$peak_power[$i],
		$sim_seconds[$i] * $peak_power[$i],
		#$sim_seconds[$i] * $peak_power[$i] * $peak_power[$i],
		#$sim_seconds[$i] * $area[$i] * $peak_power[$i] * $peak_power[$i],
		#$sim_seconds[$i] * $area[$i] * $area[$i] * $peak_power[$i] * $peak_power[$i],
		$runtime_dynamic[$i],
		$sim_seconds[$i] * $runtime_dynamic[$i],
		#$sim_seconds[$i] * $runtime_dynamic[$i] * $runtime_dynamic[$i],
		#$sim_seconds[$i] * $area[$i] * $runtime_dynamic[$i] * $runtime_dynamic[$i],
		#$sim_seconds[$i] * $area[$i] * $area[$i] * $runtime_dynamic[$i] * $runtime_dynamic[$i],
		#$total_leakage[$i],
}
print "All done\n";
#print "@l20_miss_rate\n";
close sim_ticks_file;
close host_seconds_file;
close l3_miss_rate_file;
close area_file;
close peak_power_file;
close runtime_dynamic_file;
close total_leakage_file;
close spreadsheet;
