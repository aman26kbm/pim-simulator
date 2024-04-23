import os, glob
import sys
import re

print('Number of arguments:', len(sys.argv), 'arguments.')
print('Argument List:', str(sys.argv))

path = str(sys.argv[1])

import csv

header = ['name', 'cycle', 'dynamic_energy']
data = ['', 0, 0]


f_stats = open(os.path.join(path,'00_stats.csv'), 'w', encoding='UTF8', newline='')
writer = csv.writer(f_stats)
f_breakdown = open(os.path.join(path,'01_breakdown.csv'), 'w', encoding='UTF8', newline='')
breakdown_writer = csv.writer(f_breakdown)
# write the header
writer.writerow(header)
breakdown_writer.writerow(['workload','Tile','ReqName','Num', 'cycle'])
f_breakdown_avg = open(os.path.join(path,'02_breakdown_avg.csv'), 'w', encoding='UTF8', newline='')
breakdown_avg_writer = csv.writer(f_breakdown_avg)
for filename in sorted(glob.glob(os.path.join(path, '*.log'))):
    #cycles
    cycle=0
    with open(os.path.join(os.getcwd(), filename), 'r') as f:
        for line in f:
            if "clocks" in line:
                words = line.split()
                cycle = int(words[4])
                break
    print(filename+": total cycles: "+str(cycle))
    #energy
    energy = 0
    with open(os.path.join(os.getcwd(), filename+'.energy.csv'), 'r') as f:
        csv_reader = csv.DictReader(f)
        for row in csv_reader:
            #print(f'file {filename}:')
            energy = row["Total_Dynamic_Energy"]
            print(f'\t{row["WorkloadName"]} has total dynamic energy = {row["Total_Dynamic_Energy"]}, and total static energy = {row["Total_Static_Energy"]}.')
    data = [filename, cycle, energy]
    # write the data
    writer.writerow(data)

    #breakdown
    # header = ['Tile','ReqName', 'cycle']
    

    tile_count = 0
    data_list = []
    idle_cycle = 0
    with open(os.path.join(os.getcwd(), filename+'.reqs_stats.csv'), 'r') as f:
        csv_reader = csv.DictReader(f)
        for row in csv_reader:
            tile = row["Tile"]
            reqName = row["ReqName"]
            if int(row['IDLE'])==1: cycle = int(row["timeTaken"])+int(row['IDLE'])
            else: cycle = int(row["timeTaken"])-int(row['IDLE'])+1
            idle_cycle += int(row['IDLE'])-1
            # print(f'\t{row["WorkloadName"]} has total dynamic energy = {row["Total_Dynamic_Energy"]}, and total static energy = {row["Total_Static_Energy"]}.')
            present = False
            match_tile = False
            for data in data_list:
                if data[0]==filename and data[1]==tile and data[2]==reqName:
                    data[3]+=1
                    data[4]+=int(cycle)
                    present = True
                if data[0]==filename and data[1]==tile:
                    match_tile = True
            if not present:
                new_data = [filename, tile, reqName, 1, int(cycle)]
                data_list.append(new_data)
            if not match_tile:
                tile_count+=1
    print("tile count of "+ filename +" ="+ str(tile_count))
    
    for data in data_list:
        # write the data
        breakdown_writer.writerow(data)

    #average of all tiles involved
    avg_data_list = []
    
    for data in data_list:
        tile = data[1]
        reqName = data[2]
        num = data[3]
        cycle = data[4]
        present = False
        for avg_data in avg_data_list:
            if avg_data[0]==filename and avg_data[2]==reqName:
                avg_data[3]+=int(num)
                avg_data[4]+=int(cycle)
                present = True
        if not present:
            new_data = [filename, "avg of all "+str(tile_count)+" tiles involved", reqName, num, int(cycle)]
            avg_data_list.append(new_data)
    
    for avg_data in avg_data_list:
        avg_data[3]=round(avg_data[3]/tile_count,2)
        avg_data[4]=round(avg_data[4]/tile_count,2)
        # avg_data[3]=round(avg_data[3],2)
        # avg_data[4]=round(avg_data[4],2)
        # write the data
        breakdown_avg_writer.writerow(avg_data)
    #write average idle cycles
    breakdown_avg_writer.writerow( [filename,"avg of all "+str(tile_count)+" tiles involved", "IDLE", 1, round(idle_cycle/tile_count,2)])
    # breakdown_avg_writer.writerow( [filename,"avg of all "+str(tile_count)+" tiles involved", "IDLE", 1, round(idle_cycle,2)])
    



# fout = open(dir+"/dma_extracted.txt", "w")
# if os.path.isfile(dir+"stdout"):
#     f = open(dir+"stdout")
#     for line in f:
#         if "issueDmaRequest" in line:
#             words = line.split()
#             # print(words[2])
#             dma_start_cycle = int(re.sub(":","",words[0]))
#             # kernel_invokes+=1
#             fout.write(line)
#             total_transferred_bytes += int(words[-1])
#         if "Adding DMA node" in line:
#             fout.write(line)
#         if "dmaCompleteCallback" in line:
#             words = line.split()
#             # print(words[2])
#             dma_end_cycle = int(re.sub(":","",words[0]))
#             dma_cycle_sum+=(dma_end_cycle-dma_start_cycle)
#             fout.write(line)
#     # print("kernel invokes:" + str(kernel_invokes))
#     dma_cycle_sum/=host_ticks_per_cycle
#     print("total dma transferred bytes: "+str(total_transferred_bytes))
#     print("total dma cycles: "+str(dma_cycle_sum))

# fout.close()

import csv


