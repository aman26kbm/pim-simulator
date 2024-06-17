# Run performance sensitivity to Varying size  

# list_bm=(
#     'conv2d_lowLatency_inputDup'
# )


list_bm=(
    'vadd'
    'fir_med_inp_256bit_loads'
    'gemv_new'
    'gemm_systolic'
)
# note 'conv2d_lowLatency_inputDup' is run using the param file


list_bit=(
    'half'
    'main'
    'double'
)

# rm -rf size_sens/*
mkdir -p size_sens/
for j in "${list_bm[@]}"
    do
    for i in "${list_bit[@]}"
        do
        echo "Running " "$j"_"$i" 
        mkdir -p size_sens/$j/"$j"_"$i"
        echo "./PIM_simulator -c ../configs/DMesh12x10_TileSize256_CramSize256x256.cfg -m "$j"_"$i" -l size_sens/$j/"$j"_"$i"/ &> size_sens/$j/"$j"_"$i"_run.log"
        ./PIM_simulator -c ../configs/DMesh12x10_TileSize256_CramSize256x256.cfg -m "$j"_"$i" -l size_sens/$j/"$j"_"$i"/ &> size_sens/$j/"$j"_"$i"_run.log &
    done
done

