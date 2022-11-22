import subprocess

to_sim = [
    #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'vecadd_cache_120_256_256'),
    #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'mv_120_256_256'),
    #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'gemm4_120_256_256'),
    #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'systolic_mesh_stream_10_12_256_256'),
    #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'conv_120_256_256'),
    #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'conv_3x3_weight_load_10_12_256_256'),
    ('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'fir_120_256_256'),
    ('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'fir_stream_120_256_256'),

    #('../configs/DMesh24x5_TileSize256_CramSize256x256.cfg', 'gemm4_120_256_256'),
    #('../configs/DMesh24x5_TileSize256_CramSize256x256.cfg', 'systolic_mesh_stream_5_24_256_256')

    #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'vadd'),
    #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'gemv_new'),
    #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'gemm_systolic'),
    #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'gemm_inner_multicast'),
    #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'conv2d_ocvec'),
    #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'fir_med_inp_256bit_loads'),
    #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'fir_med_inp'),

    #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'conv_3x3_weight_load_10_12_4_64'),
    #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'conv_3x3_weight_load_10_12_64_64'),
    #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'conv_3x3_weight_load_10_12_64_128'),
    #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'conv_3x3_weight_load_10_12_128_128'),
    #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'conv_3x3_weight_load_10_12_128_256'),
    #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'conv_3x3_weight_load_10_12_256_256'),
    #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'conv_3x3_weight_load_10_12_256_512'),
    #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'conv_3x3_weight_load_10_12_512_512')
]

for cfg, wl in to_sim:
    cmd = f'./PIM_simulator --cfg {cfg} --sim {wl}'
    dirname = (cfg.split('/')[-1])[:-4]
    subprocess.check_output(f'mkdir -p {dirname}', shell=True)
    dirname = dirname + '/' + wl
    subprocess.check_output(f'mkdir -p {dirname}', shell=True)
    print(f'running {cmd}')
    raw_log = subprocess.check_output(cmd, shell=True).decode('utf-8')
    with open(f'{dirname}/wl.dump', 'w') as f:
        f.write(raw_log)
    subprocess.check_output(f'mv pimsim.* ./{dirname}', shell=True)


