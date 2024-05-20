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



def sum_network(kernel_name, network_layers, input_file_dir):
    processed_lines = []
    with open(os.path.join(os.getcwd(), input_file_dir, '03_chart_breakdown_avg.csv'), 'r') as f:
        csv_reader = csv.reader(f)
        for line in csv_reader:
            workload_name = line[0]
            if kernel_name in workload_name:
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

def sum_resnet():
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
    return sum_network("resnet", resnet_layers, input_file_dir)

def sum_bert():
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


    return sum_network("gemm", bert_layers, input_file_dir)

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




# resnet_sum = sum_resnet()
# bert_sum = sum_bert()
# microbench_data = sum_microbench()
# mlp_1_sum = sum_mlp_1()
workloads = ['vecadd', 'fir', 'gemv', 'gemm', 'conv2d', 'resnet18','bert', 'mlp_charm']
num_workloads = len(workloads)
data = np.zeros([num_workloads,4])
data[0:5] = [normalize(row) for row in sum_microbench()]
data[5] = normalize(sum_resnet())
data[6] = normalize(sum_bert())
data[7] = normalize(sum_mlp_2())

print(data)


data = data.transpose()

matplotlib.rcParams['lines.linewidth'] = 2.5

plot.style.use('bmh')
binary = matplotlib.cm.get_cmap('binary')

fig, axes = plot.subplots(1, 1)
b_acc = np.zeros((num_workloads, ))
to_gm = []
to_label = []
colors = ['w', 'grey', 'w', 'w', '']
hatches = ['////', '', '','\\\\\\\\','----']
x_space = 1.0
for i, ty in enumerate(['Comp.', 'Netw.', 'DRAM', 'IDLE']):
    b=data[i]
    # print(b)
    #axes.bar(np.arange(0, 5) * 2.5 + 0, a, bottom=a_acc, width=1, edgecolor='k', color=binary(0.3 * i + 0.1), label=ty)
    axes.bar(np.arange(0, num_workloads), b, bottom=b_acc, width=0.70, edgecolor='k', color=colors[i], label=ty, hatch=hatches[i])
    b_acc += np.array(b)

axes.set_ylim(0, 1)
# axes.set_xlim(-1, 8.5)

axes.legend(bbox_to_anchor=(0.3, 1.25), handlelength=0.75, ncol=4, loc='upper center',
        labelspacing=0.2, handletextpad=0.5, columnspacing=0.5, frameon=False,
        fontsize=10)
axes.set_xticks(np.arange(0, num_workloads))
axes.set_yticks([0, 0.2, 0.4, 0.6, 0.8, 1])
axes.set_yticklabels([str(20 * i) + '%' for i in range(6)])
#axes.set_yticks([0.1 * i for i in range(12)])
#axes.set_yticklabels([str(10 * i) + '%' for i in range(12)])
axes.set_xticklabels(workloads, rotation=90)
axes.xaxis.grid(False)
axes.set_axisbelow(True)
#axes.text(0.45, 1.15, 'Exec. Time Breakdown', fontsize=14)
axes.set_ylabel('Exec. Time Brkd.')

fig.subplots_adjust(top=0.5, bottom=0.2, left=0.2, right=0.45, wspace=0.02)

plot.show()

fname = 'exec-breakdown'
fig.savefig(f'{fname}.pdf')
subprocess.check_output(f'pdfcrop {fname}.pdf', shell=True)
subprocess.check_output(f'mv {fname}-crop.pdf {fname}.pdf', shell=True)

