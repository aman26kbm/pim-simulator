#!/bin/sh
python3 run_cmd_all_kernels.py
python3 collect_stats.py output_all_kernels
python3 generate_chart_data.py output_all_kernels

python3 run_cmd_all_kernels_nap.py
python3 collect_stats.py output_all_kernels_nap
python3 generate_chart_data.py output_all_kernels_nap

python3 run_cmd_resnet.py
python3 collect_stats.py output_resnet
python3 generate_chart_data.py output_resnet


python3 run_cmd_resnet_nap.py
python3 collect_stats.py output_resnet_nap
python3 generate_chart_data.py output_resnet_nap

python3 run_cmd_bert.py
python3 collect_stats.py output_bert
python3 generate_chart_data.py output_bert

python3 run_cmd_bert_nap.py
python3 collect_stats.py output_bert_nap
python3 generate_chart_data.py output_bert_nap