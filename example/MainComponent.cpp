#include "MainComponent.h"

MainComponent::MainComponent()
{
    webgpuGraphics = std::make_unique<WebGPUGraphics>();

    statusLabel.setText ("Initializing WebGPU...", juce::dontSendNotification);
    statusLabel.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (statusLabel);

    setSize (800, 600);

    isInitialized = webgpuGraphics->initialize (getWidth(), getHeight());
    if (isInitialized)
    {
        statusLabel.setText ("WebGPU initialized successfully!", juce::dontSendNotification);
        isInitialized = true;
        // Start timer for continuous rendering
        startTimer (16); // ~60 FPS
    }
    else
    {
        statusLabel.setText ("Failed to initialize WebGPU", juce::dontSendNotification);
    }
}

void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    if (! renderedImage.isNull())
        g.drawImage (renderedImage, getLocalBounds().toFloat());
}

void MainComponent::resized()
{
    statusLabel.setBounds (getLocalBounds().removeFromTop (30));
    if (isInitialized)
        webgpuGraphics->resize (getWidth(), getHeight());
}

void MainComponent::timerCallback()
{
    renderedImage = webgpuGraphics->renderFrameToImage();
}
