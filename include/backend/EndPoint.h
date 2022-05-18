#ifndef _ENDPOINT_H_
#define _ENDPOINT_H_


class EndPoint{
public:
    int self_index;
    bool ready;
    EndPoint(int index){
        self_index = index;
    }

};
#endif