import matplotlib
import matplotlib.pyplot as plot
import pandas
import numpy as np
import functools
import operator
import matplotlib.gridspec as gridspec
import subprocess

raw = pandas.read_csv('scripts/fig_brkd/sensitivity_size_precision.csv')
# raw = pandas.read_csv('./sensitivity_size_precision.csv')

matplotlib.rcParams['lines.linewidth'] = 2.5
matplotlib.rcParams['text.usetex'] = True
matplotlib.rcParams['text.latex.preamble'] = '\\linespread{0.8}'

plot.style.use('bmh')
binary = matplotlib.cm.get_cmap('binary')

knob = 4
space = 1
height = 4


fig = plot.figure(figsize=(12,5))
gs = gridspec.GridSpec(2, 11)

axes = []

axes.append(fig.add_subplot(gs[0:1,0:4]))
axes.append(fig.add_subplot(gs[0:1,6:11]))

def plot_bars(ax, data, labels, idx, title, bbox, ylims, color):
    res = []
    if idx == 0:
        base = np.array(data[1])
    elif idx == 1:
        base = np.array(data[0])
    n = len(data)
    xs = np.arange(len(data[0])) * (n + 0.5)
    cnt = 0
    for j, abs_y in enumerate(data[0:]):
        if idx == 0:
            y = 1/ (base / np.array(abs_y))
        if idx == 1:
            y = 1/ (base / np.array(abs_y))
        res.append(y[-1])
        ax.bar(xs + j, y, label=labels[j], edgecolor='k', color=color[j], width=1)

    ax.xaxis.grid(False)
    ax.set_axisbelow(True)
    ax.set_xticks(xs + n // 2)
    ax.set_xticklabels(['vecadd', 'fir', 'gemv', 'gemm', 'conv2d'], rotation=90, fontsize=12)
    ax.set_yticks([i for i in np.arange(ylims[0], ylims[1]+0.5,0.5)])
    ax.set_yticklabels(['%.1f' % i for i in np.arange(ylims[0], ylims[1]+0.5,0.5)])
    if idx == 0:
        ax.legend(handlelength=1, frameon=False, ncol=3, handletextpad=0.5, labelspacing=0.2, loc='upper center', bbox_to_anchor=bbox, fontsize=9)
    if idx == 1: 
        ax.legend(handlelength=1, frameon=False, ncol=5, columnspacing=0.5, handletextpad=0.2, labelspacing=0.2, loc='upper center',bbox_to_anchor=bbox, fontsize=9)
    ax.set_title(title, fontsize=13)

    return res

gms = []

bars = [raw['half'],raw['base'],  raw['double']]
labels = ['half',
          'main',
          'double']
gms += plot_bars(axes[0], bars, labels, 0, '(a) Data Size Sens.', (0.5, 1.05), (0, 2.5), ['w', binary(0.45), binary(0.7)])


bars = [raw['int4'],  raw['int5'],  raw['int6'],  raw['int7'],  raw['int8']]
labels = ['i4',
          'i5',
          'i6',
          'i7',
          'i8']
gms += plot_bars(axes[1], bars, labels, 1, '(b) Precision Sens.', (0.5, 1.05), (0, 3), ['w', binary(0.45), binary(0.55), binary(0.65), binary(0.75)])



print(gms)

axes[0].set_ylabel('Relative Exec. Time \n Normalized to Main')
axes[1].set_ylabel('Relative Exec. Time \n Normalized to i4')


fig.subplots_adjust(top=0.93, bottom=0.08, left=0.1, right=0.53, wspace=0.02, hspace=1)
plot.show()

fname = 'sensitivity_size_precision'
fig.savefig(f'{fname}.pdf')
subprocess.check_output(f'pdfcrop {fname}.pdf', shell=True)
subprocess.check_output(f'mv {fname}-crop.pdf {fname}.pdf', shell=True)

