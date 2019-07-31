#ifndef SPLAYER_PIPENODE_H
#define SPLAYER_PIPENODE_H

#include "Log.h"
#include "Mutex.h"

class PipelineNode {

private:
    Mutex *mutex = nullptr;

public:
    PipelineNode();

    virtual ~PipelineNode();

    virtual void destroy() = 0;

    virtual int runSync() = 0;

    virtual int flush() = 0;
};

#endif //SPLAYER_PIPENODE_H