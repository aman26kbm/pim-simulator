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
  ('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'backprop_forward_65536_16'),
  ('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'backprop_backward_65536_16'),
  ('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'gausselim_256'),
  ('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'dwt_1024'),
  ('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'hotspot_1024_1024'),
  ('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'hotspot3d_512_8_a100'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'dwt_4096'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'hotspot_4096_4096'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'hotspot3d_1024_8'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'gausselim_512'),

  # SIMDRAM VGG-13/16
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'vgg_bnn_13_1'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'vgg_bnn_13_2'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'vgg_bnn_13_3'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'vgg_bnn_13_256_512'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'vgg_bnn_13_512_512'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'vgg_bnn_13_512_256'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'vgg_mv_store_4096_4096'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'vgg_mv_4096_4096'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'vgg_mv_4096_256'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'len_bnn_1'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'lenet_bnn_2'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'len_bnn_3'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'lenet_4'),
  #('../configs/DMesh12x10_TileSize256_CramSize256x256.cfg', 'vgg_mv_256_256'),
]

cfg_list = ['../configs/DMesh12x10_TileSize256_CramSize256x256.cfg',
            '../configs/DMesh12x10_TileSize256_CramSize256x256_bus.cfg',
            '../configs/DMesh12x10_TileSize256_CramSize256x256_constOpOff_kickoutRow.cfg',
            '../configs/DMesh12x10_TileSize256_CramSize256x256_constOpOff.cfg',
            '../configs/DMesh12x10_TileSize256_CramSize256x256_crossCramShiftOff_bypassDram.cfg',
            '../configs/DMesh12x10_TileSize256_CramSize256x256_crossCramShiftOff.cfg',
            '../configs/DMesh12x10_TileSize256_CramSize256x256_oneToAllBroadcast.cfg',
            '../configs/DMesh12x10_TileSize256_CramSize256x256_shuffleOff.cfg',
            '../configs/DMesh12x10_TileSize256_CramSize256x256_everythingOff.cfg']

# cfg_list = ['../configs/DMesh12x10_TileSize256_CramSize256x256_everythingOff.cfg',]

for cfg in cfg_list:
  to_sim = [
    (cfg, "resnet18_v1__quantized_tvmgen_default_fused_cast_3"),
    (cfg, "resnet18_v1__quantized_tvmgen_default_fused_nn_max_pool2d"),
    (cfg, "resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_6"),
    (cfg, "resnet18_v1__quantized_tvmgen_default_fused_nn_contrib_dense_pack_add_cast_multiply"),
    (cfg, "resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_1"),
    (cfg, "resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_1"),
    (cfg, "resnet18_v1__quantized_tvmgen_default_fused_divide_round_cast_clip_cast_cast_layout_transform"),
    (cfg, "resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1"),
    (cfg, "resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_5"),
    (cfg, "resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast"),
    (cfg, "resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_2"),
    (cfg, "resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2"),
    (cfg, "resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast"),
    (cfg, "resnet18_v1__quantized_tvmgen_default_fused_cast_1"),
    (cfg, "resnet18_v1__quantized_tvmgen_default_fused_nn_softmax"),
    (cfg, "resnet18_v1__quantized_tvmgen_default_fused_cast_2"),
    (cfg, "resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_5603799462163545803_"),
    (cfg, "resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_2"),
    (cfg, "resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_2938127200454701776__3"),
    (cfg, "resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_3"),
    (cfg, "resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_4"),
    (cfg, "resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast"),
    (cfg, "resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_2938127200454701776__1"),
    (cfg, "resnet18_v1__quantized_tvmgen_default_fused_cast"),
    (cfg, "resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast"),
    (cfg, "resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_2938127200454701776_"),
    (cfg, "resnet18_v1__quantized_tvmgen_default_fused_nn_global_avg_pool2d_cast"),
    (cfg, "resnet18_v1__quantized_tvmgen_default_fused_layout_transform_nn_batch_flatten_cast"),
    (cfg, "resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_2938127200454701776__2"),
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


