#include "InterConnect.h"

namespace pimsim {

enum status_t {
    IDLE, 
    REQ_MODE,
    SEND_WAIT,
    SEND_MODE,
    SEND_DONE,
    RECEIVE_WAIT,
    RECEIVE_MODE
};

class MemoryTile {
public:
    uint64_t* glb_clk;
    uint64_t next_available;
    status_t status;
    bool receive_ready;
    bool send_done;
    ReqQueue q;
    /* Per-tile statistics */
    uint64_t n_reads = 0, n_writes = 0, n_unexpected_reqs = 0;
    int nrow, ncol;
    MemoryCharacteristics memCharac;

    MemoryTile(){}
    MemoryTile(uint64_t* clk, int nrow, int ncol){
        this->glb_clk = clk;
        this->nrow = nrow;
        this->ncol = ncol;
    };

    

    void init();

    void update_next();
    void update_current();

    void issueReq(Request req);
    void finishReq(Request req);
    void commitReq(Request req);

    virtual void outputStats(FILE* rstFile);

private:
    MemoryTile* next;
};





}