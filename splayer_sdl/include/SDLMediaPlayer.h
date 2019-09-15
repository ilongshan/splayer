#ifndef SPLAYER_MAC_MACMEDIAPLAYER_H
#define SPLAYER_MAC_MACMEDIAPLAYER_H

#include <player/MediaPlayer.h>
#include <message/MessageCenter.h>
#include <message/IMessageListener.h>
#include <sync/MediaSync.h>
#include <SDL.h>
#include <SDLVideoDevice.h>

class SDLMediaPlayer : public MediaPlayer {

    const char *const TAG = "SDLMediaPlayer";

private:

    bool quit = false;

    int64_t lastMouseLeftClick = 0;

    void doKeySystem(const SDL_Event &event);

    bool isNotHaveWindow() const;

    bool isQuitKey(const SDL_Event &event) const;

public:

    class Builder;

    int eventLoop();

    void doWindowEvent(const SDL_Event &event);

    void showCursor() const;

    void hideCursor() const;

    int isFullScreenClick();

    void doExit();

    void doSeek(double increment) const;

    int destroy() override;

    void resetRemainingTime();

    void refreshVideo();

    void toggleFullScreen();
};

class SDLMediaPlayer::Builder {
private:

    bool debug = true;
    MediaSync *mediaSync = nullptr;
    AudioDevice *audioDevice = nullptr;
    VideoDevice *videoDevice = nullptr;
    MessageCenter *messageCenter = nullptr;
    IMessageListener *messageListener = nullptr;

public:

    Builder &withDebug(bool debug) {
        Builder::debug = debug;
        return *this;
    }

    Builder &withMediaSync(MediaSync *sync) {
        this->mediaSync = sync;
        return *this;
    }

    Builder &withAudioDevice(AudioDevice *audioDevice) {
        this->audioDevice = audioDevice;
        return *this;
    }

    Builder &withVideoDevice(VideoDevice *videoDevice) {
        this->videoDevice = videoDevice;
        return *this;
    }

    Builder &withMessageCenter(MessageCenter *messageCenter) {
        this->messageCenter = messageCenter;
        return *this;
    }

    Builder &withMessageListener(IMessageListener *messageListener) {
        this->messageListener = messageListener;
        return *this;
    }

    SDLMediaPlayer *build() {
        DEBUG = debug;
        SDLMediaPlayer *mediaPlayer = new SDLMediaPlayer();
        mediaPlayer->setMediaSync(mediaSync);
        mediaPlayer->setAudioDevice(audioDevice);
        mediaPlayer->setVideoDevice(videoDevice);
        mediaPlayer->setMessageCenter(messageCenter);
        mediaPlayer->setMessageListener(messageListener);
        return mediaPlayer;
    }
};

#endif