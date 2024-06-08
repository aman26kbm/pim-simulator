#!/bin/sh

python3 run_cmd_resnet.py
python3 collect_stats.py output_resnet
python3 generate_chart_data.py output_resnet


python3 run_cmd_resnet_nap.py
python3 collect_stats.py output_resnet_nap
python3 generate_chart_data.py output_resnet_nap