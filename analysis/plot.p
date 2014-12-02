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
set yr [0:1000]

plot "non_blocking.5.data", 10 * (x-0),20 * (x-0),30 * (x-0),40 * (x-0),50 * (x-0),60 * (x-0),70 * (x-0),80 * (x-0),90 * (x-0),100 * (x-0)
pause -1
