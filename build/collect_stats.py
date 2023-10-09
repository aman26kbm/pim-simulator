import os, glob
import sys
import re

print('Number of arguments:', len(sys.argv), 'arguments.')
print('Argument List:', str(sys.argv))

path = str(sys.argv[1])

for filename in sorted(glob.glob(os.path.join(path, '*.log'))):
    cycle=0
    with open(os.path.join(os.getcwd(), filename), 'r') as f:
       for line in f:
           if "clocks" in line:
               words = line.split()
               cycle = int(words[4])
               break
    print(filename+": total cycles: "+str(cycle))

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
for filename in sorted(glob.glob(os.path.join(path, '*.energy.csv'))):
    with open(os.path.join(os.getcwd(), filename), 'r') as f:
        csv_reader = csv.DictReader(f)
        for row in csv_reader:
            print(f'file {filename}:')
            print(f'\t{row["WorkloadName"]} has total dynamic energy = {row["Total_Dynamic_Energy"]}, and total static energy = {row["Total_Static_Energy"]}.')
