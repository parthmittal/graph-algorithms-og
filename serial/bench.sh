function do_run {
	../data-fixer/compress < ../data-fixer/$1 > fixed.txt
	echo "$1" >> log
	for i in {1..30}
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

cd ../serial

make clean
make CXXFLAGS="-D __TARJAN_LCA__ -D __UNION_BY_RANK__ -D __PATH_COMPRESSION__"
echo "using BFS-Tree, with Tarjan's offline LCA algorithm, with Union by Rank and Path Compression" >> log
do_runs

make clean
make CXXFLAGS="-D __TARJAN_LCA__ -D __UNION_BY_RANK__"
echo "using BFS-Tree, with Tarjan's offline LCA algorithm, with Union by Rank" >> log
do_runs

make clean
make CXXFLAGS="-D __USE_DFS_TREE__"
echo "using DFS-Tree, with O(E) space on stack"
do_runs

make clean
make CXXFLAGS="-D __USE_DFS_TREE -D __DFS_LESS_SPACE__"
echo "using DFS-Tree, with O(V) space on stack"
do_runs
