# Scale font and line width (dpi) by changing the size! It will always display stretched.
set terminal png size 1024,800 enhanced font "Arial,14"
set output 'result06.png'

# Key means label...
set key inside bottom right
set xlabel 'Test'
set ylabel 'Time per Operation (ns/op)'
set title 'Benchmark'

set logscale x 2
set logscale y 10
set format x "2^{%L}"
set format y "10^{%L}"

## the labels are in Alphabetical order
plot "build/src/examples/example_06.dat" using 1:2 title 'test' with lines