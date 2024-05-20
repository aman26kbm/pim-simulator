#!/bin/sh
#usage sh parse_output.sh
python3 collect_stats.py output_all_kernels
python3 generate_chart_data.py output_all_kernels

python3 collect_stats.py output_resnet
python3 generate_chart_data.py output_resnet

python3 collect_stats.py output_bert
python3 generate_chart_data.py output_bert

python3 collect_stats.py output_mlp_1
python3 generate_chart_data.py output_mlp_1

python3 collect_stats.py output_mlp_2
python3 generate_chart_data.py output_mlp_2