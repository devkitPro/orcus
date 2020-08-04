/*! \file audio.h
    \brief Audio
 */

#ifndef __ORCUS_AUDIO_H__
#define __ORCUS_AUDIO_H__

#include <stdint.h>

/**
   @brief Start AC97 codec.

   Start AC97 codec, will restart codec if it was previously started.

   @note DMA channels must have been set up before calling this.
 */
extern void ac97Start();

/**
   @brief Stop AC97 codec.

   Stop AC97 codec.
 */
extern void ac97Stop();

/**
   @brief Read register value from AC97 codec.

   Read register value from AC97 codec

   @note Must have called ac97Start() or audioInit() before using this.

   @param reg Register to read
   @return Register value
   @see ac97Start
   @see audioInit
 */
extern uint16_t ac97GetReg(uint8_t reg);

/**
   @brief Set register value in AC97 codec.

   Set register value in AC97 codec.

   @note Must have called ac97Start()  or audioInit() before using this.

   @param reg Register to set
   @param value Value to set
   @see ac97Start
   @see audioInit
*/
extern void ac97SetReg(uint8_t reg, uint16_t value);

/**
   @brief Initialise audio subsystem.

   Initialise auto subsystem.

   @param dmaChannel DMA channel to use for audio (0 - 15, recommend 0 - 3)
 */
extern void audioInit(int dmaChannel);

/**
   @brief Set sample rate for AC97 codec.
   
   Set sample rate for AC97 codec.

   @note Must have called audioInit() before using this.

   @param khz Sample rate to set
   @see ac97Start
   @see audioInit
 */
extern void audioSetSampleRate(uint16_t khz);

/**
   @brief Mute speakers.

   Mute speakers.
 */
extern void audioMuteSpeakers();

/**
   @brief Unmute speakers.

   Unmute speakers.
 */
extern void audioUnmuteSpeakers();

/**
   @brief Mute headphones.

   Mute headphones.
 */
extern void audioMuteHeadphones();

/**
   @brief Unmute headphones.

   Unmute headphones.
 */
extern void audioUnmuteHeadphones();

/**
   @brief Set volume.

   Set volume.

   @warn You may not wish to allow the volume to reach 31, with headphones connected this could do damage to somebody's hearing!

   @param left Volume level for left channel (0 [lowest] - 31 [highest])
   @param right Volume level for right channel (0 [lowest] - 31 [highest])
 */
extern void audioSetVolume(uint8_t left, uint8_t right);

/**
   @brief Play a PCM sample.

   Play a PCM sample. Non-blocking, see if the sample has finished playing with audioSamplePlaying().

   @param bytes Number of bytes in sample (max 0xFFFF)
   @param data Pointer to sample data
   @see audioSamplePlaying
 */
extern void audioPlaySample(uint16_t bytes, void* data);

/**
   @brief Check if a sample is currently playing.

   Check if a sample is currently playing.

   @return true if a sample is currently playing, false otherwise
 */
extern bool audioSamplePlaying();

/**
   @brief Check if headphones are connected (F100 only).

   Check if headphones are connected. This works on the F100 only, the F200 physically switches the DAC output between the speakers and headphones when you insert the jack.

   @note F100 only
   @return true if headphones are connected, false otherwise
 */
extern bool audioHeadphonesConnected();

#endif
