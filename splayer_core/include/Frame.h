#ifndef SPLAYER_MAC_FRAME_H
#define SPLAYER_MAC_FRAME_H

extern "C" {
#include <libavutil/rational.h>
#include <libavutil/frame.h>
#include <libavformat/avformat.h>
};

/**
 * Common struct for handling all types of decoded data and allocated render buffers.
 */
class Frame {
public:
    AVFrame *frame;
    AVSubtitle sub;
    AVRational sar;
    int serial;
    double pts;           /* presentation timestamp for the frame */
    double duration;      /* estimated duration of the frame */
    int64_t pos;          /* byte position of the frame in the input file */
    int width;
    int height;
    int format;
    int uploaded;
    int flip_v;
};


#endif //SPLAYER_MAC_FRAME_H