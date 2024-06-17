# PIMSAB commands to run Duality Cache workloads
./PIM_simulator -c ../configs/DMesh12x10_TileSize256_CramSize256x256.cfg -m backprop_65536_16 -l backprop_65536_16.DMesh12x10_TileSize256_CramSize256x256.log
./PIM_simulator -c ../configs/DMesh12x10_TileSize256_CramSize256x256.cfg -m dwt_1024 -l dwt_1024.DMesh12x10_TileSize256_CramSize256x256.log
./PIM_simulator -c ../configs/DMesh12x10_TileSize256_CramSize256x256.cfg -m gausselim_256 -l gausselim_256.DMesh12x10_TileSize256_CramSize256x256.log
./PIM_simulator -c ../configs/DMesh12x10_TileSize256_CramSize256x256.cfg -m hotspot_1024_1024 -l hotspot_1024_1024.DMesh12x10_TileSize256_CramSize256x256.log

# Hotspot3D ( There are 3 worklaods related to hotspot. Non of then totaly match the complete execution of the workload )
# The actual FP32 workload can be found here https://github.com/yuhc/gpu-rodinia/blob/master/cuda/hotspot3D/3D.cu
# DRAM writes are taken from hotspot3d_512_8_a100 
./PIM_simulator -c ../configs/DMesh12x10_TileSize256_CramSize256x256.cfg -m hotspot3d_512_8_a100 -l hotspot_1024_1024.DMesh12x10_TileSize256_CramSize256x256.log


