import matplotlib
import matplotlib.pyplot as plot
import pandas
import sys
import csv
import numpy as np
import functools
import operator
import matplotlib.gridspec as gridspec
import subprocess, os

print('Number of arguments:', len(sys.argv), 'arguments.')
print('Argument List:', str(sys.argv))
def normalize(data):
    # data = [float(data[3])/float(data[5]),float(data[2])/float(data[5]),float(data[1])/float(data[5]),float(data[4])/float(data[5])]
    data = [float(data[2])/float(data[4]),float(data[1])/float(data[4]),float(data[0])/float(data[4]),float(data[3])/float(data[4])]
    # print(data)
    return data



def sum_network(kernel_name, network_layers, input_file_dir, ablation):
    processed_lines = []
    with open(os.path.join(os.getcwd(), input_file_dir, '03_chart_breakdown_avg.csv'), 'r') as f:
        csv_reader = csv.reader(f)
        for line in csv_reader:
            workload_name = line[0]
            if kernel_name in workload_name and ablation in workload_name:
                processed_lines.append(line)
    processed_lines_full = []
    for layer in network_layers:
        for line in processed_lines:
            if layer in line[0]:
                processed_lines_full.append(line)
    
    network_sum = [0,0,0,0,0]
    for line in processed_lines_full:
        for index in range(1,6):
            network_sum[index-1] += float(line[index])

    # network_sum = normalize(network_sum)
    return network_sum

def sum_resnet(ablation):
    input_file_dir = 'output_resnet'
    resnet_layers = [
        "resnet_conv1",
        "resnet_conv2_1_1",
        "resnet_conv2_1_1",
        "resnet_conv2_1_1",
        "resnet_conv2_1_1",
        "resnet_conv3_1_1",
        "resnet_conv3_1_2",
        "resnet_conv3_1_2",
        "resnet_conv3_1_2",
        "resnet_conv4_1_1",
        "resnet_conv4_1_2",
        "resnet_conv4_1_2",
        "resnet_conv4_1_2",
        "resnet_conv5_1_1",
        "resnet_conv5_1_2",
        "resnet_conv5_1_2",
        "resnet_conv5_1_2"
    ]
    return sum_network("resnet", resnet_layers, input_file_dir, ablation)[4]

def sum_bert(ablation):
    input_file_dir = 'output_bert'

    bert_layers = [
        "M384_K768_N768",
        "M384_K768_N768",
        "M384_K768_N768",
        "M384_K64_N384",
        "M384_K64_N384",
        "M384_K64_N384",
        "M384_K64_N384",
        "M384_K64_N384",
        "M384_K64_N384",
        "M384_K64_N384",
        "M384_K64_N384",
        "M384_K64_N384",
        "M384_K64_N384",
        "M384_K64_N384",
        "M384_K64_N384",
        "M384_K384_N64",
        "M384_K384_N64",
        "M384_K384_N64",
        "M384_K384_N64",
        "M384_K384_N64",
        "M384_K384_N64",
        "M384_K384_N64",
        "M384_K384_N64",
        "M384_K384_N64",
        "M384_K384_N64",
        "M384_K384_N64",
        "M384_K384_N64",
        "M384_K768_N768",
        "M384_K768_N3072",
        "M384_K3072_N768"
    ]


    return sum_network("gemm", bert_layers, input_file_dir, ablation)[4]

def sum_mlp_1():
    input_file_dir = 'output_mlp_1'
    mlp_1_layers = [
        "M1024_K4096_N4096",
        "M1024_K4096_N2048",
        "M1024_K2048_N1024",
        "M1024_K1024_N512",
        "M1024_K512_N256"
    ]
    return sum_network("gemm", mlp_1_layers, input_file_dir)

def sum_mlp_2():
    input_file_dir = 'output_mlp_2'
    mlp_2_layers = [
        "M3072_K2048_N4096",
        "M3072_K4096_N4096",
        "M3072_K4096_N4096",
        "M3072_K4096_N1024"
    ]
    return sum_network("gemm", mlp_2_layers, input_file_dir)
def sum_microbench():
    data = np.zeros([5,5])
    with open(os.path.join(os.getcwd(), 'output_all_kernels', '03_chart_breakdown_avg.csv'), 'r') as f:
        csv_reader = csv.reader(f)
        for line in csv_reader:
            workload_name = line[0]
            if('bus' in workload_name): continue
            elif('workload_name' in workload_name): continue
            elif('constOpOff' in workload_name): continue
            elif('crossCramShiftOff' in workload_name): continue
            elif('oneToAllBroadcast' in workload_name): continue
            elif('shuffleOff' in workload_name): continue
            elif('everythingOff' in workload_name): continue
            else:
                # print(line)
                row=0
                # ['vecadd', 'fir', 'gemv', 'gemm', 'conv2d', 'resnet18']
                if('conv2d' in workload_name): row=4
                elif('fir' in workload_name): row=1
                elif('gemm' in workload_name): row=3
                elif('gemv' in workload_name): row=2
                elif('vecadd' in workload_name): row=0
                print(row)
                data[row] = line[1:]
    return data