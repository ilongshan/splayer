#include <decoder/MediaDecoder.h>

MediaDecoder::MediaDecoder(AVCodecContext *codecContext, AVStream *stream, int streamIndex, PlayerState *playerState,
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
    mutex.lock();
    if (packetQueue) {
        packetQueue->flush();
        delete packetQueue;
        packetQueue = nullptr;
    }
    if (codecContext) {
        avcodec_close(codecContext);
        avcodec_free_context(&codecContext);
        codecContext = nullptr;
    }
    playerState = nullptr;
    mutex.unlock();
}

void MediaDecoder::start() {
    if (packetQueue) {
        packetQueue->start();
    }
    mutex.lock();
    abortRequest = false;
    condition.signal();
    mutex.unlock();
}

void MediaDecoder::stop() {
    mutex.lock();
    abortRequest = true;
    condition.signal();
    mutex.unlock();
    if (packetQueue) {
        packetQueue->abort();
    }
}


void MediaDecoder::pushFlushPacket() {
    if (packetQueue) {
        packetQueue->pushPacket(flushPacket);
    }
}

void MediaDecoder::flush() {
    ALOGD(TAG, "%s", __func__);
    if (packetQueue) {
        packetQueue->flush();
    }
    // 定位时，音视频均需要清空缓冲区
    playerState->mMutex.lock();
    avcodec_flush_buffers(getCodecContext());
    playerState->mMutex.unlock();
}

int MediaDecoder::pushPacket(AVPacket *pkt) {
    if (packetQueue) {
        int ret = packetQueue->pushPacket(pkt);
        return ret;
    }
    return SUCCESS;
}

int MediaDecoder::getPacketSize() {
    return packetQueue ? packetQueue->getPacketSize() : 0;
}

int MediaDecoder::getStreamIndex() {
    return streamIndex;
}

AVStream *MediaDecoder::getStream() {
    return stream;
}

AVCodecContext *MediaDecoder::getCodecContext() {
    return codecContext;
}

int MediaDecoder::getMemorySize() {
    return packetQueue ? packetQueue->getSize() : 0;
}

int MediaDecoder::hasEnoughPackets() {
    Mutex::Autolock lock(mutex);
    return streamIndex < 0 || (packetQueue == nullptr) ||
           (packetQueue->isAbort() != 0) ||
           (stream->disposition & AV_DISPOSITION_ATTACHED_PIC) ||
           ((packetQueue->getPacketSize() > MIN_FRAMES) &&
            (!packetQueue->getDuration() || av_q2d(stream->time_base) * packetQueue->getDuration() > 1.0));
}


bool MediaDecoder::isFinished() {
    return packetQueue && finished == packetQueue->getLastSeekSerial() && getPacketSize() == 0;
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
    return packetQueue && packetQueue->getLastSeekSerial() == packetQueue->getFirstSeekSerial();
}

PacketQueue *MediaDecoder::getPacketQueue() const {
    return packetQueue;
}


