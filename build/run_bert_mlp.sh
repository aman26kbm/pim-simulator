
# Handcoded GEMM
# list=(
# 'gemm_tiled_M384_K3072_N768'
# 'gemm_tiled_M384_K384_N64'
# 'gemm_tiled_M384_K64_N384'
# 'gemm_tiled_M384_K768_N3072'
# 'gemm_tiled_M384_K768_N768'
# )

# list=(
#     'gemm_tiled_M1024_K4096_N2048'
#     'gemm_tiled_M1024_K2048_N1024'
#     'gemm_tiled_M1024_K1024_N512'
#     'gemm_tiled_M1024_K512_N256'
#     'gemm_tiled_M1024_K4096_N4096'
# )

# BERT + MLPs
# list=(
#     'gemm_tiled_M1024_K1024_N512'
#     'gemm_tiled_M1024_K2048_N1024'
#     'gemm_tiled_M1024_K4096_N2048'
#     'gemm_tiled_M1024_K4096_N4096'
#     'gemm_tiled_M1024_K512_N256'
#     'gemm_tiled_M3072_K2048_N4096'
#     'gemm_tiled_M3072_K4096_N1024'
#     'gemm_tiled_M3072_K4096_N4096'
#     'gemm_tiled_M384_K3072_N768'
#     'gemm_tiled_M384_K384_N64'
#     'gemm_tiled_M384_K64_N384'
#     'gemm_tiled_M384_K768_N3072'
#     'gemm_tiled_M384_K768_N768'
# )

# IntraCRAM Exp 
# list=(
#     'gemmIntraCRAM_M384_K768_N768'
#     'gemmIntraCRAM_M384_K768_N3072'
#     'gemmIntraCRAM_M384_K3072_N768'
# )


for i in "${list[@]}"
do
    rm -rf intraCRAM/$i
    mkdir -p intraCRAM/$i
    echo "Running " "$i"
    echo "./PIM_simulator -c ../configs/DMesh12x10_TileSize256_CramSize256x256.cfg -m $i -l intraCRAM/$i/$i.DMesh12x10_TileSize256_CramSize256x256.log &> intraCRAM/$i/$i.log"
    ./PIM_simulator -c ../configs/DMesh12x10_TileSize256_CramSize256x256.cfg -m $i -l intraCRAM/$i/$i.DMesh12x10_TileSize256_CramSize256x256.log &> intraCRAM/$i/$i.log &
done