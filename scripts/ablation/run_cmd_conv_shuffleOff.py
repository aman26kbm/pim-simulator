import subprocess
import pathlib
import os
from pathlib import Path
d = os.path.dirname(os.path.abspath(__file__))
path = Path(d)
ROOT_PATH = path.parent.parent.absolute()

CFG_PATH = f"{ROOT_PATH}/configs/DMesh12x10_TileSize256_CramSize256x256.cfg"
EXEC_PATH = f"{ROOT_PATH}/build/PIM_simulator"


OUTPUT_PATH = "output_all_kernels"
pathlib.Path(OUTPUT_PATH).mkdir(parents=True, exist_ok=True) 

runs = [
("conv2d_lowLatency_inputDup", f"{OUTPUT_PATH}/conv2d_handcoded.DMesh12x10_TileSize256_CramSize256x256")
]
ablations = [
    "everythingOff"
]
# for (task, log_file) in runs:
#     print(f"Running {task}")
#     subprocess.run([EXEC_PATH, "-c", CFG_PATH, "-m", task, "-l", log_file])
#     for ablation in ablations:
#         print(f"Running {task} with {ablation}")
#         subprocess.run([EXEC_PATH, "-c", f"{ROOT_PATH}/configs/DMesh12x10_TileSize256_CramSize256x256_{ablation}.cfg", "-m", task, "-l", f"{log_file}_{ablation}.log"])
for (task, log_file) in runs:
    PARAM_PATH = f"{ROOT_PATH}/params/conv2d.param"
    # print(f"Running {task}")
    # subprocess.run([EXEC_PATH, "-c", CFG_PATH, "-m", task,"-p", PARAM_PATH, "-l", f"{log_file}.log"])
    for ablation in ablations:
        print(f"Running {task} with {ablation}")
        subprocess.run([EXEC_PATH, "-c", f"{ROOT_PATH}/configs/DMesh12x10_TileSize256_CramSize256x256_{ablation}.cfg", "-m", task,"-p", PARAM_PATH, "-l", f"{log_file}_{ablation}.log"])
