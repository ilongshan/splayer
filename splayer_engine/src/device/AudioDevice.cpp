
#include <device/AudioDevice.h>

#include "device/AudioDevice.h"

AudioDevice::AudioDevice() {

}

AudioDevice::~AudioDevice() {

}

int AudioDevice::open(AudioDeviceSpec *desired, AudioDeviceSpec *obtained) {
    return 0;
}

void AudioDevice::start() {

}

void AudioDevice::stop() {
    if (DEBUG) {
        ALOGD(TAG, "[%s]", __func__);
    }
}

void AudioDevice::pause() {

}

void AudioDevice::resume() {

}

void AudioDevice::flush() {

}

void AudioDevice::setStereoVolume(float left_volume, float right_volume) {

}

void AudioDevice::run() {
    // do nothing
}

int AudioDevice::create() {
    return SUCCESS;
}

void AudioDevice::destroy() {

}
