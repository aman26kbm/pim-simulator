import csv
import copy
import matplotlib.pyplot as plot
import numpy as np
import subprocess, os
import re

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
mlp_2_layers = [
            "M3072_K2048_N4096",
            "M3072_K4096_N4096",
            "M3072_K4096_N4096",
            "M3072_K4096_N1024"
        ]

def sum_network_energy(layers, csv_reader, ablation):
    sum = 0
    for layer in layers:
        for line in csv_reader:
            if layer in line[0] and ablation in line[0]:
                sum+=float(line[2])
    return sum
def sum_network_time(layers, csv_reader, ablation):
    sum = 0
    for layer in layers:
        for line in csv_reader:
            # print(line[0])
            # print(re.split('_|\.',line[0]))
            if layer in line[0] and (ablation==re.split('_|\.',line[0])[-2] or ablation==''):
                sum+=float(line[1])
    return sum

def get_time(workload_name, dir, ablation):
    with open(f"{dir}/00_stats.csv", 'r') as f:
        csv_reader = list(csv.reader(f))
        scaling_factor = 1/1.5
        if workload_name == "resnet":
            return sum_network_time(resnet_layers, csv_reader, ablation) * scaling_factor
        elif workload_name == "bert":
            return (sum_network_time(bert_layers, csv_reader, ablation)+13136) * scaling_factor#13136 is the time for softmax
        elif workload_name == "mlp":
            return sum_network_time(mlp_2_layers, csv_reader, ablation) * scaling_factor
        else:
            for line in csv_reader:
                if workload_name in line[0]:
                    return float(line[1]) * scaling_factor

def get_energy(workload_name, dir, ablation):
    with open(f"{dir}/00_stats.csv", 'r') as f:
        csv_reader = list(csv.reader(f))
        scaling_factor = 1
        if workload_name == "resnet":
            return sum_network_energy(resnet_layers, csv_reader, ablation) * scaling_factor
        elif workload_name == "bert":
            return sum_network_energy(bert_layers, csv_reader, ablation) * scaling_factor
        elif workload_name == "mlp":
            return sum_network_energy(mlp_2_layers, csv_reader, ablation) * scaling_factor
        else:
            for line in csv_reader:
                print(line)
                if workload_name in line[0]:
                    return float(line[2]) * scaling_factor
# data  = []

# def get_data(workload_list, dir):
#     data = []
#     for workload in workload_list:
#         data.append([workload, get_time(workload, dir), get_energy(workload, dir)])
#     return data

# kernels_workload_list = ['vecadd', 'fir', 'gemv', 'gemm', 'conv2d']
# data=data+get_data(kernels_workload_list, 'output_all_kernels')

# resnet_workload_list = ['resnet']
# data = data + get_data(resnet_workload_list, 'output_resnet')

# bert_workload_list = ['bert']
# data = data + get_data(bert_workload_list, 'output_bert')

# mlp_workload_list = ['mlp']
# data = data + get_data(mlp_workload_list, 'output_mlp_2')