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
set xr [0:10]
set yr [0:10]

#plot "data1.test", (x-1), 0.5*(x-1), 1.5*(x-1)
#plot "data1.test", 3*(x-0)
plot "data1.test",0.5*(x - 0),1*(x - 0),0.5*(x - 1),1*(x - 1),0.5*(x - 2),1*(x - 2),0.5*(x - 3),1*(x - 3),0.5*(x - 4),1*(x - 4),1.5*(x - 4),0.5*(x - 5),1*(x - 5),1.5*(x - 5),2*(x - 5),0.5*(x - 6),1*(x - 6),1.5*(x - 6),2*(x - 6),2.5*(x - 6),0.5*(x - 7),1*(x - 7),1.5*(x - 7),2*(x - 7),2.5*(x - 7),3*(x - 7),0.5*(x - 8),1*(x - 8),1.5*(x - 8),2*(x - 8),2.5*(x - 8),3*(x - 8),0.5*(x - 9),1*(x - 9),1.5*(x - 9),2*(x - 9),2.5*(x - 9),3*(x - 9)
pause -1
