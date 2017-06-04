function do_run {
	../data-fixer/compress < ../data-fixer/$1 > fixed.txt
	outfile=memory-test-out/$2$1.massif.out
	echo $outfile
	valgrind --tool=massif --massif-out-file=$outfile ./test < fixed.txt > temp 2> log
	ms_print $outfile > $outfile.print
	rm -f fixed.txt
}

function do_runs {
	do_run roadNet-CA.txt $1
	do_run roadNet-TX.txt $1
	do_run roadNet-PA.txt $1
}

cd ../data-fixer
g++ -O3 -std=c++14 compress.cpp -o compress

cd ../serial

rm -rf memory-test-out
mkdir memory-test-out

make clean
make CXXFLAGS="-D __DP_LCA__"
do_runs bfs_dp_lca-

make clean
make CXXFLAGS="-D __TARJAN_LCA__ -D __UNION_BY_RANK__ -D __PATH_COMPRESSION__"
do_runs bfs_tarjan_rank_compression-

make clean
make CXXFLAGS="-D __TARJAN_LCA__ -D __UNION_BY_RANK__"
do_runs bfs_tarjan_rank-

make clean
make CXXFLAGS="-D __USE_DFS_TREE__"
do_runs dfs-

make clean
make CXXFLAGS="-D __USE_DFS_TREE__ -D __DFS_LESS_SPACE__"
do_runs dfs_less_space-
