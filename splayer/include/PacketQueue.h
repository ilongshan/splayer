//
// Created by biezhihua on 2019-07-16.
//

#ifndef ANDROID_SPLAYER_PACKETQUEUE_H
#define ANDROID_SPLAYER_PACKETQUEUE_H

#include "MyAVPacketList.h"
#include "Mutex.h"

#include "Thread.h"
#include "FFPlay.h"

extern "C" {
#include <libavutil/time.h>
#include <libavutil/rational.h>
#include <libavutil/mem.h>
#include <libavutil/log.h>
#include <libavutil/error.h>
#include <libavutil/frame.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
};

class PacketQueue {
public:
    MyAVPacketList *first_pkt, *last_pkt;
    int nb_packets;
    int size;
    int64_t duration;
    int abort_request;
    int serial;
    Mutex *mutex;
};


#endif //ANDROID_SPLAYER_PACKETQUEUE_H
