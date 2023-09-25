#pragma once
#include <JuceHeader.h>

/* This class takes a square wave and mixes it with either a buffer or sine wave
when using the buffer, you need to connect it to an audio source to see it work. 
I used Juce's audio plugin host to test this.
*/
class SquareWaveMix
{
public:
    SquareWaveMix();

    // Resets the sampleRate, number of channels and phase
    void reset(double sampleRate, int numChannels);

    // Sets the frequency based on a slider value
    void setFrequency(float frequency);

    // Sets the mix rate based on a slider value
    void setMix(float mixRate);

    // Allows for the square wave to be mixed with either the buffer or sine wave
    void process(juce::AudioBuffer<float>& buffer);
    
private:
    // Helper function to generate square wave
    float generateSquareWave(double& currentPhase);

    // Samplerate
    double sampleRate_{0};

    // Number of channels in use
    int numChannels_{0};

    // The rate of the mixutre between square and sine
    float mix_{0.0f};

    // Vector of phases, previously with a single variable there was
    // phase issues in the stereo image
    std::vector<double> currentPhases_;

    // The Increment of phase
    double phaseIncrement_{0.0};

     // An alias to reduce the length of the code
    using IndexType = std::vector<double>::size_type;
};
