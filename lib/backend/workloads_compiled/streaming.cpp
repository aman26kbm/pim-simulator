// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>
#include <cstring>

#include "backend/System.h"

#define BC_LOAD_CONV_KERN_IMPL(kernel_size, loaders)                                          \
  int32_t bc_load_conv_kernel_##kernel_size##loaders(System *sys) {                           \
    for (int tile = 0; tile < loaders; ++tile) {                                              \
      for (int row = 0; row < (kernel_size); ++row) {                                         \
        Request request(Request::Type::RowLoad);                                              \
        request.addOperand(sys->getAddress(tile, 0, row), 0, PrecisionT::Precision{0, 8, 0}); \
        request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::Precision{0, 8, 0});                \
        sys->sendRequest(request);                                                            \
      }                                                                                       \
      for (int row = 0; row < (kernel_size); ++row) {                                         \
        Request request(Request::Type::TileSend_BroadCast);                                   \
        request.addOperand(sys->getAddress(tile, 0, row), 0, PrecisionT::Precision{0, 8, 0}); \
        sys->sendRequest(request);                                                            \
        for (int tile_in = loaders; tile_in < 128; tile_in++) {                               \
          Request request(Request::Type::TileReceive_BroadCast);                              \
          request.addOperand(sys->getAddress(tile_in, 0, row), 0,                             \
                             PrecisionT::Precision{0, 8, 0});                                 \
          sys->sendRequest(request);                                                          \
        }                                                                                     \
      }                                                                                       \
    }                                                                                         \
    return 0;                                                                                 \
  }                                                                                           \
static __attribute__((unused)) Registry::Entry &bc_load_conv_kernel##kernel_size##loaders =   \
  pimsim::registerFunc("bc_load_conv_kernel_" #kernel_size "_" #loaders,                      \
                       bc_load_conv_kernel_##kernel_size##loaders)

BC_LOAD_CONV_KERN_IMPL(9, 12);
// BC_LOAD_CONV_KERN_IMPL(8, 2352);
// BC_LOAD_CONV_KERN_IMPL(18, 8);

#define LOAD_CONV_KERN_IMPL(kernel_size, loaders)                                             \
  int32_t load_conv_kernel_##kernel_size##loaders(System *sys) {                              \
    for (int tile = 0; tile < 0; ++tile) {                                                    \
      for (int row = 0; row < (kernel_size); ++row) {                                         \
        Request request(Request::Type::RowLoad);                                              \
        request.addOperand(sys->getAddress(tile, 0, row), 1024,                               \
                           PrecisionT::Precision{0, 8, 0});                                   \
        request.addOperand(sys->DRAM_ADDR, 1024, PrecisionT::Precision{0, 8, 0});             \
        sys->sendRequest(request);                                                            \
      }                                                                                       \
    }                                                                                         \
    for (int sender = 0; sender < (loaders); ++sender) {                                      \
      for (int recv = loaders; recv < 120; recv++) {                                          \
        for (int row = 0; row < (kernel_size); ++row) {                                       \
          {                                                                                   \
            Request request(Request::Type::TileReceive);                                      \
            request.addOperand(sys->getAddress(sender, 0, row), 1024,                         \
                               PrecisionT::Precision{0, 8, 0});                               \
            request.addOperand(sys->getAddress(recv, 0, row), 1024,                           \
                               PrecisionT::Precision{0, 8, 0});                               \
            sys->sendRequest(request);                                                        \
          }                                                                                   \
          {                                                                                   \
            Request request(Request::Type::TileSend);                                         \
            request.addOperand(sys->getAddress(sender, 0, row), 1024,                         \
                               PrecisionT::Precision{0, 8, 0});                               \
            request.addOperand(sys->getAddress(recv, 0, row), 1024,                           \
                               PrecisionT::Precision{0, 8, 0});                               \
            sys->sendRequest(request);                                                        \
          }                                                                                   \
        }                                                                                     \
      }                                                                                       \
    }                                                                                         \
    return 0;                                                                                 \
  }                                                                                           \
static __attribute__((unused)) Registry::Entry &load_conv_kernel##kernel_size##loaders =      \
  pimsim::registerFunc("load_conv_kernel_" #kernel_size "_" #loaders,                         \
                       load_conv_kernel_##kernel_size##loaders)


LOAD_CONV_KERN_IMPL(9, 8);
LOAD_CONV_KERN_IMPL(1, 8);
// LOAD_CONV_KERN_IMPL(8, 16);
// LOAD_CONV_KERN_IMPL(18, 16);

int32_t write_read_activations(System *sys) {
  int cores = 112;
  int llc_cores = 128 - cores;
  int blocks = 7;
  for (int tile = 0; tile < cores; ++tile) {
    for (int block = 0; block < blocks; ++block) {
      {
        Request request(Request::Type::TileReceive);
        request.addOperand(sys->getAddress(tile, 0, block), 0, PrecisionT::Precision{0, 8, 0});
        request.addOperand(sys->getAddress(cores + tile % llc_cores, 0, block), 0,
                           PrecisionT::Precision{0, 8, 0});
        
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::TileSend);
        request.addOperand(sys->getAddress(tile, 0, block), 0, PrecisionT::Precision{0, 8, 0});
        request.addOperand(sys->getAddress(cores + tile % llc_cores, 0, block), 0,
                           PrecisionT::Precision{0, 8, 0});
        sys->sendRequest(request);
      }
    }
  }
  return 0;
}

static __attribute__((unused)) Registry::Entry &write_read_activations_reg =
  pimsim::registerFunc("write_read_act_112_7", write_read_activations);


#define SYSTOLIC_MESH_STREAM(rows, cols, arrays, bitlines) \
int32_t systolic_mesh_stream_##rows##_##cols##_##arrays##_##bitlines##_impl(System *sys) { \
  int total = (120 * 512 * 256 * 8) / cols / arrays / bitlines; \
  for (int i = 0; i < cols; ++i) { \
    for (int k = 0; k < total; ++k) { \
      Request request(Request::Type::RowLoad); \
      request.addOperand(sys->getAddress(i, 0, 0), 0, PrecisionT::Precision{0, 4, 0}); \
      request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::Precision{0, 4, 0}); \
      sys->sendRequest(request); \
      std::vector<int> recv; \
      for (int j = 1; j < rows; ++j) { \
        recv.push_back(j * cols + i); \
      } \
      sys->broadcast_p2p(sys->getAddress(i, 0, 1), PrecisionT::Precision{0, 4, 0}, recv, \
                         arrays * bitlines); \
    } \
  } \
  for (int i = 0; i < rows; ++i) { \
    int current = i % cols; \
    { \
      Request request(Request::Type::RowLoad); \
      request.addOperand(sys->getAddress(current, 0, 1), 0, PrecisionT::Precision{0, 4, 0}); \
      request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::Precision{0, 4, 0}); \
      sys->sendRequest(request); \
    } \
    std::vector<int> recv; \
    for (int j = 1; j < cols; ++j) { \
      recv.push_back(i * cols + j); \
    } \
    sys->broadcast_p2p(sys->getAddress(current, 0, 1), PrecisionT::Precision{0, 4, 0}, recv, \
                       arrays * bitlines); \
  } \
  return 0; \
} \
static __attribute__((unused)) Registry::Entry \
  &systolic_mesh_stream_##rows##_##cols##_##arrays##_##bitlines##_reg = \
  pimsim::registerFunc("systolic_mesh_stream_" #rows "_" #cols "_" #arrays "_" #bitlines, \
                       systolic_mesh_stream_##rows##_##cols##_##arrays##_##bitlines##_impl);

SYSTOLIC_MESH_STREAM(10, 12, 1024, 128);
SYSTOLIC_MESH_STREAM(10, 12, 64, 512);

SYSTOLIC_MESH_STREAM(5, 12, 512, 256);
SYSTOLIC_MESH_STREAM(10, 12, 256, 256);
SYSTOLIC_MESH_STREAM(20, 12, 128, 256);

SYSTOLIC_MESH_STREAM(5, 24, 256, 256);
SYSTOLIC_MESH_STREAM(20, 6, 256, 256);

#define CONV_3x3_WEIGHT_LOAD(row, col, ic, oc) \
int conv_3x3_weight_load_##row##_##col##_##ic##_##oc##_impl(System *sys) { \
  int loaders = col; \
  int width = min(ic * oc, 65536); \
  int cnt[loaders]; \
  memset(cnt, 0, sizeof cnt); \
  int total = 3 * 3 * ic * oc; \
  int tile = 0; \
  while (total) { \
    Request request(Request::Type::RowLoad); \
    request.addOperand(sys->getAddress(tile, 0, 1), width, PrecisionT::Precision{0, 8, 0}); \
    request.addOperand(sys->DRAM_ADDR, width, PrecisionT::Precision{0, 8, 0}); \
    sys->sendRequest(request); \
    total -= width; \
    cnt[tile]++; \
    tile = (tile + 1) % loaders; \
  } \
  for (int i = 0; i < loaders; ++i) { \
    std::vector<int> recv; \
    for (int j = 8; j < row * col; ++j) { \
      if (j != i) { \
        recv.push_back(j); \
      } \
    } \
    for (int j = 0; j < cnt[i]; ++j) { \
      sys->broadcast_p2p(sys->getAddress(i, 0, 1), PrecisionT::Precision{0, 8, 0}, recv, 65536); \
    } \
  } \
  return 0; \
} \
static __attribute__((unused)) Registry::Entry &conv_3x3_weight_load_##row##_##col##_##ic##_##oc##_reg = \
  pimsim::registerFunc("conv_3x3_weight_load_" #row "_" #col "_" #ic "_" #oc, \
  conv_3x3_weight_load_##row##_##col##_##ic##_##oc##_impl)

CONV_3x3_WEIGHT_LOAD(10, 12, 4, 64);
CONV_3x3_WEIGHT_LOAD(10, 12, 64, 64);
CONV_3x3_WEIGHT_LOAD(10, 12, 64, 128);
CONV_3x3_WEIGHT_LOAD(10, 12, 128, 128);
CONV_3x3_WEIGHT_LOAD(10, 12, 128, 256);
CONV_3x3_WEIGHT_LOAD(10, 12, 256, 256);
CONV_3x3_WEIGHT_LOAD(10, 12, 256, 512);
CONV_3x3_WEIGHT_LOAD(10, 12, 512, 512);

CONV_3x3_WEIGHT_LOAD(5, 24, 512, 512);

int conv_3x3_weight_load_6_impl(System *sys) {
  for (int tile = 0; tile < 9; ++tile) {
    Request request(Request::Type::RowLoad);
    request.addOperand(sys->getAddress(tile % 6, 0, 1), 65536, PrecisionT::Precision{0, 8, 0});
    request.addOperand(sys->DRAM_ADDR, 65536, PrecisionT::Precision{0, 8, 0});
    sys->sendRequest(request);
  }
  for (int i = 0; i < 9; ++i) {
    std::vector<int> recv;
    for (int j = 0; j < 120; ++j) {
      if (j != i) {
        recv.push_back(j);
      }
    }
    sys->broadcast_p2p(sys->getAddress(i % 6, 0, 1), PrecisionT::Precision{0, 8, 0}, recv, 65536);
  }
  return 0;
}

static __attribute__((unused)) Registry::Entry &conv_3x3_weight_load_6_reg =
  pimsim::registerFunc("conv_3x3_weight_load_6", conv_3x3_weight_load_6_impl);

#define FIR_STREAM(cores, arrays, bitlines) \
int fir_stream_##cores##_##arrays##_##bitlines##_impl(System *sys) { \
  int total = 120 * 256 * 256; \
  int width = (bitlines * arrays); \
  int tile = 0; \
  while (total) { \
    total -= width; \
    { \
      Request request(Request::Type::RowLoad); \
      request.addOperand(sys->getAddress(tile, 0, 1), width, PrecisionT::Precision{0, 8, 0}); \
      request.addOperand(sys->DRAM_ADDR, width, PrecisionT::Precision{0, 8, 0}); \
      sys->sendRequest(request); \
    } \
    tile++; \
    tile %= cores; \
  } \
  { \
    Request request(Request::Type::RowLoad); \
    request.addOperand(sys->getAddress(0, 0, 1), 32, PrecisionT::Precision{0, 8, 0}); \
    request.addOperand(sys->DRAM_ADDR, 32, PrecisionT::Precision{0, 8, 0}); \
    sys->sendRequest(request); \
  } \
  { \
    std::vector<int> recv; \
    for (int i = 1; i < cores; ++i) { \
      recv.push_back(i); \
    } \
    sys->broadcast_p2p(sys->getAddress(0, 0, 1), PrecisionT::Precision{0, 8, 0}, recv, 32); \
  } \
  return 0; \
} \
static __attribute__((unused)) Registry::Entry &fir_stream_##cores##_##bitlines##_##arrays##_reg = \
  pimsim::registerFunc("fir_stream_" #cores "_" #arrays "_" #bitlines, \
                       fir_stream_##cores##_##arrays##_##bitlines##_impl);

FIR_STREAM(120, 64, 512);
FIR_STREAM(120, 256, 256);
FIR_STREAM(120, 1024, 128);
FIR_STREAM(60, 512, 256);
FIR_STREAM(240, 128, 256);

