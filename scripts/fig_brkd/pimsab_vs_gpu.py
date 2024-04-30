import csv
import copy
import matplotlib.pyplot as plot
import numpy as np
import subprocess, os

def get_time(workload_name, csv_reader):
    scaling_factor = 1/1.5
    if workload_name == "resnet":
        sum=0
        for line in csv_reader:
            if workload_name in line[0]:
                if "conv1" in line[0]:
                    sum+=float(line[1])
                elif "conv2_1_1" in line[0]:
                    sum+=float(line[1]) * 4
                elif "conv3_1_1" in line[0]:
                    sum+=float(line[1])
                elif "conv3_1_2" in line[0]:
                    sum+=float(line[1]) * 3
                elif "conv4_1_1" in line[0]:
                    sum+=float(line[1])
                elif "conv4_1_2" in line[0]:
                    sum+=float(line[1]) * 3
                elif "conv5_1_1" in line[0]:
                    sum+=float(line[1])
                elif "conv5_1_2" in line[0]:
                    sum+=float(line[1]) * 3
        return float(sum)*scaling_factor
    elif workload_name == "bert":
        return 1.0
    elif workload_name == "mlp":
        return 1.0
    else:
        for line in csv_reader:
            if workload_name in line[0]:
                return float(line[1])*scaling_factor

def get_energy(workload_name, csv_reader):
    scaling_factor = 1
    if workload_name == "resnet":
        sum=0
        for line in csv_reader:
            if workload_name in line[0]:
                if "conv1" in line[0]:
                    sum+=float(line[2])
                elif "conv2_1_1" in line[0]:
                    sum+=float(line[2]) * 4
                elif "conv3_1_1" in line[0]:
                    sum+=float(line[2])
                elif "conv3_1_2" in line[0]:
                    sum+=float(line[2]) * 3
                elif "conv4_1_1" in line[0]:
                    sum+=float(line[2])
                elif "conv4_1_2" in line[0]:
                    sum+=float(line[2]) * 3
                elif "conv5_1_1" in line[0]:
                    sum+=float(line[2])
                elif "conv5_1_2" in line[0]:
                    sum+=float(line[2]) * 3
        return float(sum) * scaling_factor
    elif workload_name == "bert":
        return 1.0
    elif workload_name == "mlp":
        return 1.0
    else:
        for line in csv_reader:
            print(line)
            if workload_name in line[0]:
                return float(line[2]) * scaling_factor
data  = []

workload_list = ['vecadd', 'fir', 'gemv', 'gemm', 'conv2d', 'resnet', 'bert', 'mlp']
with open('output_all_kernels/00_stats.csv', 'r') as f:
    csv_reader = csv.reader(f)
    lines = list(csv_reader)
    
    print(get_energy('vecadd', lines))
    for workload in workload_list:
        data.append([workload, get_time(workload, lines), get_energy(workload, lines)])
    print(data)

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