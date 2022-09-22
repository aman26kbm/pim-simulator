#include "backend/MemoryComponent.h"

using namespace std;
using namespace pimsim;

MemoryComponent::MemoryComponent(Level level)
    : _level(level)
{
    for (int i = 0; i < int(Request::Type::MAX); i++) {
        req_cnt[i] = 0;
        req_proctime[i] = 0;
        req_waittime[i] = 0;
        req_energy[i] = 0;
    }
}

void 
MemoryComponent::setController(Controller* ctrl)
{
    _ctrl = ctrl;
}

void
MemoryComponent::setValues(MemoryCharacteristics* values) 
{
    _values = values;
    for (auto child : _children) {
        child->setValues(values);
    }
}

double MemoryComponent::getReqTiming(Request req) {
    return _values->getTiming(req);
}

int MemoryComponent::getPrecisionBits(Request req) {
    return _values->getPrecisionBits(req);
}


TimeT
MemoryComponent::getDecoderTime()
{
    return _ctrl->getDecoderTime();
}

void
MemoryComponent::setDecoderTime(TimeT time)
{
    _ctrl->setDecoderTime(time);
}

void 
MemoryComponent::outputStats(FILE* rstFile) 
{
    Request req(Request::Type::MAX);

    fprintf(rstFile, "\n----------------------------------------\n");

    fprintf(rstFile, "%s#%d\n", level_str[int(_level)].c_str(), _id);

    fprintf(rstFile, "Request Type Breakdown:\nType, Cnt, ProcessingTime(clk), WaitTime(clk), Energy(nj)\n");
    for (int i = 0; i < int(Request::Type::MAX); i++) {
        if (req_cnt[i] != 0) {
            fprintf(rstFile, "%s, %lu, %.4lf, %.4lf, %.4lf\n",
                Request::print_name((Request::Type)i).c_str(), req_cnt[i], req_proctime[i], req_waittime[i], req_energy[i]);
        }
    }

}

bool 
MemoryComponent::receiveReq(Request& req) 
{
#ifdef DEBUG_OUTPUT
    printf("+++++%s_%d receives a reqeust! (%s : %lu)\n", level_str[int(_level)].c_str(), _id,
            req.reqToStr().c_str(), req.arrive_time);
#endif
    bool res;
    if (getLevel() == MemoryComponent::Level::Chip) {
        //Send to tiles
        int idx = req.tile;
        return _children[idx]->receiveReq(req);
    }
    else if (getLevel() == MemoryComponent::Level::Tile) {
        //Send to controller
        res = _ctrl->receiveReq(req);
        return res;
    }
    else {
        std::cout<<"receiveReq() for Block is illegal to call, because we only use it for Tile";
        assert(0);
    }

#ifdef DEBUG_OUTPUT
    if (res)
        printf("-----%s_%d receives a reqeust (%s : %lu) successfully!\n", level_str[int(_level)].c_str(), _id, req.reqToStr().c_str(), req.arrive_time);
    else
        printf("=====%s_%d cannot enqueue a request (%s : %lu)!\n", level_str[int(_level)].c_str(), _id, req.reqToStr().c_str(), req.arrive_time);
#endif

}

void
MemoryComponent::finishReq(Request& req)
{
    req_cnt[int(req.type)]++;    
    req_proctime[int(req.type)] += req.finish_time - req.process_time;
    req_waittime[int(req.type)] += req.process_time - req.start_time;
}