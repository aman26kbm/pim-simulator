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

# ./PIM_simulator -c ../configs/DMesh12x10_TileSize256_CramSize256x256.cfg -m gemm_systolic -l gemm_handcoded.DMesh12x10_TileSize256_CramSize256x256.log
# ./PIM_simulator -c ../configs/DMesh12x10_TileSize256_CramSize256x256.cfg -m gemv_new -l gemv_handcoded.DMesh12x10_TileSize256_CramSize256x256.log
# ./PIM_simulator -c ../configs/DMesh12x10_TileSize256_CramSize256x256.cfg -m fir_med_inp_256bit_loads -l fir_handcoded.DMesh12x10_TileSize256_CramSize256x256.log
# ./PIM_simulator -c ../configs/DMesh12x10_TileSize256_CramSize256x256.cfg -m vadd -l vecadd_handcoded.DMesh12x10_TileSize256_CramSize256x256.log
# ./PIM_simulator -c ../configs/DMesh12x10_TileSize256_CramSize256x256.cfg -m conv2d_ocvec -l conv2d_handcoded.DMesh12x10_TileSize256_CramSize256x256.log
runs = [
("gemm_systolic", f"{OUTPUT_PATH}/gemm_handcoded.DMesh12x10_TileSize256_CramSize256x256.log"),
("gemv_new", f"{OUTPUT_PATH}/gemv_handcoded.DMesh12x10_TileSize256_CramSize256x256.log"),
("fir_med_inp_256bit_loads", f"{OUTPUT_PATH}/fir_handcoded.DMesh12x10_TileSize256_CramSize256x256.log"),
("vadd", f"{OUTPUT_PATH}/vecadd_handcoded.DMesh12x10_TileSize256_CramSize256x256.log"),
("conv2d_ocvec", f"{OUTPUT_PATH}/conv2d_handcoded.DMesh12x10_TileSize256_CramSize256x256.log")
]
for (task, log_file) in runs:
    print(f"Running {task}")
    subprocess.run([EXEC_PATH, "-c", CFG_PATH, "-m", task, "-l", log_file])