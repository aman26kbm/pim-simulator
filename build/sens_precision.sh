# Run performance sensitivity to varying precision 

# list_bm=(
#     'fir_med_inp_256bit_loads'
# )


list_bm=(
    'vadd'
    'fir_med_inp_256bit_loads'
    'gemv_new'
    'gemm_systolic'
    'conv2d_lowLatency_inputDup'
)

list_bit=(
    '4'
    '5'
    '6'
    '7'
    '8'
)

# rm -rf precision_sens/*
mkdir -p precision_sens/
for j in "${list_bm[@]}"
    do
    for i in "${list_bit[@]}"
        do
        echo "Running " "$j"_int"$i" 
        mkdir -p precision_sens/$j/"$j"_int"$i"
        echo "./PIM_simulator -c ../configs/DMesh12x10_TileSize256_CramSize256x256.cfg -m "$j"_int"$i" -l precision_sens/$j/"$j"_int"$i"/ &> precision_sens/$j/"$j"_net"$i"_run.log"
        ./PIM_simulator -c ../configs/DMesh12x10_TileSize256_CramSize256x256.cfg -m "$j"_int"$i" -l precision_sens/$j/"$j"_int"$i"/ &> precision_sens/$j/"$j"_net"$i"_run.log &
    done
done

