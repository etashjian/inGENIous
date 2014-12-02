# Gnuplot script file for plotting frame times line
set autoscale
unset log
unset label
set xtic auto
set ytic auto
set title "Frame arrival time"
set xlabel "Time (seconds)"
set ylabel "Frame Number"
set key l
#set xr [0:6]
set yr [0:30000]

#plot "data/ScenarioA", "data/B_S0_VM2", 10 * (x-1),20 * (x-1),30 * (x-1),40 * (x-1),50 * (x-1),60 * (x-1),70 * (x-1),80 * (x-1),90 * (x-1),100 * (x-1)
plot "data/ScenarioA", "data/B_VM2_VM3", "data/B_VM3_C", 10 * (x-1),20 * (x-1),30 * (x-1),40 * (x-1),50 * (x-1),60 * (x-1),70 * (x-1),80 * (x-22),90 * (x-47),100 * (x-67)
pause -1
