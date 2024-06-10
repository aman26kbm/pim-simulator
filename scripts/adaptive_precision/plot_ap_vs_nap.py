import sum_network
print("conv2d with ap: " + str( sum_network.get_time('conv2d', 'output_all_kernels', '')))
print("conv2d with nap: " + str( sum_network.get_time('conv2d', 'output_all_kernels_nap', '')))

print("fir with ap: " + str( sum_network.get_time('fir', 'output_all_kernels', '')))
print("fir with nap: " + str( sum_network.get_time('fir', 'output_all_kernels_nap', '')))

print("gemm with ap: " + str( sum_network.get_time('gemm', 'output_all_kernels', '')))
print("gemm with nap: " + str( sum_network.get_time('gemm', 'output_all_kernels_nap', '')))

print("gemv with ap: " + str( sum_network.get_time('gemv', 'output_all_kernels', '')))
print("gemv with nap: " + str( sum_network.get_time('gemv', 'output_all_kernels_nap', '')))

print("vecadd with ap: " + str( sum_network.get_time('vecadd', 'output_all_kernels', '')))
print("vecadd with nap: " + str( sum_network.get_time('vecadd', 'output_all_kernels_nap', '')))

print("resnet with ap: " + str( sum_network.get_time('resnet', 'output_resnet', '')))
print("resnet with nap: " + str( sum_network.get_time('resnet', 'output_resnet_nap', '')))

print("bert with ap: " + str( sum_network.get_time('bert', 'output_bert', '')))
print("bert with nap: " + str( sum_network.get_time('bert', 'output_bert_nap', '')))
