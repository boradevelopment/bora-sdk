// BaseTest.h
#pragma once

class BaseTest {
public:
    virtual ~BaseTest() {}

    // Name of the test
    virtual const char* getName() const = 0;

    // Called once at startup
    virtual void onInit() = 0;

    // Called every frame
    virtual void onUpdate(float deltaTime) = 0;

    // Called every frame to render visuals
    virtual void onRender() = 0;

    // Called on shutdown
    virtual void onShutdown() = 0;
};
