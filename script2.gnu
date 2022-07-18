set term gif animate
set terminal gif animate delay 0.000005
set output "test.gif"
stats "data2.txt" name "A" nooutput
set xrange [A_min_x:A_max_x]
set yrange [A_min_y - 1:A_max_y + 1]
do for [i=0 : int(A_blocks - 1)]{plot "data2.txt" index i}
set output 'output.gif'
