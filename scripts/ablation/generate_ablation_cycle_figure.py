import os, glob
import sys
import re

print('Number of arguments:', len(sys.argv), 'arguments.')
print('Argument List:', str(sys.argv))

import csv
import exec_brkd

header = ['workload_name', 'All', 'minus Htree, plus Bus', 'minus constant ops', 'minus cross cram shift', 'minus systolic broadcast, plus one-to-all broadcast', 'minus shuffle units', 'minus Htree, constant ops, cross cram shift, sys-bcast, shuffle']
data = ['', 0, 0, 0, 0, 0, 0, 0]

f_fig = open(os.path.join('04_upload_ablation_cycle_figure.csv'), 'w', encoding='UTF8', newline='')
writer = csv.writer(f_fig)
# write the header
writer.writerow(header)


fig_data_list = [['conv', 0, 0, 0, 0, 0, 0, 0, 0],
                    ['fir', 0, 0, 0, 0, 0, 0, 0, 0],
                    ['gemm', 0, 0, 0, 0, 0, 0, 0, 0],
                    ['gemv', 0, 0, 0, 0, 0, 0, 0, 0],
                    ['vadd', 0, 0, 0, 0, 0, 0, 0, 0],
                    ['resnet18', 0, 0, 0, 0, 0, 0, 0, 0],
                    ['bert', 0, 0, 0, 0, 0, 0, 0, 0],
                    ['mlp_charm', 0, 0, 0, 0, 0, 0, 0, 0]
]
assert os.path.isfile(os.path.join(os.getcwd(),'output_all_kernels', '03_chart_breakdown_avg.csv')), "chart_breakdown_avg.csv does not exist. Run collect_stats.py and generate_chart_data.py first."
all_kernels_input_file_dir = os.path.join(os.getcwd(),'output_all_kernels', '03_chart_breakdown_avg.csv')
with open(os.path.join(os.getcwd(), all_kernels_input_file_dir, '03_chart_breakdown_avg.csv'), 'r') as f:

    csv_reader = csv.reader(f)
    for row in csv_reader:
        workload_name = row[0]
        total_cycle = row[5]
        col=0
        if('bus' in workload_name): col = 2
        elif('constOpOff' in workload_name): col=3
        elif('crossCramShiftOff' in workload_name): col=4
        elif('oneToAllBroadcast' in workload_name): col=5
        elif('shuffleOff' in workload_name): col=6
        elif('everythingOff' in workload_name): col=7
        elif('ring' in workload_name): col=8
        else: col=1
        row=0
        if('conv2d' in workload_name): row=0
        elif('fir' in workload_name): row=1
        elif('gemm' in workload_name): row=2
        elif('gemv' in workload_name): row=3
        elif('vecadd' in workload_name): row=4
        else: continue
        fig_data_list[row][col]=total_cycle

assert os.path.isfile(os.path.join(os.getcwd(),'output_resnet', '03_chart_breakdown_avg.csv')), "chart_breakdown_avg.csv does not exist. Run collect_stats.py and generate_chart_data.py first."
resnet_input_file_dir = os.path.join(os.getcwd(),'output_resnet', '03_chart_breakdown_avg.csv')
with open(os.path.join(os.getcwd(), resnet_input_file_dir, '03_chart_breakdown_avg.csv'), 'r') as f:

    csv_reader = csv.reader(f)
    for row in csv_reader:
        workload_name = row[0]
        total_cycle = row[5]
        col=0
        if('bus' in workload_name): col = 2
        elif('constOpOff' in workload_name): col=3
        elif('crossCramShiftOff' in workload_name): col=4
        elif('oneToAllBroadcast' in workload_name): col=5
        elif('shuffleOff' in workload_name): col=6
        elif('everythingOff' in workload_name): col=7
        elif('ring' in workload_name): col=8
        else: col=1
        row=5
        fig_data_list[row][col]=exec_brkd.sum_resnet()

bert_input_file_dir = os.path.join(os.getcwd(),'output_bert', '03_chart_breakdown_avg.csv')
with open(os.path.join(os.getcwd(), bert_input_file_dir, '03_chart_breakdown_avg.csv'), 'r') as f:

    csv_reader = csv.reader(f)
    for row in csv_reader:
        workload_name = row[0]
        total_cycle = row[5]
        col=0
        if('bus' in workload_name): col = 2
        elif('constOpOff' in workload_name): col=3
        elif('crossCramShiftOff' in workload_name): col=4
        elif('oneToAllBroadcast' in workload_name): col=5
        elif('shuffleOff' in workload_name): col=6
        elif('everythingOff' in workload_name): col=7
        elif('ring' in workload_name): col=8
        else: col=1
        row=6
        fig_data_list[row][col]=exec_brkd.sum_bert()

mlp_input_file_dir = os.path.join(os.getcwd(),'output_mlp_2', '03_chart_breakdown_avg.csv')
with open(os.path.join(os.getcwd(), mlp_input_file_dir, '03_chart_breakdown_avg.csv'), 'r') as f:

    csv_reader = csv.reader(f)
    for row in csv_reader:
        workload_name = row[0]
        total_cycle = row[5]
        col=0
        if('bus' in workload_name): col = 2
        elif('constOpOff' in workload_name): col=3
        elif('crossCramShiftOff' in workload_name): col=4
        elif('oneToAllBroadcast' in workload_name): col=5
        elif('shuffleOff' in workload_name): col=6
        elif('everythingOff' in workload_name): col=7
        elif('ring' in workload_name): col=8
        else: col=1
        row=7
        fig_data_list[row][col]=exec_brkd.sum_resnet()
for row in fig_data_list:
    writer.writerow(row)
