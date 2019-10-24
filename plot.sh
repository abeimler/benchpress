#!/bin/bash

./build/src/examples/example_06 --plotdata --bench .*test.* > ./build/src/examples/example_06.dat
./build/src/examples/example_07 --plotdata --bench .*test.* --bench .*foo.* --bench .*bar.* > ./build/src/examples/example_07.dat
./build/src/examples/example_08 --plotdata --bench .*test.* --bench .*foo.* --bench .*bar.* > ./build/src/examples/example_08.dat

gnuplot ./plot06.plt
gnuplot ./plot07.plt
gnuplot ./plot08.plt