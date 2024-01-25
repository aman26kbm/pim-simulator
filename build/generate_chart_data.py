import os, glob
import sys
import re

print('Number of arguments:', len(sys.argv), 'arguments.')
print('Argument List:', str(sys.argv))

input_file_dir = str(sys.argv[1])

import csv

header = ['workload_name', 'dram_cycle', 'network_cycle', 'comput_cycle', 'idle_cycle', 'total_cycle']
data = ['', 0, 0, 0, 0, 0]


f_stats = open(os.path.join(input_file_dir,'03_chart_breakdown_avg.csv'), 'w', encoding='UTF8', newline='')
writer = csv.writer(f_stats)
# write the header
writer.writerow(header)

assert os.path.isfile(os.path.join(os.getcwd(), input_file_dir, '02_breakdown_avg.csv')), "breakdown_avg.csv does not exist. Run collect_stats.py first."
    
with open(os.path.join(os.getcwd(), input_file_dir, '02_breakdown_avg.csv'), 'r') as f:
    chart_data_list = []
    csv_reader = csv.reader(f)
    for row in csv_reader:
        #print(f'file {filename}:')
        workload_name = row[0]
        req_name = row[2]
        req_num = float(row[3])
        req_cycle = float(row[4])
        data_col = 0
        if(req_name == 'RowLoad' or req_name=="RowLoad_RF" or req_name == 'RowStore'):
            data_col = 1 #dram_cycle
        elif (req_name == 'TileSend' or req_name == 'TileReceive'):
            data_col = 2 #network_cycle
        elif (req_name == 'IDLE'):
            data_col = 4 #idle_cycle
        else:
            data_col = 3 #compute_cycle

        present = False
        for chart_data in chart_data_list:
            if workload_name == chart_data[0]:
                present = True
                chart_data[data_col] += req_cycle
        if(not present):
            new_chart_data = [workload_name, 0,0,0,0,0]
            new_chart_data[data_col] += req_cycle
            chart_data_list.append(new_chart_data)     
    #post process, averaging _constOpOff with _constOpOff_kickoutRow
    #averaging _crossCramShiftOff with _crossCramShiftOff_bypassDram
    for chart_data in chart_data_list:
        if "_kickoutRow" in chart_data[0]:
            filename = chart_data[0].replace('_kickoutRow','')
            for another in chart_data_list:
                if(another[0]==filename):
                    another[1] = (another[1]+chart_data[1])/2
                    another[2] = (another[2]+chart_data[2])/2
                    another[3] = (another[3]+chart_data[3])/2
                    another[4] = (another[4]+chart_data[4])/2
        if "_bypassDram" in chart_data[0]:
            filename = chart_data[0].replace('_bypassDram','')
            for another in chart_data_list:
                if(another[0]==filename):
                    another[1] = (another[1]+chart_data[1])/2
                    another[2] = (another[2]+chart_data[2])/2
                    another[3] = (another[3]+chart_data[3])/2
                    another[4] = (another[4]+chart_data[4])/2
    #eliminate "_kickoutRow" lines and "_bypassDram" lines
    chart_data_list = [x for x in chart_data_list if not ("_kickoutRow" in x[0] or "_bypassDram" in x[0])]
    for x in chart_data_list:
        x[5] = round(x[1]+x[2]+x[3]+x[4])
    for chart_data in chart_data_list:
        writer.writerow(chart_data)




