# Run performance sensitivity to hardware parameters 

# list_bm=(
#     'fir_med_inp_256bit_loads'
# )

# list_comp_config=(
#     'DMesh12x10_TileSize64_CramSize512x512.cfg'
# )

list_bm=(
    'vadd'
    'fir_med_inp_256bit_loads'
    'gemv_new'
    'gemm_systolic'
    'conv2d_lowLatency_inputDup'
)

list_comp_config=(
    'DMesh12x10_TileSize64_CramSize512x512.cfg'
    'DMesh12x10_TileSize256_CramSize256x256.cfg'
    'DMesh12x10_TileSize1024_CramSize128x128.cfg'
)


list_TilevsArray_config=(
    'DMesh12x5_TileSize512_CramSize256x256.cfg'
    'DMesh12x10_TileSize256_CramSize256x256.cfg'
    'DMesh12x20_TileSize128_CramSize256x256.cfg'
)

list_dram_config=(
    'DMesh6x20_TileSize256_CramSize256x256.cfg'
    'DMesh12x10_TileSize256_CramSize256x256.cfg'
    'DMesh24x5_TileSize256_CramSize256x256.cfg'
)


# rm -rf hw_sens/comp_intensity/*
mkdir -p hw_sens/comp_intensity/
for i in "${list_comp_config[@]}"
do
    for j in "${list_bm[@]}"
    do
        echo "Running " "$j"_"$i" 
        mkdir -p hw_sens/comp_intensity/$i/$j/
        echo "./PIM_simulator -c ../configs/$i -m $j -l hw_sens/comp_intensity/$i/$j/$j.$i.log &> hw_sens/comp_intensity/$i/$j.$i.run.log"
        ./PIM_simulator -c ../configs/$i -m $j -l hw_sens/comp_intensity/$i/$j/$j.$i.log &> hw_sens/comp_intensity/$i/$j.$i.run.log &
    done
done



# rm -rf hw_sens/tileVSarray/*
mkdir -p hw_sens/tileVSarray/
for i in "${list_TilevsArray_config[@]}"
do
    for j in "${list_bm[@]}"
    do
        echo "Running " "$j"_"$i" 
        mkdir -p hw_sens/tileVSarray/$i/$j/
        echo "./PIM_simulator -c ../configs/$i -m $j -l hw_sens/tileVSarray/$i/$j/$j.$i.log &> hw_sens/tileVSarray/$i/$j.$i.run.log"
        ./PIM_simulator -c ../configs/$i -m $j -l hw_sens/tileVSarray/$i/$j/$j.$i.log &> hw_sens/tileVSarray/$i/$j.$i.run.log &
    done
done




# rm -rf hw_sens/dram/*
mkdir -p hw_sens/dram/
for i in "${list_dram_config[@]}"
do
    for j in "${list_bm[@]}"
    do
        echo "Running " "$j"_"$i" 
        mkdir -p hw_sens/dram/$i/$j/
        echo "./PIM_simulator -c ../configs/$i -m $j -l hw_sens/dram/$i/$j/$j.$i.log &> hw_sens/dram/$i/$j.$i.run.log"
        ./PIM_simulator -c ../configs/$i -m $j -l hw_sens/dram/$i/$j/$j.$i.log &> hw_sens/dram/$i/$j.$i.run.log &
    done
done

