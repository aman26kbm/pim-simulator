import subprocess

to_sim = [
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'vecadd_cache_120_256_256'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'mv_120_256_256'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'gemm4_120_256_256'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'systolic_mesh_stream_10_12_256_256'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'conv_120_256_256'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'conv_3x3_weight_load_10_12_256_256'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'fir_120_256_256'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'fir_stream_120_256_256'),
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

  # precision sensitivity.
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'vecadd_120_256_256_int4'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'vecadd_120_256_256_int5'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'vecadd_120_256_256_int6'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'vecadd_120_256_256_int7'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'vecadd_120_256_256_int8'),

  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'fir_120_256_256_int4_int16'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'fir_120_256_256_int5_int20'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'fir_120_256_256_int6_int24'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'fir_120_256_256_int7_int28'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'fir_120_256_256_int8_int32'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'fir_stream_120_120_256_256_4'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'fir_stream_120_120_256_256_5'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'fir_stream_120_120_256_256_6'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'fir_stream_120_120_256_256_7'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'fir_stream_120_120_256_256_8'),

  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'mv_120_256_256_int4_int16'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'mv_120_256_256_int5_int20'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'mv_120_256_256_int6_int24'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'mv_120_256_256_int7_int28'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'mv_120_256_256_int8_int32'),

  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'gemm_120_256_256_int4_int20'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'gemm_120_256_256_int5_int22'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'gemm_120_256_256_int6_int24'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'gemm_120_256_256_int7_int26'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'gemm_120_256_256_int8_int28'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'systolic_mesh_stream_120_10_12_256_256_4'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'systolic_mesh_stream_120_10_12_256_256_5'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'systolic_mesh_stream_120_10_12_256_256_6'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'systolic_mesh_stream_120_10_12_256_256_7'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'systolic_mesh_stream_120_10_12_256_256_8'),

  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'conv_120_256_256_int4_int20'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'conv_120_256_256_int5_int22'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'conv_120_256_256_int6_int24'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'conv_120_256_256_int7_int26'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'conv_120_256_256_int8_int28'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'conv_3x3_weight_load_10_12_256_256_4'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'conv_3x3_weight_load_10_12_256_256_5'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'conv_3x3_weight_load_10_12_256_256_6'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'conv_3x3_weight_load_10_12_256_256_7'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'conv_3x3_weight_load_10_12_256_256_8'),

  # size sensitivity
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'vecadd_120_256_256_60'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'vecadd_120_256_256_240'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'fir_120_256_256_60'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'fir_120_256_256'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'fir_120_256_256_240'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'mv_120_256_256_60'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'mv_120_256_256_240'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'gemm_120_256_256_60'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'gemm_120_256_256_240'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'conv_120_256_256_1'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'conv_120_256_256_4'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'systolic_mesh_stream_60_10_12_256_256_4'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'systolic_mesh_stream_240_10_12_256_256_4'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'fir_stream_60_120_256_256_16'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'fir_stream_120_120_256_256_16'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'fir_stream_240_120_256_256_16'),

  #VGG
  #('../configs/SIMDRAM.cfg', 'vgg1'),
  #('../configs/SIMDRAM.cfg', 'vgg2'),
  #('../configs/SIMDRAM.cfg', 'vgg3'),
  #('../configs/SIMDRAM.cfg', 'vgg4'),
  #('../configs/SIMDRAM.cfg', 'vgg5'),

  # dc
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'backprop_65536_16'),
  ('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'gausselim_256'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'dwt_1024'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'hotspot_1024_1024'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'hotspot3d_512_8'),
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


