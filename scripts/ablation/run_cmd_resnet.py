import subprocess
import pathlib
import os
from pathlib import Path
d = os.path.dirname(os.path.abspath(__file__))
path = Path(d)
ROOT_PATH = path.parent.parent.absolute()

CFG_PATH = f"{ROOT_PATH}/configs/DMesh12x10_TileSize256_CramSize256x256.cfg"
EXEC_PATH = f"{ROOT_PATH}/build/PIM_simulator"

OUTPUT_PATH = "output_resnet"
pathlib.Path(OUTPUT_PATH).mkdir(parents=True, exist_ok=True) 

ablations = [ 
    "constOpOff",
    "constOpOff_kickoutRow",
    "shuffleOff",
    "crossCramShiftOff",
    "crossCramShiftOff_bypassDram",
    "oneToAllBroadcast",
    "bus",
    "ring",
    "everythingOff"
]

# ablations = [ 
#     "everythingOff"
# ]

for param in ["resnet_conv1.param","resnet_conv2_1_1.param","resnet_conv3_1_1.param","resnet_conv3_1_2.param","resnet_conv4_1_1.param"]:
    PARAM_PATH = f"{ROOT_PATH}/params/resnet18_bs_1/{param}"
    for task in ["conv2d_lowLatency_onlyLoadWeight_weightDup"]:
    # for task in ["conv2d_low_latency_only_load_weight"]:
        for ablation in ablations:
            print(f"Running {PARAM_PATH} {task} with {ablation}")
            subprocess.run([EXEC_PATH, "-c", f"{ROOT_PATH}/configs/DMesh12x10_TileSize256_CramSize256x256_{ablation}.cfg", "-m", task, "-p", PARAM_PATH, "-l", f"{OUTPUT_PATH}/{param}_{task}_{ablation}.log"])

for param in ["resnet_conv4_1_2.param","resnet_conv5_1_1.param","resnet_conv5_1_2.param"]:
    PARAM_PATH = f"{ROOT_PATH}/params/resnet18_bs_1/{param}"
    for task in ["conv2d_lowLatency_inputDup"]:
    # for task in ["conv2d_low_latency_only_load_weight"]:
        for ablation in ablations:
            print(f"Running {PARAM_PATH} {task} with {ablation}")
            subprocess.run([EXEC_PATH, "-c", f"{ROOT_PATH}/configs/DMesh12x10_TileSize256_CramSize256x256_{ablation}.cfg", "-m", task, "-p", PARAM_PATH, "-l", f"{OUTPUT_PATH}/{param}_{task}_{ablation}.log"])