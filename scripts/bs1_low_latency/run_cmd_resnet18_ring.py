import subprocess
import pathlib
import os
from pathlib import Path
d = os.path.dirname(os.path.abspath(__file__))
path = Path(d)
ROOT_PATH = path.parent.parent.absolute()

CFG_PATH = f"{ROOT_PATH}/configs/DMesh120x1_TileSize256_CramSize256x256.cfg"
EXEC_PATH = f"{ROOT_PATH}/build/PIM_simulator"

OUTPUT_PATH = "output_resnet18_ring"
pathlib.Path(OUTPUT_PATH).mkdir(parents=True, exist_ok=True) 

for param in ["resnet_conv1.param","resnet_conv2_1_1.param","resnet_conv3_1_1.param","resnet_conv3_1_2.param","resnet_conv4_1_1.param"]:
    PARAM_PATH = f"{ROOT_PATH}/params/resnet18_bs_1/{param}"
    for task in ["conv2d_lowLatency_onlyCompute_weightDup", "conv2d_lowLatency_onlyLoadWeight_weightDup", "conv2d_lowLatency_onlyLoadInput", "conv2d_lowLatency_onlyStore_weightDup", "conv2d_lowLatency_weightDup"]:
    # for task in ["conv2d_low_latency_only_load_weight"]:
        print(f"Running {PARAM_PATH} {task}")
        subprocess.run([EXEC_PATH, "-c", CFG_PATH, "-m", task, "-p", PARAM_PATH, "-l", f"{OUTPUT_PATH}/{param}_{task}.log"])

for param in ["resnet_conv4_1_2.param","resnet_conv5_1_1.param","resnet_conv5_1_2.param"]:
    PARAM_PATH = f"{ROOT_PATH}/params/resnet18_bs_1/{param}"
    for task in ["conv2d_lowLatency_onlyCompute_inputDup", "conv2d_lowLatency_onlyLoadWeight", "conv2d_lowLatency_onlyLoadInput_inputDup", "conv2d_lowLatency_onlyStore_inputDup", "conv2d_lowLatency_inputDup"]:
    # for task in ["conv2d_low_latency_only_load_weight"]:
        print(f"Running {PARAM_PATH} {task}")
        subprocess.run([EXEC_PATH, "-c", CFG_PATH, "-m", task, "-p", PARAM_PATH, "-l", f"{OUTPUT_PATH}/{param}_{task}.log"])