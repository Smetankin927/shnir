set term pop
stats "data.txt" name "A" nooutput
set xrange [-8:8]
set yrange [A_min_y - 1:A_max_y + 1]
plot 'data.txt' using 2:1 pt 7 ps 0.5
