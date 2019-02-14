//
// Created by biezhihua on 2019/2/4.
//

#include "SPlayer.h"

void *startDecodeAudioFrameCallback(void *data) {
    SPlayer *sPlayer = (SPlayer *) data;
    if (sPlayer != NULL) {
        SFFmpeg *pSFFmpeg = sPlayer->getSFFmpeg();
        SStatus *pPlayerStatus = sPlayer->getPlayerStatus();
        if (pSFFmpeg != NULL && pPlayerStatus != NULL) {
            while (!pPlayerStatus->isDestroy()) {
                int result = pSFFmpeg->decodeAudioFrame();
                if (result == S_SUCCESS) {

                } else if (result == S_ERROR_BREAK) {
                    LOGE("SPlayer: decodeMediaInfo error");
                    while (!pPlayerStatus->isDestroy()) {
                        SMedia *pAudio = pSFFmpeg->getAudio();
                        if (pAudio == NULL) {
                            break;
                        }
                        if (pAudio->getQueueSize() > 0) {
                            continue;
                        } else {
                            // pStatus->changeStateToExit();
                            break;
                        }
                    }
                }
            }
        }
        pthread_exit(&sPlayer->startDecodeAudioThread);
    }
    return NULL;
}

void *startDecodeMediaInfoCallback(void *data) {
    SPlayer *sPlayer = (SPlayer *) data;
    if (sPlayer != NULL) {
        if (sPlayer->getSFFmpeg() != NULL) {
            SFFmpeg *pSFFmpeg = sPlayer->getSFFmpeg();
            SStatus *pSStatus = sPlayer->getPlayerStatus();
            int result = pSFFmpeg->decodeMediaInfo();
            if (result == S_SUCCESS) {
                pSStatus->moveStatusToStart();
                sPlayer->getSJavaMethods()->onCallJavaStart();
            } else {
                // TODO Release
                pSStatus->moveStatusToStop();
                // TODO ERROR
                LOGE("SPlayer:  startDecodeMediaInfoCallback error");
            }
        }
        pthread_exit(&sPlayer->startDecodeMediaInfoThread);
    }
    return NULL;
}

void *playAudioCallback(void *data) {
    LOGD("SPlayer: playAudioCallback called");
    SPlayer *sPlayer = (SPlayer *) data;
    if (sPlayer != NULL && sPlayer->getSFFmpeg() != NULL) {
        SFFmpeg *pSFFmpeg = sPlayer->getSFFmpeg();
        SStatus *pPlayerStatus = sPlayer->getPlayerStatus();
        SOpenSLES *pSOpenSLES = sPlayer->getSOpenSLES();
        if (pPlayerStatus != NULL &&
            pSFFmpeg != NULL &&
            pSOpenSLES != NULL &&
            pPlayerStatus->isLeastActiveState(STATE_PRE_PLAY)) {

//            while (pPlayerStatus->isLeastActiveState(STATE_PLAY)) {
//                int result = pSFFmpeg->resampleAudio();
//                LOGD("playAudioCallback result = %d", result);
//                if (result == S_ERROR_BREAK) {
//                    // TODO Release
//                    pPlayerStatus->moveStatusToStop();
//                    break;
//                } else if (result == S_ERROR_CONTINUE) {
//                    continue;
//                } else if (result >= 0) {
//
//                }
//            }

            LOGD("SPlayer: playAudioCallback called: Init OpenSLES");
            pSOpenSLES->createEngine();
            pSOpenSLES->createBufferQueueAudioPlayer();
            pSOpenSLES->play();

            pPlayerStatus->moveStatusToPlay();
        }
        pthread_exit(&sPlayer->playAudioThread);
    }
    return NULL;
}

SPlayer::SPlayer(JavaVM *pVm, JNIEnv *env, jobject instance, SJavaMethods *pMethods) {
    pJavaVM = pVm;
    mainJniEnv = env;
    javaInstance = mainJniEnv->NewGlobalRef(instance);
    pJavaMethods = pMethods;

    create();
}

SPlayer::~SPlayer() {

    destroy();

    pJavaMethods = NULL;
    mainJniEnv->DeleteGlobalRef(javaInstance);
    javaInstance = NULL;
    mainJniEnv = NULL;
    pJavaVM = NULL;
}


void SPlayer::create() {
    pStatus = new SStatus();
    pSFFmpeg = new SFFmpeg();
    pOpenSLES = new SOpenSLES();

    if (pStatus != NULL) {
        pStatus->moveStatusToCreate();
    }
    if (pJavaMethods != NULL) {
        pJavaMethods->onCallJavaCreate();
    }
}


void SPlayer::setSource(string *url) {
    if (pStatus->isCreate() || pStatus->isStop()) {
        pSource = url;
        if (pSFFmpeg != NULL) {
            pSFFmpeg->setSource(url);
        }
        pStatus->moveStatusToSource();
    }
}

void SPlayer::start() {
    if (pStatus->isSource() || pStatus->isStop()) {
        pthread_create(&startDecodeMediaInfoThread, NULL, startDecodeMediaInfoCallback, this);
        pStatus->moveStatusToStart();
    }
}

void SPlayer::play() {
    if (pStatus->isStart() || pStatus->isPause()) {
        pStatus->moveStatusToPrePlay();
//        pthread_create(&startDecodeAudioThread, NULL, startDecodeAudioFrameCallback, this);
        pthread_create(&playAudioThread, NULL, playAudioCallback, this);
    }
}

void SPlayer::pause() {
    if (pStatus->isPlay()) {
        pStatus->moveStatusToPause();
    }
}

void SPlayer::stop() {
    if (pStatus->isLeastActiveState(STATE_SOURCE)) {
        pStatus->moveStatusToStop();
    }
}

void SPlayer::destroy() {
    delete pOpenSLES;
    pOpenSLES = NULL;

    delete pSFFmpeg;
    pSFFmpeg = NULL;

    if (pJavaMethods != NULL) {
        pJavaMethods->onCallJavaDestroy();
    }

    if (pStatus != NULL) {
        pStatus->moveStatusToDestroy();
    }

    delete pStatus;
    pStatus = NULL;

    pSource = NULL;
}

SFFmpeg *SPlayer::getSFFmpeg() {
    return pSFFmpeg;
}

SJavaMethods *SPlayer::getSJavaMethods() {
    return pJavaMethods;
}


SStatus *SPlayer::getPlayerStatus() {
    return pStatus;
}

SOpenSLES *SPlayer::getSOpenSLES() {
    return pOpenSLES;
}


