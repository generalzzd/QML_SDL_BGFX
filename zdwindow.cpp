#include "zdwindow.h"

#include <stdio.h>
#include <iostream>
using namespace std;

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include "logo.h"

const static uint32_t width = 800;
const static uint32_t height = 600;

static void* sdlNativeWindowHandle(SDL_Window* _window)
{
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(_window, &wmi) )
    {
        return NULL;
    }

#	if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#		if ENTRY_CONFIG_USE_WAYLAND
    wl_egl_window *win_impl = (wl_egl_window*)SDL_GetWindowData(_window, "wl_egl_window");
    if(!win_impl)
    {
        int width, height;
        SDL_GetWindowSize(_window, &width, &height);
        struct wl_surface* surface = wmi.info.wl.surface;
        if(!surface)
            return nullptr;
        win_impl = wl_egl_window_create(surface, width, height);
        SDL_SetWindowData(_window, "wl_egl_window", win_impl);
    }
    return (void*)(uintptr_t)win_impl;
#		else
    return (void*)wmi.info.x11.window;
#		endif
#	elif BX_PLATFORM_OSX
    return wmi.info.cocoa.window;
#	elif BX_PLATFORM_WINDOWS
    return wmi.info.win.window;
#	endif // BX_PLATFORM_
}

inline bool sdlSetWindow(SDL_Window* _window)
{
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(_window, &wmi) )
    {
        return false;
    }

    bgfx::PlatformData pd;
#	if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#		if ENTRY_CONFIG_USE_WAYLAND
    pd.ndt          = wmi.info.wl.display;
#		else
    pd.ndt          = wmi.info.x11.display;
#		endif
#	elif BX_PLATFORM_OSX
    pd.ndt          = NULL;
#	elif BX_PLATFORM_WINDOWS
    pd.ndt          = NULL;
#	endif // BX_PLATFORM_
    pd.nwh          = sdlNativeWindowHandle(_window);

    pd.context      = NULL;
    pd.backBuffer   = NULL;
    pd.backBufferDS = NULL;
    bgfx::setPlatformData(pd);

    return true;
}

ZDWindow::ZDWindow(QObject *parent) : QObject(parent)
{

}

bool ZDWindow::openGLWindow()
{
    cout << "Hhhh"<<endl;
    SDL_Init(SDL_INIT_VIDEO| SDL_INIT_GAMECONTROLLER);

    SDL_Window* window =
            SDL_CreateWindow(
                "bgfx",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                800, 600,
                SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
                );
    sdlSetWindow(window);
    bgfx::renderFrame();
    //---bgfx------
    bgfx::Init init;
    init.type = bgfx::RendererType::OpenGL;
    init.platformData.nwh = sdlNativeWindowHandle(window);//wmi.info.cocoa.window;
    init.resolution.width = width;
    init.resolution.height = height;
    init.resolution.reset = BGFX_RESET_VSYNC;
    if (!bgfx::init(init))
        return 1;
    // Set view 0 to the same dimensions as the window and to clear the color buffer.
    const bgfx::ViewId kClearView = 0;
    bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR);
    bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
    //--------------
    SDL_Event event;
    bool exit = false;
    while (!exit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                exit = true;
                break;

            case SDL_WINDOWEVENT: {
                const SDL_WindowEvent& wev = event.window;
                switch (wev.event) {
                case SDL_WINDOWEVENT_RESIZED:
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                    break;

                case SDL_WINDOWEVENT_CLOSE:
                    exit = true;
                    break;
                }
            } break;
            }
        }

        bgfx::touch(kClearView);
        // Use debug font to print information about this example.
        bgfx::dbgTextClear();
        bgfx::dbgTextImage(bx::max<uint16_t>(uint16_t(width / 2 / 8), 20) - 20, bx::max<uint16_t>(uint16_t(height / 2 / 16), 6) - 6, 40, 12, s_logo, 160);
        bgfx::dbgTextPrintf(0, 0, 0x0f, "Press F1 to toggle stats.");
        bgfx::dbgTextPrintf(0, 1, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");
        bgfx::dbgTextPrintf(80, 1, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
        bgfx::dbgTextPrintf(80, 2, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");
        const bgfx::Stats* stats = bgfx::getStats();
        bgfx::dbgTextPrintf(0, 2, 0x0f, "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters.", stats->width, stats->height, stats->textWidth, stats->textHeight);
        // Enable stats or debug text.
        bgfx::setDebug(false ? BGFX_DEBUG_STATS : BGFX_DEBUG_TEXT);
        // Advance to next frame. Process submitted rendering primitives.
        bgfx::frame();
    }
    bgfx::shutdown();
    SDL_DestroyWindow(window);
    SDL_Quit();
    return true;
}
