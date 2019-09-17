#include "decoder/AudioDecoder.h"

AudioDecoder::AudioDecoder(AVCodecContext *avctx, AVStream *stream, int streamIndex, PlayerState *playerState,
                           AVPacket *flushPacket, Condition *readWaitCond)
        : MediaDecoder(avctx, stream, streamIndex, playerState, flushPacket, readWaitCond) {
    isPendingPacket = false;
}

AudioDecoder::~AudioDecoder() {
    isPendingPacket = false;
}

void AudioDecoder::start() {
    MediaDecoder::start();
}

int AudioDecoder::getAudioFrame(AVFrame *frame) {
    return -1;
}






