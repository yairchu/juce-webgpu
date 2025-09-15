#include "MainComponent.h"

MainComponent::MainComponent()
{
    webgpuGraphics = std::make_unique<WebGPUGraphics>();

    statusLabel.setText ("Initializing WebGPU...", juce::dontSendNotification);
    statusLabel.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (statusLabel);

    setSize (800, 600);

    std::thread ([this]()
                 {
        bool success = webgpuGraphics->initialize(getWidth(), getHeight());
        juce::MessageManager::callAsync([this, success]() {
            if (success) {
                statusLabel.setText("WebGPU initialized successfully!", juce::dontSendNotification);
                isInitialized = true;
                // Start timer for continuous rendering
                startTimer (16); // ~60 FPS
            } else {
                statusLabel.setText("Failed to initialize WebGPU", juce::dontSendNotification);
            }
        }); })
        .detach();
}

MainComponent::~MainComponent()
{
    // Stop the timer first to prevent new render calls
    stopTimer();

    isInitialized = false;

    if (webgpuGraphics)
    {
        webgpuGraphics->shutdown();
    }
}

void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    if (isInitialized && ! renderedImage.isNull())
    {
        // Draw the WebGPU rendered image
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
    // Check both flags to prevent rendering during shutdown
    if (isInitialized)
        renderGraphics();
}

void MainComponent::renderGraphics()
{
    // Render frame on background thread to avoid blocking UI
    std::thread ([this]()
                 {
        juce::Image newImage = webgpuGraphics->renderFrameToImage();
        
        juce::MessageManager::callAsync([this, newImage]() {
            renderedImage = newImage;
            repaint();
        }); })
        .detach();
}
