import os, glob
import sys
import re

print('Number of arguments:', len(sys.argv), 'arguments.')
print('Argument List:', str(sys.argv))

input_file_dir = str(sys.argv[1])

import csv

header = ['workload_name', 'All', 'minus Htree, plus Bus', 'minus constant ops', 'minus cross cram shift', 'minus systolic broadcast, plus one-to-all broadcast', 'minus shuffle units', 'minus Htree, constant ops, cross cram shift, sys-bcast, shuffle']
data = ['', 0, 0, 0, 0, 0, 0, 0]

f_fig = open(os.path.join(input_file_dir,'05_upload_ablation_energy_figure.csv'), 'w', encoding='UTF8', newline='')
writer = csv.writer(f_fig)
# write the header
writer.writerow(header)

assert os.path.isfile(os.path.join(os.getcwd(), input_file_dir, '00_stats.csv')), "00_stats.csv does not exist. Run collect_stats.py and generate_chart_data.py first."

with open(os.path.join(os.getcwd(), input_file_dir, '00_stats.csv'), 'r') as f:
    fig_data_list = [['conv', 0, 0, 0, 0, 0, 0, 0],
                     ['fir', 0, 0, 0, 0, 0, 0, 0],
                     ['gemm', 0, 0, 0, 0, 0, 0, 0],
                     ['gemv', 0, 0, 0, 0, 0, 0, 0],
                     ['vadd', 0, 0, 0, 0, 0, 0, 0]]
    csv_reader = csv.reader(f)
    for row in csv_reader:
        workload_name = row[0]
        total_dynamic_energy = row[2]
        col=0
        if('bus' in workload_name): col = 2
        elif('constOpOff' in workload_name): col=3
        elif('crossCramShiftOff' in workload_name): col=4
        elif('oneToAllBroadcast' in workload_name): col=5
        elif('shuffleOff' in workload_name): col=6
        elif('everythingOff' in workload_name): col=7
        else: col=1
        row=0
        if('conv2d' in workload_name): row=0
        elif('fir' in workload_name): row=1
        elif('gemm' in workload_name): row=2
        elif('gemv' in workload_name): row=3
        elif('vecadd' in workload_name): row=4
        else: continue
        if fig_data_list[row][col]==0:
            fig_data_list[row][col]=total_dynamic_energy
        else:
            fig_data_list[row][col]=((float)(fig_data_list[row][col])+(float)(total_dynamic_energy))/2
    for row in fig_data_list:
        writer.writerow(row)
