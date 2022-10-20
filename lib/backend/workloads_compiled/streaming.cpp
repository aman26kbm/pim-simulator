// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

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

BC_LOAD_CONV_KERN_IMPL(12, 9);
// BC_LOAD_CONV_KERN_IMPL(8, 2352);
// BC_LOAD_CONV_KERN_IMPL(18, 8);

#define LOAD_CONV_KERN_IMPL(kernel_size, loaders)                                             \
  int32_t load_conv_kernel_##kernel_size##loaders(System *sys) {                              \
    for (int tile = 0; tile < 0; ++tile) {                                              \
      for (int row = 0; row < (kernel_size); ++row) {                                         \
        Request request(Request::Type::RowLoad);                                              \
        request.addOperand(sys->getAddress(tile, 0, row), 256,                                \
                           PrecisionT::Precision{0, 8, 0});                                   \
        request.addOperand(sys->DRAM_ADDR, 256, PrecisionT::Precision{0, 8, 0});              \
        sys->sendRequest(request);                                                            \
      }                                                                                       \
    }                                                                                         \
    for (int sender = 0; sender < (loaders); ++sender) {                                      \
      for (int recv = loaders; recv < 120; recv++) {                                          \
        for (int row = 0; row < (kernel_size); ++row) {                                       \
          {                                                                                   \
            Request request(Request::Type::TileReceive);                                      \
            request.addOperand(sys->getAddress(sender, 0, row), 256,                          \
                               PrecisionT::Precision{0, 8, 0});                               \
            request.addOperand(sys->getAddress(recv, 0, row), 256,                            \
                               PrecisionT::Precision{0, 8, 0});                               \
            sys->sendRequest(request);                                                        \
          }                                                                                   \
          {                                                                                   \
            Request request(Request::Type::TileSend);                                         \
            request.addOperand(sys->getAddress(sender, 0, row), 256,                          \
                               PrecisionT::Precision{0, 8, 0});                               \
            request.addOperand(sys->getAddress(recv, 0, row), 256,                            \
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
