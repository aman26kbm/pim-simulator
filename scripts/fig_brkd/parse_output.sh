#!/bin/sh
#usage sh parse_output.sh <output_dir>
python3 collect_stats.py $1
python3 generate_chart_data.py $1