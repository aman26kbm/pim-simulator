#!/bin/sh
# python3 run_cmd_all_kernels.py
# python3 run_cmd_resnet.py
python3 run_cmd_bert.py
python3 run_cmd_mlp_1.py
python3 run_cmd_mlp_2.py

python3 collect_stats.py output_all_kernels
python3 generate_chart_data.py output_all_kernels
# python3 collect_stats.py output_resnet
# python3 generate_chart_data.py output_resnet
python3 collect_stats.py output_bert
python3 generate_chart_data.py output_bert
python3 collect_stats.py output_mlp_1
python3 generate_chart_data.py output_mlp_1
python3 collect_stats.py output_mlp_2
python3 generate_chart_data.py output_mlp_2

python3 exec-brkd.py

python3 energy-brkd.py

