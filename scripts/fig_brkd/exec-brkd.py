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

input_file_dir = str(sys.argv[1])

matplotlib.rcParams['lines.linewidth'] = 2.5

plot.style.use('bmh')
binary = matplotlib.cm.get_cmap('binary')

fig, axes = plot.subplots(1, 1)

data = np.zeros([7,4])
with open(os.path.join(os.getcwd(), input_file_dir, '03_chart_breakdown_avg.csv'), 'r') as f:
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
            print(line)
            row=0
            # ['vecadd', 'fir', 'gemv', 'gemm', 'conv2d', 'resnet18']
            if('conv2d' in workload_name): row=4
            elif('fir' in workload_name): row=1
            elif('gemm' in workload_name): row=3
            elif('gemv' in workload_name): row=2
            elif('vecadd' in workload_name): row=0
            elif('resnet' in workload_name): row=6
            else: row=5
            data[row]=[float(line[3])/float(line[5]),float(line[2])/float(line[5]),float(line[1])/float(line[5]),float(line[4])/float(line[5])]
            print(data[row])
    print(data)
    data = data.transpose()
    

    b_acc = np.zeros((6, ))
    to_gm = []
    to_label = []
    colors = ['w', 'grey', 'w', 'w', '']
    hatches = ['', '', '////','\\\\\\\\','----']
    for i, ty in enumerate(['Comp.', 'Netw.', 'DRAM', 'IDLE']):
        b=data[i]
        print(b)
        #axes.bar(np.arange(0, 5) * 2.5 + 0, a, bottom=a_acc, width=1, edgecolor='k', color=binary(0.3 * i + 0.1), label=ty)
        axes.bar(np.arange(0, 6) * 1.5, b, bottom=b_acc, width=1, edgecolor='k', color=colors[i], label=ty, hatch=hatches[i])
        b_acc += np.array(b)

    axes.set_ylim(0, 1)
    axes.set_xlim(-1, 8.5)

    axes.legend(bbox_to_anchor=(0.3, 1.25), handlelength=0.75, ncol=4, loc='upper center',
            labelspacing=0.2, handletextpad=0.5, columnspacing=0.5, frameon=False,
            fontsize=10)
    axes.set_xticks(np.arange(0, 6) * 1.5)
    axes.set_yticks([0, 0.2, 0.4, 0.6, 0.8, 1])
    axes.set_yticklabels([str(20 * i) + '%' for i in range(6)])
    #axes.set_yticks([0.1 * i for i in range(12)])
    #axes.set_yticklabels([str(10 * i) + '%' for i in range(12)])
    axes.set_xticklabels(['vecadd', 'fir', 'gemv', 'gemm', 'conv2d', 'resnet18'], rotation=90)
    axes.xaxis.grid(False)
    axes.set_axisbelow(True)
    #axes.text(0.45, 1.15, 'Exec. Time Breakdown', fontsize=14)
    axes.set_ylabel('Exec. Time Brkd.')

    fig.subplots_adjust(top=0.5, bottom=0.2, left=0.2, right=0.45, wspace=0.02)

    plot.show()

    fname = 'exec-breakdown'
    fig.savefig(f'{fname}.pdf')
    # subprocess.check_output(f'pdfcrop {fname}.pdf', shell=True)
    # subprocess.check_output(f'mv {fname}-crop.pdf {fname}.pdf', shell=True)

