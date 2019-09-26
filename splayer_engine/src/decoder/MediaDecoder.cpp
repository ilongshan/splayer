#include <decoder/MediaDecoder.h>

MediaDecoder::MediaDecoder(AVCodecContext *codecContext, AVStream *stream,
                           int streamIndex, PlayerState *playerState,
                           AVPacket *flushPacket, Condition *readWaitCond) {
    this->packetQueue = new PacketQueue(flushPacket);
    this->codecContext = codecContext;
    this->stream = stream;
    this->streamIndex = streamIndex;
    this->playerState = playerState;
    this->flushPacket = flushPacket;
    this->readWaitCond = readWaitCond;
}

MediaDecoder::~MediaDecoder() {
    packetQueue->flush();
    delete packetQueue;
    packetQueue = nullptr;
    if (codecContext != nullptr) {
        avcodec_close(codecContext);
        avcodec_free_context(&codecContext);
        codecContext = nullptr;
    }
    playerState = nullptr;
}

void MediaDecoder::start() { packetQueue->start(); }

void MediaDecoder::stop() { packetQueue->abort(); }

void MediaDecoder::pushFlushPacket() { packetQueue->pushPacket(flushPacket); }

void MediaDecoder::flush() { packetQueue->flush(); }

int MediaDecoder::pushPacket(AVPacket *pkt) {
    return packetQueue->pushPacket(pkt);
}

int MediaDecoder::getPacketSize() { return packetQueue->getPacketSize(); }

int MediaDecoder::getStreamIndex() { return streamIndex; }

AVStream *MediaDecoder::getStream() { return stream; }

AVCodecContext *MediaDecoder::getCodecContext() { return codecContext; }

int MediaDecoder::getMemorySize() { return packetQueue->getSize(); }

int MediaDecoder::hasEnoughPackets() {
    return streamIndex < 0 || (packetQueue == nullptr) ||
           packetQueue->isAbort() ||
           (stream->disposition & AV_DISPOSITION_ATTACHED_PIC) ||
           ((packetQueue->getPacketSize() > MIN_FRAMES) &&
            (!packetQueue->getDuration() ||
             av_q2d(stream->time_base) * packetQueue->getDuration() > 1.0));
}

bool MediaDecoder::isFinished() {
    return finished == packetQueue->getLastSeekSerial() && getPacketSize() == 0;
}

void MediaDecoder::run() {
    // do nothing
}

void MediaDecoder::pushNullPacket() {
    AVPacket pkt1, *pkt = &pkt1;
    av_init_packet(pkt);
    pkt->data = nullptr;
    pkt->size = 0;
    pkt->stream_index = streamIndex;
    pushPacket(pkt);
}

bool MediaDecoder::isSamePacketSerial() {
    return packetQueue->getLastSeekSerial() == packetQueue->getFirstSeekSerial();
}

PacketQueue *MediaDecoder::getPacketQueue() { return packetQueue; }

void MediaDecoder::setStartPts(int64_t startPts) {
    MediaDecoder::startPts = startPts;
}

void MediaDecoder::setStartPtsTb(const AVRational &startPtsTb) {
    MediaDecoder::startPtsTb = startPtsTb;
}
