import subprocess
import pathlib
import os
from pathlib import Path
d = os.path.dirname(os.path.abspath(__file__))
path = Path(d)
ROOT_PATH = path.parent.parent.absolute()

CFG_PATH = f"{ROOT_PATH}/configs/DMesh12x10_TileSize256_CramSize256x256.cfg"
EXEC_PATH = f"{ROOT_PATH}/build/PIM_simulator"

OUTPUT_PATH = "output_all_kernels_nap"
pathlib.Path(OUTPUT_PATH).mkdir(parents=True, exist_ok=True) 

runs = [
("gemm_systolic_nap", f"{OUTPUT_PATH}/gemm_handcoded.DMesh12x10_TileSize256_CramSize256x256.log"),
("gemv_new_nap", f"{OUTPUT_PATH}/gemv_handcoded.DMesh12x10_TileSize256_CramSize256x256.log"),
("fir_med_inp_256bit_loads_nap", f"{OUTPUT_PATH}/fir_handcoded.DMesh12x10_TileSize256_CramSize256x256.log"),
("vadd", f"{OUTPUT_PATH}/vecadd_handcoded.DMesh12x10_TileSize256_CramSize256x256.log")
]
for (task, log_file) in runs:
    print(f"Running {task}")
    subprocess.run([EXEC_PATH, "-c", CFG_PATH, "-m", task, "-l", log_file])

runs_conv = [
("conv2d_lowLatency_inputDup_nap", f"{OUTPUT_PATH}/conv2d_handcoded.DMesh12x10_TileSize256_CramSize256x256.log")
]
for (task, log_file) in runs_conv:
    print(f"Running {task}")
    PARAM_PATH = f"{ROOT_PATH}/params/conv2d.param"
    subprocess.run([EXEC_PATH, "-c", CFG_PATH, "-m", task, "-p", PARAM_PATH,  "-l", log_file])