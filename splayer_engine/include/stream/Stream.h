#ifndef SPLAYER_MAC_DEMO_STREAM_H
#define SPLAYER_MAC_DEMO_STREAM_H

class MediaPlayer;

#include <player/MediaPlayer.h>
#include <common/Thread.h>
#include <player/PlayerState.h>
#include <decoder/AudioDecoder.h>
#include <decoder/VideoDecoder.h>
#include <sync/MediaSync.h>
#include <stream/IStreamListener.h>

class Stream : public Runnable {

    const char *const TAG = "Stream";

    const char *const OPT_SCALL_ALL_PMTS = "scan_all_pmts";

    const char *const OPT_HEADERS = "headers";

    const char *const FORMAT_OGG = "ogg";

private:
    /// 读数据包线程
    Thread *readThread = nullptr;

    /// 播放器
    MediaPlayer *mediaPlayer = nullptr;

    /// 播放器状态
    PlayerState *playerState = nullptr;

    /// 音频解码器
    AudioDecoder *audioDecoder = nullptr;

    /// 视频解码器
    VideoDecoder *videoDecoder = nullptr;

    /// 媒体同步器
    MediaSync *mediaSync = nullptr;

    /// 解码上下文
    AVFormatContext *formatContext = nullptr;

    /// 刷新的包,用于在SEEK时，刷新数据队列
    AVPacket flushPacket;

    /// 文件总时长
    int64_t duration;

    IStreamListener *streamListener = nullptr;

    Mutex waitMutex;

    Condition waitCondition;

public:

    Stream(MediaPlayer *mediaPlayer, PlayerState *playerState);

    ~Stream() override;

    void run() override;

    int start();

    int stop();

    void setAudioDecoder(AudioDecoder *audioDecoder);

    void setVideoDecoder(VideoDecoder *videoDecoder);

    void setMediaSync(MediaSync *mediaSync);

    Mutex *getWaitMutex();

    Condition *getWaitCondition();

    AVPacket *getFlushPacket();

    void setStreamListener(IStreamListener *streamListener);

private:

    int readPackets();

    bool isRetryPlay() const;

    bool isPacketInPlayRange(const AVFormatContext *formatContext, const AVPacket *packet) const;

    int openStream();

    int notifyMsg(int what);

    int notifyMsg(int what, int arg1);

    int notifyMsg(int what, int arg1, int arg2);

    void closeStream();
};


#endif //SPLAYER_MAC_DEMO_STREAM_H