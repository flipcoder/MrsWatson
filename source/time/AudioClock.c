//
// AudioClock.c - MrsWatson
// Copyright (c) 2016 Teragon Audio. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#include "AudioClock.h"

#include <stdio.h>
#include <stdlib.h>

AudioClock audioClockInstance = NULL;

void initAudioClock(void) {
  audioClockInstance = (AudioClock)malloc(sizeof(AudioClockMembers));
  audioClockInstance->currentFrame = 0;
  audioClockInstance->transportChanged = false;
  audioClockInstance->isPlaying = false;
}

AudioClock getAudioClock(void) { return audioClockInstance; }

void advanceAudioClock(AudioClock self, const unsigned long blocksize) {
  if (self->currentFrame == 0 || !self->isPlaying) {
    self->transportChanged = true;
    self->isPlaying = true;
  } else {
    self->transportChanged = false;
  }

  self->currentFrame += blocksize;
}

void audioClockStop(AudioClock self) {
  self->isPlaying = false;
  self->transportChanged = true;
}

static inline double _samplesPerBeat(const Tempo tempo,
                                     const SampleRate sampleRate) {
  return (60.0 / tempo) * sampleRate;
}

double audioClockSamplesToPpq(const SampleCount samples, const Tempo tempo,
                              const SampleRate sampleRate) {
  // Musical time starts with 1, not 0
  return (samples / _samplesPerBeat(tempo, sampleRate)) + 1.0;
}

SampleCount audioClockPpqToSamples(const double ppq, const Tempo tempo,
                                   const SampleRate sampleRate) {
  return (SampleCount)(_samplesPerBeat(tempo, sampleRate) * (ppq - 1.0));
}

void freeAudioClock(AudioClock self) {
  if (self != NULL) {
    free(self);
    audioClockInstance = NULL;
  }
}
