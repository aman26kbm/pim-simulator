import matplotlib
import matplotlib.pyplot as plot
import pandas
import numpy as np
import functools
import operator
import matplotlib.gridspec as gridspec
import subprocess

raw = pandas.read_csv('./sensitivity_hw_param.csv')

matplotlib.rcParams['lines.linewidth'] = 2.5
matplotlib.rcParams['text.usetex'] = True
matplotlib.rcParams['text.latex.preamble'] = '\\linespread{0.8}'

plot.style.use('bmh')
binary = matplotlib.cm.get_cmap('binary')

knob = 4
space = 1
height = 4


fig = plot.figure(figsize=(6.5, 7))
gs = gridspec.GridSpec(3*knob+3*space+height+2, 1)

axes = []

axes.append(fig.add_subplot(gs[0:4,0]))
axes.append(fig.add_subplot(gs[5:9,0]))
axes.append(fig.add_subplot(gs[10:14,0]))

def plot_bars(ax, data, labels, idx, title, bbox, ylims, color):
    res = []
    base = np.array(data[1])
    n = len(data)
    xs = np.arange(len(data[0])) * (n + 0.5)
    cnt = 0
    for j, abs_y in enumerate(data[0:]):
        y = base / np.array(abs_y)
        res.append(y[-1])
        ax.bar(xs + j, y, label=labels[j], edgecolor='k', color=color[j], width=1)

    ax.xaxis.grid(False)
    ax.set_axisbelow(True)
    ax.set_xticks(xs + n // 2)
    ax.set_xticklabels(['vecadd', 'fir', 'gemv', 'gemm', 'conv2d'], rotation=45, fontsize=12)
    ax.set_yticks([i for i in range(ylims[0], int(ylims[1]) + 1)])
    ax.set_yticklabels(['%d.0' % i for i in range(ylims[0], int(ylims[1]) + 1)])
    ax.set_xlim(-0.75, 16.75)
    ax.legend(handlelength=1, frameon=False, ncol=1, handletextpad=0.5, labelspacing=0.2, loc='right', bbox_to_anchor=bbox, fontsize=9)
    ax.set_title(title, fontsize=13)
    if idx == 0 or idx == 1:
        ax.tick_params('x', labelbottom=False)
    return res

areas = {
    '12x10; 64; 512x512': 2905703685,
    '12x10; 256; 256x256': 2991942779,
    '12x10; 1024; 128x128': 3163962894,
    '12x20; 128; 256x256': 3169943274,
    '12x5; 512; 256x256': 2903858675,
    '6x20; 256; 256x256': 2736428866,
    '24x5; 256; 256x256': 3502970603,
}

gms = []

bars = [raw['12x10; 64; 512x512'], raw['12x10; 256; 256x256'],  raw['12x10; 1024; 128x128']]
labels = ['\\noindent mesh: 12x10\\\\\\textbf{64 crams/tile\\\\(512bl x 512wl)/cram}',
          '\\noindent mesh: 12x10\\\\\\textbf{256 crams/tile\\\\(256bl x 256wl)/cram}',
          '\\noindent mesh: 12x10\\\\\\textbf{1024 crams/tile\\\\(128bl x 128wl)/cram}']
gms += plot_bars(axes[0], bars, labels, 0, '(a) Comp. Intensity', (1.58, 0.5), (0, 1.2), ['w', binary(0.45), binary(0.7)])

bars = [raw['12x5; 512; 256x256'],raw['12x10; 256; 256x256'],  raw['12x20; 128; 256x256']]
labels = ['\\noindent \\textbf{mesh: 12x5\\\\512 crams/tile}\\\\(256bl x 256wl)/cram',
          '\\noindent \\textbf{mesh: 12x10\\\\256 crams/tile}\\\\(256bl x 256wl)/cram',
          '\\noindent \\textbf{mesh: 12x20\\\\128 crams/tile}\\\\(256bl x 256wl)/cram']
gms += plot_bars(axes[1], bars, labels, 1, '(b) \\#Tile v.s. \\#Array', (1.55, 0.5), (0, 1.2), ['w', binary(0.45), binary(0.7)])[1:]

bars = [raw['6x20; 256; 256x256'], raw['12x10; 256; 256x256'],  raw['24x5; 256; 256x256']]
labels = ['\\noindent \\textbf{mesh: 6x20}\\\\256 crams/tile\\\\(256bl x 256wl)/cram',
          '\\noindent \\textbf{mesh: 12x10}\\\\256 crams/tile\\\\(256bl x 256wl)/cram',
          '\\noindent \\textbf{mesh: 24x5}\\\\256 crams/tile\\\\(256bl x 256wl)/cram']
gms += plot_bars(axes[2], bars, labels, 2, '(c) DRAM B/w', (1.55, 0.5), (0, 2.2), ['w', binary(0.45), binary(0.7)])[1:]

parea = [areas['12x10; 256; 256x256'], areas['12x10; 64; 512x512'], areas['12x10; 1024; 128x128'],
         areas['12x5; 512; 256x256'], areas['12x20; 128; 256x256'],
         areas['6x20; 256; 256x256'], areas['24x5; 256; 256x256']]

print(gms)
print(parea)

axes[0].set_ylabel('Normalized Perf.')
axes[1].set_ylabel('Normalized Perf.')
axes[2].set_ylabel('Normalized Perf.')


fig.subplots_adjust(top=0.93, bottom=0.08, left=0.1, right=0.53, wspace=0.02, hspace=1)
plot.show()

fname = 'sensitivity_hw_param'
fig.savefig(f'{fname}.pdf')
subprocess.check_output(f'pdfcrop {fname}.pdf', shell=True)
subprocess.check_output(f'mv {fname}-crop.pdf {fname}.pdf', shell=True)

