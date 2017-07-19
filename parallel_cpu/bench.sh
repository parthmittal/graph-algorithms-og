#!/bin/bash
unset SGE_ROOT

function do_run {
	../data-fixer/compress < ../data-fixer/$1 > fixed.txt
	echo "$1" >> log
	for i in {1..5}
	do
		echo "run "$i >> log
		./test < fixed.txt > temp 2>>log
	done
	rm -f fixed.txt
}

function do_runs {
	do_run roadNet-CA.txt
	do_run roadNet-TX.txt
	do_run roadNet-PA.txt
}

rm -f log

cd ../data-fixer
g++ -O3 -std=c++14 compress.cpp -o compress

cd ../parallel_cpu

make clean
make CXXFLAGS="-D __BRANDES__"
echo "testing brandes algorithm" >> log
do_runs
