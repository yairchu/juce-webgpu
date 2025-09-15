#include "MainComponent.h"

MainComponent::MainComponent()
{
    statusLabel.setText ("Initializing WebGPU...", juce::dontSendNotification);
    statusLabel.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (statusLabel);

    setSize (800, 600);

    isInitialized = scene.initialize (context);
    if (isInitialized)
    {
        statusLabel.setText ("WebGPU initialized successfully!", juce::dontSendNotification);
    }
    else
    {
        statusLabel.setText ("Failed to initialize WebGPU", juce::dontSendNotification);
    }
}

void MainComponent::renderFrame()
{
    if (isInitialized)
        scene.render (context, texture);
}

void MainComponent::paint (juce::Graphics& g)
{
    WebGPUComponent::paint (g);
}

void MainComponent::resized()
{
    WebGPUComponent::resized();
    statusLabel.setBounds (getLocalBounds().removeFromTop (30));
}
