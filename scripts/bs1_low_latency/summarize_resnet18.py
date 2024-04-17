import csv
import os
import glob
import sys

print('Number of arguments:', len(sys.argv), 'arguments.')
print('Argument List:', str(sys.argv))

path = str(sys.argv[1])

header = ["layer", "N", "total cycles", "weight load", "input load", "compute", "store"]
data = []

for filename in sorted(glob.glob(os.path.join(path, '*.log'))):
   with open(os.path.join(os.getcwd(), filename), 'r') as f: # open in readonly mode
        # read layer name from this log file
        print(filename)
        layer = filename.split("/")[-1].split(".")[0]
        print(layer)
        layer_data = []
        # append new layer data if not exist
        exist = False
        for i in range(len(data)):
            if data[i][0] == layer:
                layer_data = data[i]
                exist = True
                break
        if not exist:
            layer_data = [layer, -1, -1, -1, -1, -1, -1]
            data.append(layer_data)
        
        workload = filename.split("/")[-1].split(".")[1]
        print(workload)

        # read N from app_param file with same layer name
        with open(os.path.join(os.getcwd(), path, layer+"."+workload+"."+"log.app_param"), 'r') as f_param:
            for line in f_param:
                if "N" in line:
                    words = line.split()
                    N = int(words[1])
                    print(N)
                    layer_data[1] = N
                    break
        
        # read cycle number from this log file
        cycles = -1
        for line in f:
            if "Chip#0 Tile#0 has ticked" in line:
                words = line.split()
                cycles = int(words[4])
                print(cycles)
                break
        
        #determine which column to insert the cycles
        if workload == "param_conv2d_low_latency":
            layer_data[2] = cycles
        elif workload == "param_conv2d_low_latency_only_load_weight":
            layer_data[3] = cycles
        elif workload == "param_conv2d_low_latency_only_load_input":
            layer_data[4] = cycles
        elif workload == "param_conv2d_low_latency_only_compute":
            layer_data[5] = cycles
        elif workload == "param_conv2d_low_latency_only_store":
            layer_data[6] = cycles
        else:
            print("ERROR: unknown workload")

print(data)
data_out = []
data_out.append(data[0][:])
data_out.append(data[1][:] )
data_out.append(data[1][:] )
data_out[-1][0] = "resnet_conv2_1_2"
data_out.append(data[1][:] )
data_out[-1][0] = "resnet_conv2_2_1"
data_out.append(data[1][:] )
data_out[-1][0] = "resnet_conv2_2_2"
data_out.append(data[2][:] )
data_out.append(data[3][:] )
data_out.append(data[3][:] )
data_out[-1][0] = "resnet_conv3_2_1"
data_out.append(data[3][:] )
data_out[-1][0] = "resnet_conv3_2_2"
data_out.append(data[4][:] )
data_out.append(data[5][:] )
data_out.append(data[5][:] )
data_out[-1][0] = "resnet_conv4_2_1"
data_out.append(data[5][:] )
data_out[-1][0] = "resnet_conv4_2_2"
data_out.append(data[6][:] )
data_out.append(data[7][:] )
data_out.append(data[7][:] )
data_out[-1][0] = "resnet_conv5_2_1"
data_out.append(data[7][:] )
data_out[-1][0] = "resnet_conv5_2_2"

with open(os.path.join(path, '00_layer_breakdown.csv'), 'w', encoding='UTF8', newline='') as f_stats:
    writer = csv.writer(f_stats)
    writer.writerow(header)
    for i in range(len(data_out)):
        writer.writerow(data_out[i])