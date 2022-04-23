#include "system.h"
#include "Memory_characteristics.h"


namespace pimsim {

enum status_t {
    IDLE, 
    REQ_MODE,
    SEND_WAIT,
    SEND_MODE,
    RECEIVE_WAIT,
    RECEIVE_MODE
};

class MemoryTile {
public:
    uint32_t next_available;
    status_t status;
    ReqQueue q;
    /* Per-tile statistics */
    uint64_t n_reads = 0, n_writes = 0, n_unexpected_reqs = 0;

    MemoryTile(int n_rows, int n_cols);

    bool receive_ready;
    bool send_done;

    void tick();

    void issueReq(Request req);
    void finishReq(Request req);
    void commitReq(Request req);

    virtual void outputStats(FILE* rstFile);
};




}