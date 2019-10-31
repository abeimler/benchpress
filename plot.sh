#!/bin/bash

./build/src/examples/example_06 --plotdata --bench .*test.* > ./examples/example_06.dat
./build/src/examples/example_07 --plotdata --bench .*test.* --bench .*foo.* --bench .*bar.* > ./examples/example_07.dat
./build/src/examples/example_08 --plotdata --bench .*test.* --bench .*foo.* --bench .*bar.* > ./examples/example_08.dat

./build/src/examples/example_08 --plotdata --bench .*test.* --bench .*foo.* --bench .*bar.* --csvoutput ./examples/ --csvsuffix update
./build/src/examples/example_09 --plotdata --bench ".*foo.*10M\\s+entities.*" --bench ".*bar.*10M\\s+entities.*" --csvoutput ./examples/ --csvsuffix 10Mentities --csvsort

./build/src/examples/example_08 --plotdata --bench .*test.* --bench .*foo.* --bench .*bar.* --csvoutput ./examples/ --csvprefix update
./build/src/examples/example_09 --plotdata --bench ".*foo.*10M\\s+entities.*" --bench ".*bar.*10M\\s+entities.*" --csvoutput ./examples/ --csvprefix 10Mentities
./build/src/examples/example_08 --plotdata --bench .*test.* --bench .*foo.* --bench .*bar.* --csvoutput ./examples/ --csvprefix updates --csvunit seconds

gnuplot ./plot06.plt
gnuplot ./plot07.plt
gnuplot ./plot08.plt