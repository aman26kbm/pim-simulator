import matplotlib
import matplotlib.pyplot as plot
import pandas
import numpy as np
import functools
import operator
import matplotlib.gridspec as gridspec
import subprocess, os


matplotlib.rcParams['lines.linewidth'] = 2.5

plot.style.use('bmh')
binary = matplotlib.cm.get_cmap('binary')

fig, ax = plot.subplots(1, 1)

types = {
    'Compute': [
        'RowAdd_Energy',
        'RowAdd_CRAM_RF_Energy',
        'RowCompare_Energy',
        'RowMul_Energy',
        'RowMul_CRAM_RF_Energy',
        'RowShift_Energy',
        'RowReduce_Energy',
        'RowReduceWithinTile_Energy',
        'RowBitwise_Energy',
        'RowSet_Energy',
        'RowReset_Energy',
        'Popcount_Energy',
        'Popcount_Dynamic_Energy',
    ],
    'Network': [
        'Shuffle_Dynamic_Energy', #
        'RF_Dynamic_Energy', #
        'CRAM_Dynamic_Energy', #
        'RowRead_RF_Energy', #
        'RowWrite_RF_Energy', #
        'HTree_Dynamic_Energy', #
        'RowRead_Energy', #
        'RowWrite_Energy', #
        'Block_Broadcast_Energy', #
        'Block_SendReceive_Energy', #
        'TileSend_Energy',
        'TileReceive_Energy',
        'NoC_Dynamic_Energy'
    ],
    'DRAM': [
        'RowLoad_Energy',
        'RowLoad_RF_Energy',
        'RowStore_Energy',
        'Transpose_Dynamic_Energy',
        'Dram_Dynamic_Energy',
    ],
}


e2e_layers = []
# for i in os.listdir('./output_all_kernels/'):
#     if not i.endswith('.dump'):
#         e2e_layers.append('./output_all_kernels/' + i + '/pimsim.log.energy.csv')

e2e_layers += [
        './output_all_kernels/resnet_conv1.param_conv2d_lowLatency_weightDup.log.energy.csv',
        './output_all_kernels/resnet_conv2_1_1.param_conv2d_lowLatency_weightDup.log.energy.csv',
        './output_all_kernels/resnet_conv2_1_1.param_conv2d_lowLatency_weightDup.log.energy.csv',
        './output_all_kernels/resnet_conv2_1_1.param_conv2d_lowLatency_weightDup.log.energy.csv',
        './output_all_kernels/resnet_conv2_1_1.param_conv2d_lowLatency_weightDup.log.energy.csv',
        './output_all_kernels/resnet_conv3_1_1.param_conv2d_lowLatency_weightDup.log.energy.csv',
        './output_all_kernels/resnet_conv3_1_2.param_conv2d_lowLatency_weightDup.log.energy.csv',
        './output_all_kernels/resnet_conv3_1_2.param_conv2d_lowLatency_weightDup.log.energy.csv',
        './output_all_kernels/resnet_conv3_1_2.param_conv2d_lowLatency_weightDup.log.energy.csv',
        './output_all_kernels/resnet_conv4_1_1.param_conv2d_lowLatency_weightDup.log.energy.csv',
        './output_all_kernels/resnet_conv4_1_2.param_conv2d_lowLatency_inputDup.log.energy.csv',
        './output_all_kernels/resnet_conv4_1_2.param_conv2d_lowLatency_inputDup.log.energy.csv',
        './output_all_kernels/resnet_conv4_1_2.param_conv2d_lowLatency_inputDup.log.energy.csv',
        './output_all_kernels/resnet_conv5_1_1.param_conv2d_lowLatency_inputDup.log.energy.csv',
        './output_all_kernels/resnet_conv5_1_2.param_conv2d_lowLatency_inputDup.log.energy.csv',
        './output_all_kernels/resnet_conv5_1_2.param_conv2d_lowLatency_inputDup.log.energy.csv',
        './output_all_kernels/resnet_conv5_1_2.param_conv2d_lowLatency_inputDup.log.energy.csv'
]

def washdata(raws):
    count = {}
    for ky in types.keys():
        count[ky] = 0.0
    total_exec = 0
    for fname in raws:
        parsed_csv = pandas.read_csv(fname)
        for ky in types.keys():
            for j in types[ky]:
                try:
                    count[ky] += float(parsed_csv[j][0])
                except:
                    print(fname, j)
    total = 0
    for ky in types.keys():
        total += count[ky]
    for ky in types.keys():
        count[ky] /= total
    return count


data = []
#vecadd
data.append(washdata(['./output_all_kernels/vecadd_handcoded.DMesh12x10_TileSize256_CramSize256x256.log.energy.csv']))
#fir
data.append(washdata(['./output_all_kernels/fir_handcoded.DMesh12x10_TileSize256_CramSize256x256.log.energy.csv']))
#gemv
data.append(washdata(['./output_all_kernels/gemv_handcoded.DMesh12x10_TileSize256_CramSize256x256.log.energy.csv']))
#gemm
data.append(washdata(['./output_all_kernels/gemm_handcoded.DMesh12x10_TileSize256_CramSize256x256.log.energy.csv']))
#conv
data.append(washdata(['./output_all_kernels/conv2d_handcoded.DMesh12x10_TileSize256_CramSize256x256.log.energy.csv']))
#e2e
data.append(washdata(e2e_layers))


acc = np.zeros((6, ))
colors = ['w', 'grey', 'w', 'w', 'w']
hatches = ['', '', '////', '----', '++++']
for i, ky in enumerate(types.keys()):
    y = []
    for j in data:
        y.append(j[ky])
    y = np.array(y)
    ax.bar(np.arange(6), y, bottom=acc, label=ky, color=colors[i], hatch=hatches[i], edgecolor='k', width=0.70)
    acc += y

ax.legend(bbox_to_anchor=(0.3, 1.25), handlelength=0.75, ncol=3, loc='upper center',
          labelspacing=0.2, handletextpad=0.5, columnspacing=0.5, frameon=False,
          fontsize=10)
ax.set_xticks(np.arange(6))
ax.set_xticklabels(['vecadd', 'fir', 'gemv', 'gemm', 'conv2d', 'resnet18'], rotation=90)
ax.xaxis.grid(False)
ax.set_axisbelow(True)
ax.set_ylim(0, 1)
ax.set_yticks([0.2*i for i in range(6)])
ax.set_yticklabels([str(20 * i) + '%' for i in range(6)])
#ax.text(0.45, 1.15, 'Energy Breakdown', fontsize=14)
ax.set_ylabel('Energy Breakdown')

#fig.subplots_adjust(top=0.5, bottom=0.2, left=0.2, right=0.6, wspace=0.02)
fig.subplots_adjust(top=0.5, bottom=0.2, left=0.2, right=0.45, wspace=0.02)

plot.show()

fname = 'energy-breakdown'
fig.savefig(f'{fname}.pdf')
subprocess.check_output(f'pdfcrop {fname}.pdf', shell=True)
subprocess.check_output(f'mv {fname}-crop.pdf {fname}.pdf', shell=True)

