#include "SquareWaveMix.h"
#include <iostream>

SquareWaveMix::SquareWaveMix()
    // Initailises variables
    : sampleRate_(44100.0), numChannels_(2), mix_(0.0), phaseIncrement_(0.0)
{
    // resizes the vector
    currentPhases_.resize(static_cast<IndexType>(numChannels_), 0.0);
}

void SquareWaveMix::reset(double sampleRate, int numChannels)
{
    // Defensive programming, if the value is out side of this range it will flag up
    jassert(numChannels > 0 && "Number of channels must be greater than 0!");

    // resets variables
    sampleRate_ = sampleRate;
    numChannels_ = numChannels;
    currentPhases_.resize(static_cast<IndexType>(numChannels_), 0.0);
}

void SquareWaveMix::setFrequency(float value)
{
    // Defensive programming, if the value is out side of this range it will flag up
    jassert(value >= 0.0f && value <= 10.0f && "Frequency value is out of range!");

    // Map the frequency range
    float frequency = juce::jmap(value, 0.0f, 10.0f, 10.0f, 200.0f);

    // Updates phase
    phaseIncrement_ = 2.0f * juce::MathConstants<double>::pi * frequency / sampleRate_;
}

void SquareWaveMix::setMix(float mixRate)
{
    // Defensive programming, if the value is out side of this range it will flag up
    jassert(mixRate >= 0.0f && mixRate <= 1.0f && "mixRate value is out of range!");

    mix_ = juce::jlimit(0.0f, 1.0f, mixRate);
}

float SquareWaveMix::generateSquareWave(double& currentPhase)
{
    return (currentPhase < juce::MathConstants<double>::pi) ? 1.0f : -1.0f;
}

void SquareWaveMix::process(juce::AudioBuffer<float>& buffer)
{
    for (int channel = 0; channel < numChannels_; ++channel)
    {
        // This allows you to write information into the buffer
        float* const bufferWrite = buffer.getWritePointer(channel);

        /* Note to self, every time you acess a vector, list, array there is a small 
        computational cost. If you access a container multiple times it can add up
        by having the variable declared outside of the loop it speeds up the code 
        especially for stereo channels*/
        
        double localPhase = currentPhases_[static_cast<IndexType>(channel)];

        for (int samples = 0; samples < buffer.getNumSamples(); ++samples)
        {
            // Generating the square wave for the wet signal
            float squareWave = generateSquareWave(localPhase);

            // Updates the phase
            localPhase += phaseIncrement_;
            while (localPhase >= 2.0 * juce::MathConstants<double>::pi)
                localPhase -= 2.0 * juce::MathConstants<double>::pi;

            // Generating the sine wave for the dry signal
            float sineWave = static_cast<float>(std::sin(localPhase));

            /* If you used bufferWrite[samples], when you load an audio file
            it will use that audio file */
            float drySignal = (1.0f - mix_) * bufferWrite[samples] /*bufferWrite[samples]*/;

            float wetSignal = mix_ * squareWave;

            bufferWrite[samples] = drySignal + wetSignal;
        }

        // This resets the phase for each time process is called 
        currentPhases_[static_cast<IndexType>(channel)] = localPhase;
    }
}