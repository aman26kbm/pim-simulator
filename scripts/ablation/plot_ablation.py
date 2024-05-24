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

def get_wkld_from_fname(fname):
    if('conv2d' in fname): return 0
    elif('fir' in fname): return 1
    elif('gemm' in fname): return 2
    elif('gemv' in fname): return 3
    elif('vecadd' in fname): return 4
    else: return 'unknown'

def get_hdw_feature_from_fname(fname):
    if('shuffleOff' in fname): return 0
    elif('constOpOff' in fname): return 1
    elif('bus' in fname): return 2
    elif('crossCramShiftOff' in fname): return 3
    elif('oneToAllBroadcast' in fname): return 4
    elif('everythingOff' in fname): return 5
    elif('IntraCRAM Reduction' in fname): return 6
    else: return 'unknown'

print('Number of arguments:', len(sys.argv), 'arguments.')
print('Argument List:', str(sys.argv))

input_file_dir = str(sys.argv[1])

matplotlib.rcParams['lines.linewidth'] = 2.5

plot.style.use('bmh')
binary = matplotlib.cm.get_cmap('binary')

fig, axes = plot.subplots(1,7,figsize=(18,4.5))
data = np.zeros([8,5])
with open(os.path.join(os.getcwd(), input_file_dir, 'chart_breakdown_avg.csv'), 'r') as f:
    csv_reader = csv.reader(f)
    for line in csv_reader:
        if(line[0]=='workload_name'): continue
        workload_name = line[0]
        hdw_feature = get_hdw_feature_from_fname(workload_name)
        wkld=get_wkld_from_fname(workload_name)
        if(hdw_feature=='unknown' or wkld=='unknown'): continue
        data[hdw_feature][wkld]=float(line[5])
    

    # All	
    # minus Htree, plus Bus	
    # minus constant ops	
    # minus cross cram shift	
    # minus systolic broadcast, plus one-to-all broadcast	
    # minus shuffle units	
    # minus Htree, constant ops, cross cram shift, sys-bcast, shuffle
    # IntraCRAM reduction

    data = np.array([[20713,	58112,	20713,	20713,	105211,	58138,	156003,48828],\
                    [27336,	28464,	48924,	681138,	27290,	27336,	724868,27336],\
                    [146310,	471659,	146310,	146310,	148416,	210767,	535777,396395],\
                    [117171,	137319,	129064,	117171,	117171,	117171,	144257,117171],\
                    [30944,	30944,	30944,	30944,	30944,	30944,	30944,30944]])
    print(data)
    data = data.transpose()
    # (a) shuffle disabled
    # (b) Const Ops disabled
    # (c) H-Tree disabled
    # (d) Cross-CRAM Shift disabled
    # (e) Systolic Bcast disabled
    # (f) All disabled
    # (g) IntraCRAM Reduction
    data = np.array([data[5], data[2], data[1], data[3], data[4], data[6],data[7],data[0]])
    print(data)

    to_gm = []
    to_label = []
    colors = ['0.2', '0.4', '0.6', '0.8', '1.0']
    hatches = ['', '', '////','\\\\\\\\','----']
    

    for i, ty in enumerate(['(a) shuffle disabled', '(b) Const Ops disabled', '(c) H-Tree disabled', '(d) Cross-CRAM \n Shift disabled', '(e) Systolic Bcast \n disabled', '(f) All disabled','(g) IntraCRAM Reduction']):
        b=data[7]/data[i]
        print(b)
        axs = axes[i]
        #axes.bar(np.arange(0, 5) * 2.5 + 0, a, bottom=a_acc, width=1, edgecolor='k', color=binary(0.3 * i + 0.1), label=ty)
        axs.bar(np.arange(0, 5) * 1.9, b, bottom=0, width=1, edgecolor='k', color=colors, hatch=hatches[0])
        axs.set_ylim(0, 1)
        axs.set_xlim(-1, 8.5)

        # axs.legend(bbox_to_anchor=(0.3, 1.25), handlelength=0.75, ncol=4, loc='upper center',
        #         labelspacing=0.2, handletextpad=0.5, columnspacing=0.5, frameon=False,
        #         fontsize=10)
        axs.set_xticks(np.arange(0, 5) * 1.9)
        axs.set_yticks([0, 0.2, 0.4, 0.6, 0.8, 1])
        axs.set_yticklabels([str(20 * i) + '%' for i in range(6)],fontsize=12)
        if(i!=0): axs.tick_params(left = False, right = False , labelleft = False , labelright = False)
        #axes.set_yticks([0.1 * i for i in range(12)])
        #axes.set_yticklabels([str(10 * i) + '%' for i in range(12)])
        axs.set_xticklabels(['conv2d', 'fir', 'gemm', 'gemv', 'vadd'], rotation=90,fontsize=15)
        axs.xaxis.grid(False)
        axs.set_axisbelow(True)
        axs.set_title(ty, fontsize=13, wrap=True)
        
        #axes.text(0.45, 1.15, 'Exec. Time Breakdown', fontsize=14)
        # axs.set_ylabel('Exec. Time Brkd.')

        # fig.subplots_adjust(top=0.5, bottom=0.2, left=0.2, right=0.45, wspace=0.02)
    
    fig.subplots_adjust(bottom=0.2)
    plot.show()
    fname = 'hdw-feature-ablation'
    fig.savefig(f'{fname}.pdf')
    subprocess.check_output(f'pdf-crop-margins -v -p 0 -a -6 {fname}.pdf', shell=True)
    subprocess.check_output(f'mv {fname}_cropped.pdf {fname}.pdf', shell=True)