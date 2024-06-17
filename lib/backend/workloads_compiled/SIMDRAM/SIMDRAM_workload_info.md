# SIMDRAM mapping from PIMRA Comparision sheet to workload_compiled (as undestood by Kaustubh) https://docs.google.com/spreadsheets/d/1njzAOTV0o_Iy71bgavtyzLt91Wj0vEC8Gz7M1oJwYJ8/edit?gid=363901466#gid=363901466

## VGG-13 
Network Layers     --> Workload name in PIM_simulator 
conv2d-3-96        --> vgg_bnn_13_1
pool-3x3           --> vgg_bnn_13_2
conv2d-96-256      --> vgg_bnn_13_3
conv-256-384       --> vgg_bnn_13_256_512
conv-384-384       --> vgg_bnn_13_512_512
conv-384-256       --> vgg_bnn_13_512_256
conv-flatten-4096  --> vgg_mv_4096_256
conv-4096-4096     --> vgg_mv_4096_4096
mv-4096-4096       --> vgg_mv_store_4096_4096


## VGG-16 
Network Layers     --> Workload name in PIM_simulator 
conv2d-3-96        --> vgg_bnn_13_1
pool-3x3           --> vgg_bnn_13_2
conv2d-96-256      --> vgg_bnn_13_3
conv-256-384       --> vgg_bnn_13_256_512
conv-384-384       --> vgg_bnn_13_512_512
conv-384-256       --> vgg_bnn_13_512_256
conv-256-384       --> vgg_bnn_13_256_512
conv-384-384       --> vgg_bnn_13_512_512
conv-384-256       --> vgg_bnn_13_512_256
conv-flatten-4096  --> vgg_mv_4096_256
conv-4096-4096     --> vgg_mv_4096_4096
mv-4096-4096       --> vgg_mv_store_4096_4096

## LeNet
Network Layers  --> Workload name in PIM_simulator 
conv2d-1-6      --> len_bnn_1
pool-2x2        --> lenet_bnn_2
conv-6-16       --> len_bnn_3
pool-2x2        --> lenet_4
mv-120          --> (Could not find the worklaod)
mv-84           --> (Could not find the worklaod)
mv-10           --> (Could not find the worklaod)