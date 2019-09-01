#pragma once


namespace srf
{
namespace system
{
// initialize sdl2.
// automatically called when creating Application.
// can call manually if sdl2 needs to be initialized before Application.
class SRFInitializer
{
public:
    static void Init();
    static bool IsInit();
    static void Quit();
};


class Application
{
public:
    Application();
    virtual ~Application();
    
    /// application cannot be copied
    Application(Application const&) = delete;
    Application& operator=(Application const&) = delete;


    // run the application by take handle from user.
    void run();

    // pause running application and return handle to user.
    void pause();
    
    // resume from paused state and take handle from user again.
    void resume();

    // stop the application and release all resources.
    void stop();

    bool isIdle() const { return _state == AppState::Idle; }
    bool isRunning() const { return _state == AppState::Run; }
    bool isPaused() const { return _state == AppState::Pause; }
    bool isStoped() const { return _state == AppState::Stop; }

protected:

    /// application cycles
    /// [Idle]  -- run()    --> [Run]   : onInit -- onBegin -- onLoop(keep calling)
    /// [Run]   -- pause()  --> [Pause] : onPaused
    /// [Pause] -- resume() --> [Run]   : onBegin -- onLoop(keep calling)
    /// [Run]   -- stop()   --> [Stop]  : onPaused -- onStop
    /// [Pause] -- stop()   --> [Stop]  : onStop
    ///
    /// onInit --+-- onBegin --+-- onLoop --+-- onPaused --+-- onStop
    ///          |             |------------|              |
    ///          |                                         |
    ///          +-----------------------------------------+
    ///

    virtual void onInit() { }
    virtual void onBegin() { }
    virtual void onLoop() { }
    virtual void onPaused() { }
    virtual void onStop() { }

private:
    void loop();

    enum class AppState
    {
        Idle,
        Run,
        Pause,
        Stop
    };

    AppState _state;
};

}
}



