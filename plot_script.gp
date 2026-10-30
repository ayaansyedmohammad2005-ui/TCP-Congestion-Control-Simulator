set title "TCP Tahoe vs TCP Reno Congestion Window (cwnd)"
set xlabel "Round"
set ylabel "Congestion Window (cwnd)"

set grid
set datafile separator ","

set terminal png size 900,600
set output "tcp_comparison_plot.png"

# Plot both datasets on one graph
plot "tahoe_data.csv" using 1:2 title "TCP Tahoe" with linespoints lw 2 pt 7 ps 1.2, \
     "reno_data.csv"  using 1:2 title "TCP Reno"  with linespoints lw 2 pt 5 ps 1.2