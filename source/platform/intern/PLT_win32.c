#include "PLT_platform.h"

// Make sure we're on Windows
#if ACN_PLATFORM_WINDOWS

    #include "LOG_log.h"

    #include <windows.h>
    #include <windowsx.h>   // parameter input extraction
    #include <stdlib.h>

    typedef struct PLT_InternalState
    {
        HINSTANCE hInstance;
        HWND hWnd;
    } PLT_InternalState;

    // For the clock
    static float64 clockFrequency;
    static LARGE_INTEGER startTime;

    // Forward declare the message processor function
    LRESULT CALLBACK PLT_Win32MessageProcessor(HWND hWnd, uint32 message, WPARAM wParam, LPARAM lParam);

    // Call this at the start of the execution in order to
    // setup the platform layer.
    bool8 PLT_Init(PLT_PlatformState* platformState)
    {
        platformState->internalState = malloc(sizeof(PLT_InternalState));
        PLT_InternalState *state = (PLT_InternalState *)platformState->internalState;

        state->hInstance = GetModuleHandleA(0);

        // Let's fill out the class to tell Windows about our window
        // IDI_APPLICATION is the default Windows Application icon (the rectangular one!)
        HICON icon = LoadIcon(state->hInstance, IDI_APPLICATION);

        // Create an instance of a WindowClassA structure and initialise this to 0 
        // with memset
        WNDCLASSA windowClass;
        memset(&windowClass, 0, sizeof(windowClass));

        windowClass.style = CS_DBLCLKS;                         // Get double clicks
        windowClass.lpfnWndProc = PLT_Win32MessageProcessor;    // Callback function to handle events from Windows
        windowClass.cbClsExtra = 0;
        windowClass.cbWndExtra = 0;
        windowClass.hInstance = state->hInstance;
        windowClass.hIcon = icon;
        windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);      // NULL = manage the cursor manually
        windowClass.hbrBackground = NULL;                       // Transparent
        windowClass.lpszClassName = "acornWindowClass";

        // Now we've filled out the class - let's register it with Windows
        if(!RegisterClassA(&windowClass))
        {
            MessageBoxA(0, "Window registration failed", "Error", MB_ICONEXCLAMATION | MB_OK);
            return FALSE;
        }

        // Define the window
        uint32 windowX = 0;
        uint32 windowY = 0;
        uint32 windowWidth = 800;
        uint32 windowHeight = 600;

        // https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
        uint32 windowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
        // https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
        uint32 windowExStyle = WS_EX_APPWINDOW;

        windowStyle |= WS_MAXIMIZEBOX;
        windowStyle |= WS_MINIMIZEBOX;
        windowStyle |= WS_THICKFRAME;

        HWND handle = CreateWindowExA(
            windowExStyle,
            "acornWindowClass",
            "Acorn Engine",
            windowStyle,
            windowX,
            windowY,
            windowWidth,
            windowHeight,
            0,
            0,
            state->hInstance,
            0
        );

        if(handle == 0)
        {
            MessageBoxA(NULL, "Window creation failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);

            LOG_FATAL("Window creation failed!");
            return FALSE;
        }
        else
        {
            state->hWnd = handle;
        }

        // Finally show the window
        ShowWindow(state->hWnd, SW_SHOW);

        // Setup the clock
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        clockFrequency = 1.0 / (float64)frequency.QuadPart;
        QueryPerformanceCounter(&startTime);

        // Windows platform has been successfully initalised.
        return TRUE;
    }

    // Call this clean up after our platform
    void PLT_Terminate(PLT_PlatformState* platformState)
    {
        // Cold cast the platformState to PLT_InternalState
        PLT_InternalState *state = (PLT_InternalState *)platformState;

        // If we have a window ... destory it!
        if(state->hWnd)
        {
            DestroyWindow(state->hWnd);
            state->hWnd = 0;
        }
    }

    // Fetch the messages from the platform
    bool8 PLT_FetchMessages(PLT_PlatformState* platformState)
    {
        MSG message;

        while(PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }

        return TRUE;
    }

    // Allocate memory
    void* PLT_AllocateMemory(uint64 size, bool8 aligned)
    {
        return malloc(size);
    }

    // Free memory
    void PLT_FreeMemory(void* block, bool8 aligned)
    {
        free(block);
    }

    // Zero memory
    void* PLT_ZeroMemory(void* block, uint64 size)
    {
        return memset(block, 0, size);
    }

    // Copy memory
    void* PLT_CopyMemory(void* destination, const void* source, uint64 size)
    {
        return memcpy(destination, source, size);
    }

    // Set memory
    void* PLT_SetMemory(void* destination, int32 value, uint64 size)
    {
        return memset(destination, value, size);
    }

    // Function to write to the console
    void PLT_ConsoleOut(const char* message, uint8 colour)
    {
        // Colour the message
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        // FATAL, ERROR, WARN, INFO, DEBUG, TRACE
        static uint8 levels[6] = {64, 4, 6, 2, 1, 8};
        SetConsoleTextAttribute(consoleHandle, levels[colour]);

        // Output the message to the console and debug console
        OutputDebugStringA(message);
        uint64 length = strlen(message);
        LPDWORD numberWritten = 0;
        WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message, (DWORD)length, numberWritten, 0);
    }

    // Seperate function to write out as an error (just in case we want seperate stream for errors)
    void PLT_ConsoleOutError(const char* message, uint8 colour)
    {
        // Colour the message
        HANDLE consoleHandle = GetStdHandle(STD_ERROR_HANDLE);
        // FATAL, ERROR, WARN, INFO, DEBUG, TRACE
        static uint8 levels[6] = {64, 4, 6, 2, 1, 8};
        SetConsoleTextAttribute(consoleHandle, levels[colour]);

        // Output the message to the console and debug console
        OutputDebugStringA(message);
        uint64 length = strlen(message);
        LPDWORD numberWritten = 0;
        WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), message, (DWORD)length, numberWritten, 0);
    }

    // Get the time according to the platform we're on
    float64 PLT_GetAbsoluteTime()
    {
        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);
        return (float64)currentTime.QuadPart * clockFrequency;
    }

    // Call this function to tell the current thread to sleep for the provided number of ms
    void PLT_Sleep(uint64 ms)
    {
        Sleep(ms);
    }

    LRESULT CALLBACK PLT_Win32MessageProcessor(HWND hWnd, uint32 message, WPARAM wParam, LPARAM lParam)
    {
        switch(message)
        {
            case WM_ERASEBKGND:
            {
                // The application will be handling erasing so let Windows know we've got it
                return 1;
            }
            break;

            case WM_CLOSE:
            {
                // TODO: Close the application
                return 0;
            }
            break;

            case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            }
            break;

            case WM_SIZE:
            {
                // Get the updated size of the window
                //RECT rectangle;

                //GetClientRect(hWnd, &rectangle);
                //uint32 width = rectangle.right - rectangle.left;
                //uint32 height = rectangle.bottom - rectangle.top;

                // TODO: We'll now need to fire an event to tell the engine to resize the window
            }
            break;

            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
            case WM_KEYUP:
            case WM_SYSKEYUP:
            {
                // Is this a key press or release?
                // bool8 pressed = (message == WM_KEYDOWN || WM_SYSKEYDOWN);
                // TODO: Handle input processing
            }
            break;

            case WM_MOUSEMOVE:
            {
                // Get Mouse X and Y co-ordinates
                //int32 xMousePosition = GET_X_LPARAM(lParam);
                //int32 yMousePosition = GET_Y_LPARAM(lParam);
                // TODO: Handle the mouse input processing
            }
            break;

            case WM_MOUSEWHEEL:
            {
                //int32 mouseScrolled = GET_WHEEL_DELTA_WPARAM(wParam);

                // This will set mouseScrolled to 
                //  * 0 if it has not been touched
                //  * -1 if it has been scrolled down
                //  * 1 if it has been scrolled up
               // if(mouseScrolled != 0)
                //{
                   // mouseScrolled = (mouseScrolled < 0) ? -1 : 1;
                    // TODO: Handle the scroll wheel input processing 
               // }
            }
            break;

            case WM_LBUTTONDOWN:
            case WM_MBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_MBUTTONUP:
            case WM_RBUTTONUP:
            {
                //bool8 pressed = (message == WM_LBUTTONDOWN || WM_MBUTTONDOWN || WM_RBUTTONDOWN);
                // TODO: Handle mouse button click input processing
            }
            break;
        }

        return DefWindowProcA(hWnd, message, wParam, lParam);
    }

#endif