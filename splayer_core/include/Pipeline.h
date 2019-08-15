
#ifndef SPLAYER_PIPELINE_H
#define SPLAYER_PIPELINE_H

#include "Log.h"
#include "Audio.h"
#include "Surface.h"
#include "PipelineNode.h"

class Pipeline {

private:

    Surface *surface;

public:
    Pipeline();

    virtual ~Pipeline();

    // destroy
    virtual void close() = 0;

    virtual Audio *openAudioOutput() = 0;

    virtual PipelineNode *openAudioDecoder() = 0;

    virtual PipelineNode *openVideoDecoder() = 0;

    void setSurface(Surface *surface);

};


#endif //SPLAYER_PIPELINE_H
