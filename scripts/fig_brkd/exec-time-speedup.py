import csv

def get_time(workload_name, csv_reader):
    if workload_name == "resnet":
        sum=0
        for line in csv_reader:
            if workload_name in line[0]:
                if "conv1" in line[0]:
                    sum+=float(line[1])
                elif "conv2_1_1" in line[0]:
                    sum+=float(line[1]) * 4
                elif "conv3_1_1" in line[0]:
                    sum+=float(line[1])
                elif "conv3_1_2" in line[0]:
                    sum+=float(line[1]) * 3
                elif "conv4_1_1" in line[0]:
                    sum+=float(line[1])
                elif "conv4_1_2" in line[0]:
                    sum+=float(line[1]) * 3
                elif "conv5_1_1" in line[0]:
                    sum+=float(line[1])
                elif "conv5_1_2" in line[0]:
                    sum+=float(line[1]) * 3
        return float(sum)
    else:
        for line in csv_reader:
            if workload_name in line[0]:
                return float(line[1])

def get_energy(workload_name, csv_reader):
    if workload_name == "resnet":
        sum=0
        for line in csv_reader:
            if workload_name in line[0]:
                if "conv1" in line[0]:
                    sum+=float(line[2])
                elif "conv2_1_1" in line[0]:
                    sum+=float(line[2]) * 4
                elif "conv3_1_1" in line[0]:
                    sum+=float(line[2])
                elif "conv3_1_2" in line[0]:
                    sum+=float(line[2]) * 3
                elif "conv4_1_1" in line[0]:
                    sum+=float(line[2])
                elif "conv4_1_2" in line[0]:
                    sum+=float(line[2]) * 3
                elif "conv5_1_1" in line[0]:
                    sum+=float(line[2])
                elif "conv5_1_2" in line[0]:
                    sum+=float(line[2]) * 3
        return float(sum)
    else:
        for line in csv_reader:
            print(line)
            if workload_name in line[0]:
                return float(line[2])
data  = []
with open('output_all_kernels/00_stats.csv', 'r') as f:
    csv_reader = csv.reader(f)
    lines = list(csv_reader)
    
    print(get_energy('vecadd', lines))
    for workload in ['vecadd', 'fir', 'gemv', 'gemm', 'conv2d', 'resnet']:
        data.append([workload, get_time(workload, lines), get_energy(workload, lines)])
    print(data)

with open('gpu.csv') as f_gpu:
    csv_reader = csv.reader(f_gpu)
    lines = list(csv_reader)
    for line in lines:
        for i in range(len(data)):
            if data[i][0] in line[0]:
                data[i]+=[float(line[1]), float(line[2])]
    print(data)
        