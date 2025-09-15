#include "MainComponent.h"

MainComponent::MainComponent()
{
    scene.initialize (context);
    setSize (800, 600);
}

void MainComponent::renderFrame()
{
    scene.render (context, texture);
}
