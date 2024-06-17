## Run conv2d with different size using the param file 

## The main size run 
./PIM_simulator -c ../configs/DMesh12x10_TileSize1024_CramSize128x128.cfg -m conv2d_lowLatency_inputDup -p ../params/conv2d.param

# conv2d.param 
# N  2
# H  9
# W  9
# C  256
# Stride  1
# R  3
# S  3
# M  256
# E  7
# F  7

./PIM_simulator -c ../configs/DMesh12x10_TileSize1024_CramSize128x128.cfg -m conv2d_lowLatency_inputDup -p ../params/conv2d_half.param

# conv2d_half.param 
# N  1
# H  9
# W  9
# C  256
# Stride  1
# R  3
# S  3
# M  256
# E  7
# F  7

./PIM_simulator -c ../configs/DMesh12x10_TileSize1024_CramSize128x128.cfg -m conv2d_lowLatency_inputDup -p ../params/conv2d_double.param

# conv2d_double.param
# N  4
# H  9
# W  9
# C  256
# Stride  1
# R  3
# S  3
# M  256
# E  7
# F  7