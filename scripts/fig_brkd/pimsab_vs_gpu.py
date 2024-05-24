import csv
import copy
import matplotlib.pyplot as plot
import numpy as np
import subprocess, os

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
# def get_time(workload_name, csv_reader):
#     scaling_factor = 1/1.5
#     if workload_name == "resnet":
#         sum=0
#         for line in csv_reader:
#             if workload_name in line[0]:
#                 if "conv1" in line[0]:
#                     sum+=float(line[1])
#                 elif "conv2_1_1" in line[0]:
#                     sum+=float(line[1]) * 4
#                 elif "conv3_1_1" in line[0]:
#                     sum+=float(line[1])
#                 elif "conv3_1_2" in line[0]:
#                     sum+=float(line[1]) * 3
#                 elif "conv4_1_1" in line[0]:
#                     sum+=float(line[1])
#                 elif "conv4_1_2" in line[0]:
#                     sum+=float(line[1]) * 3
#                 elif "conv5_1_1" in line[0]:
#                     sum+=float(line[1])
#                 elif "conv5_1_2" in line[0]:
#                     sum+=float(line[1]) * 3
#         return float(sum)*scaling_factor
#     elif workload_name == "bert":
#         return 1.0
#     elif workload_name == "mlp":
#         return 1.0
#     else:
#         for line in csv_reader:
#             if workload_name in line[0]:
#                 return float(line[1])*scaling_factor
def sum_network_energy(layers, csv_reader):
    sum = 0
    for layer in layers:
        for line in csv_reader:
            if layer in line[0]:
                sum+=float(line[2])
    return sum
def sum_network_time(layers, csv_reader):
    sum = 0
    for layer in layers:
        for line in csv_reader:
            if layer in line[0]:
                sum+=float(line[1])
    return sum

def get_time(workload_name, csv_reader):
    scaling_factor = 1/1.5
    if workload_name == "resnet":
        return sum_network_time(resnet_layers, csv_reader) * scaling_factor
    elif workload_name == "bert":
        return (sum_network_time(bert_layers, csv_reader)+13136) * scaling_factor#13136 is the time for softmax
    elif workload_name == "mlp":
        return sum_network_time(mlp_2_layers, csv_reader) * scaling_factor
    else:
        for line in csv_reader:
            if workload_name in line[0]:
                return float(line[1]) * scaling_factor

def get_energy(workload_name, csv_reader):
    scaling_factor = 1
    if workload_name == "resnet":
        return sum_network_energy(resnet_layers, csv_reader) * scaling_factor
    elif workload_name == "bert":
        return sum_network_energy(bert_layers, csv_reader) * scaling_factor
    elif workload_name == "mlp":
        return sum_network_energy(mlp_2_layers, csv_reader) * scaling_factor
    else:
        for line in csv_reader:
            print(line)
            if workload_name in line[0]:
                return float(line[2]) * scaling_factor
data  = []

def get_data(workload_list, dir):
    data = []
    with open(f"{dir}/00_stats.csv", 'r') as f:
        csv_reader = csv.reader(f)
        lines = list(csv_reader)
        for workload in workload_list:
            data.append([workload, get_time(workload, lines), get_energy(workload, lines)])
    return data

kernels_workload_list = ['vecadd', 'fir', 'gemv', 'gemm', 'conv2d']
data=data+get_data(kernels_workload_list, 'output_all_kernels')

resnet_workload_list = ['resnet']
data = data + get_data(resnet_workload_list, 'output_resnet')

bert_workload_list = ['bert']
data = data + get_data(bert_workload_list, 'output_bert')

mlp_workload_list = ['mlp']
data = data + get_data(mlp_workload_list, 'output_mlp_2')

workload_list = kernels_workload_list + resnet_workload_list + bert_workload_list + mlp_workload_list
with open('gpu.csv') as f_gpu:
    csv_reader = csv.reader(f_gpu)
    lines = list(csv_reader)
    for line in lines:
        for i in range(len(data)):
            if data[i][0] in line[0]:
                data[i]+=[float(line[1]), float(line[2])]
    print(data)

with open('pimsav-vs-gpu.csv', 'w', newline='') as f:
    writer = csv.writer(f)
    writer.writerow(['Workload', 'PIMSAB Time', 'PIMSAB Energy', 'GPU Time', 'GPU Energy'])
    writer.writerows(data)

exec_time_speedup = [[x[0], x[3]/x[1]] for x in data]

print(exec_time_speedup)

energy_improve = [[x[0], x[4]/x[2]] for x in data]

print(energy_improve)

fig, axes = plot.subplots(1, 2)
axes[0].bar(np.arange(0, len(exec_time_speedup)) * 1.5, [x[1] for x in exec_time_speedup], width=1, edgecolor='k', color='w', label='Execution Time Speedup')

axes[0].set_ylim(0, 4)

# axes.legend(bbox_to_anchor=(0.3, 1.25), handlelength=0.75, ncol=4, loc='upper center',
#             labelspacing=0.2, handletextpad=0.5, columnspacing=0.5, frameon=False,
#             fontsize=10)

# axes.bar(np.arange(0, len(exec_time_speedup)) * 1.5, exec_time_speedup[:][1], width=1, edgecolor='k', color='b', label='Execution Time Speedup')
axes[0].set_xticks(np.arange(0, len(exec_time_speedup)) * 1.5)
axes[0].set_xticklabels(workload_list, rotation=90)
axes[0].set_ylabel('Exec. Time Speedup')
# axes.xaxis.grid(False)
# axes.set_axisbelow(True)

axes[1].bar(np.arange(0, len(exec_time_speedup)) * 1.5, [x[1] for x in energy_improve], width=1, edgecolor='k', color='w', label='Execution Time Speedup')

axes[1].set_ylim(0, 8)

# axes.legend(bbox_to_anchor=(0.3, 1.25), handlelength=0.75, ncol=4, loc='upper center',
#             labelspacing=0.2, handletextpad=0.5, columnspacing=0.5, frameon=False,
#             fontsize=10)

# axes.bar(np.arange(0, len(exec_time_speedup)) * 1.5, exec_time_speedup[:][1], width=1, edgecolor='k', color='b', label='Execution Time Speedup')
axes[1].set_xticks(np.arange(0, len(exec_time_speedup)) * 1.5)
axes[1].set_xticklabels(workload_list, rotation=90)
axes[1].set_ylabel('Energy Improvement')
# axes.xaxis.grid(False)
# axes.set_axisbelow(True)
fig.subplots_adjust(top=0.5, bottom=0.2, left=0.2, right=0.8, wspace=0.3)

plot.show()



fname = 'pimsab-vs-gpu'
fig.savefig(f'{fname}.pdf')
subprocess.check_output(f'pdfcrop {fname}.pdf', shell=True)
subprocess.check_output(f'mv {fname}-crop.pdf {fname}.pdf', shell=True)