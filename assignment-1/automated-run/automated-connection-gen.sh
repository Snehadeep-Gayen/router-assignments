# bash script to run connection-gen.py with different parameters
topos=( "ARPANET-Topo.txt" "NSFNET-Topo.txt")
bw_x=(2 4 6 8 10 12 14 16 18 20)
number_of_connections=(100 200 300)
for topo in "${topos[@]}"
do
    for bw in "${bw_x[@]}"
    do
        for num in "${number_of_connections[@]}"
        do
            bw_10=$(echo "$bw * 0.01" | bc -l)
            bw_1=$(echo "$bw * 0.001" | bc -l)
            python3 connection-gen.py $num $topo 0.001 $bw_1 $bw_10
        done
    done
done