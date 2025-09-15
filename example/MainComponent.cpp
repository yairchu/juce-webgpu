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

    if (isInitialized && ! renderedImage.isNull())
    {
        g.drawImage (renderedImage, getLocalBounds().toFloat());
    }
    else
    {
        g.setColour (juce::Colours::white);
        g.setFont (20.0f);
        g.drawText ("JUCE WebGPU Graphics Example", getLocalBounds().removeFromTop (60), juce::Justification::centred, true);
    }
}

void MainComponent::resized()
{
    auto bounds = getLocalBounds();

    // Position status label at top
    auto statusArea = bounds.removeFromTop (30);
    statusLabel.setBounds (statusArea);

    // Resize WebGPU graphics if initialized
    if (isInitialized)
    {
        webgpuGraphics->resize (getWidth(), getHeight() - 30); // Account for status label
    }
}

void MainComponent::timerCallback()
{
    renderedImage = webgpuGraphics->renderFrameToImage();
}
