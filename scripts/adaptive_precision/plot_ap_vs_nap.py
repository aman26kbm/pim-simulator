import sum_network

print("resnet with ap: " + str( sum_network.get_time('resnet', 'output_resnet', '')))
print("resnet with nap: " + str( sum_network.get_time('resnet', 'output_resnet_nap', '')))