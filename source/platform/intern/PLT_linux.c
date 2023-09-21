#include "PLT_platform.h"

// Make sure we're on Linux
#if ACN_PLATFORM_LINUX

    #include "LOG_log.h"

    #include <xcb/xcb.h>

    #include <X11/keysym.h>
    #include <X11/XKBlib.h>     
    #include <X11/Xlib.h>
    #include <X11/Xlib-xcb.h>
    #include <sys/time.h>

    #if _POSIX_C_SOURCE >= 199309L
        #include <time.h>
    #else
        #include <unistd.h>
    #endif

    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>

    typedef struct PLT_InternalState
    {
        Display *display;
        xcb_connection_t *connection;
        xcb_window_t window;
        xcb_screen_t *screen;
        xcb_atom_t wmProtocols;
        xcb_atom_t wmDeleteWin;
    } PLT_InternalState;

    // Call this at the start of the execution in order to
    // setup the platform layer.
    bool8 PLT_Init(PLT_PlatformState* platformState)
    {
        // Create the internal state
        platformState->internalState = malloc(sizeof(PLT_InternalState));
        PLT_InternalState *state = (PLT_InternalState *)platformState->internalState;

        // Connect to the X server using XLib
        state->display = XOpenDisplay(NULL);

        // Turn off key repeats
        XAutoRepeatOff(state->display);

        // Retrieve the connection from the display
        state->connection = XGetXCBConnection(state->display);

        if(xcb_connection_has_error(state->connection))
        {
            LOG_FATAL("Failed to connect to the X server via XCB");
            return FALSE;
        }

        // Get some data from the X server
        const struct xcb_setup_t *setup = xcb_get_setup(state->connection);

        // Loop through the available screens on the system
        xcb_screen_iterator_t iterator = xcb_setup_roots_iterator(setup);
        int screenProperties = 0;

        for(int32 s = screenProperties; s > 0; s--)
        {
            xcb_screen_next(&iterator);
        }

        // Now we can assign the screen
        state->screen = iterator.data;

        // Assign an XID for the window so we can reference it later
        state->window = xcb_generate_id(state->connection);

        // Let Linux know which event types we want to be told about
        uint32 eventMask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;

        // Lets make sure we get the keyboard and mouse events
        uint32 eventValues =    XCB_EVENT_MASK_BUTTON_PRESS |
                                XCB_EVENT_MASK_BUTTON_RELEASE |
                                XCB_EVENT_MASK_KEY_PRESS |
                                XCB_EVENT_MASK_KEY_RELEASE |
                                XCB_EVENT_MASK_EXPOSURE |
                                XCB_EVENT_MASK_POINTER_MOTION |
                                XCB_EVENT_MASK_STRUCTURE_NOTIFY;

        // Values to be sent over to XCB (background colour and events)
        uint32 valueList[] = {state->screen->black_pixel, eventValues};

        // Create the window and get a cookie! (Nom, Nom, Nom!) 
        xcb_void_cookie_t cookie = xcb_create_window(
            state->connection,
            XCB_COPY_FROM_PARENT,           // copy from the root window
            state->window,
            state->screen->root,
            WINDOW_X,
            WINDOW_Y,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            0,
            XCB_WINDOW_CLASS_INPUT_OUTPUT,
            state->screen->root_visual,
            eventMask,
            valueList
        );

        // Set the title of the window
        xcb_change_property(
            state->connection,
            XCB_PROP_MODE_REPLACE,
            state->window,
            XCB_ATOM_WM_NAME,
            XCB_ATOM_STRING,
            8,                      // data needs to be viewed 8 bits at a time
            strlen(WINDOW_TITLE),
            WINDOW_TITLE
        );

        // We need to know when the server has decided to destroy our window (how rude!)
        xcb_intern_atom_cookie_t wmDeleteCookie = xcb_intern_atom(
            state->connection,
            0,
            strlen("WM_DELETE_WINDOW"),
            "WM_DELETE_WINDOW"
        );

        xcb_intern_atom_cookie_t wmProtocolsCookie = xcb_intern_atom(
            state->connection,
            0,
            strlen("WM_PROTOCOLS"),
            "WM_PROTOCOLS"
        );

        xcb_intern_atom_reply_t *wmDeleteReply = xcb_intern_atom_reply(
            state->connection,
            wmDeleteCookie,
            NULL
        );

        xcb_intern_atom_reply_t *wmProtocolsReply = xcb_intern_atom_reply(
            state->connection,
            wmProtocolsCookie,
            NULL
        );

        state->wmDeleteWin = wmDeleteReply->atom;
        state->wmProtocols = wmProtocolsReply->atom;

        xcb_change_property(
            state->connection,
            XCB_PROP_MODE_REPLACE,
            state->window,
            wmProtocolsReply->atom,
            4,
            32,
            1,
            &wmDeleteReply->atom
        );

        // Map the window to the screen
        xcb_map_window(state->connection, state->window);

        // Make sure we've recieved all the data (aka flush the stream)
        int32 streamFlushResult = xcb_flush(state->connection);

        if(streamFlushResult <= 0)
        {
            LOG_FATAL("We tried to flush the stream but failed: %d", streamFlushResult);
            return FALSE;
        }

        return TRUE;
    }

    // Call this clean up after our platform
    void PLT_Terminate(PLT_PlatformState* platformState)
    {
        // Cold cast to the known type
        PLT_InternalState *state = (PLT_InternalState *)platformState->internalState;

        // Turn the auto repeats back on 
        XAutoRepeatOn(state->display);

        // Destory the window
        xcb_destroy_window(state->connection, state->window);
    }

    // Fetch the messages from the platform
    bool8 PLT_FetchMessages(PLT_PlatformState* platformState)
    {
        // Cold cast to the known type
        PLT_InternalState *state = (PLT_InternalState *)platformState->internalState;

        xcb_generic_event_t *event;
        xcb_client_message_event_t *theClientMessage;

        bool8 quitRequested = FALSE;

        // Check for events until this xcb returns NULL
        while(event != 0)
        {
            event = xcb_poll_for_event(state->connection);

            if(event == 0)
            {
                break;
            }

            // Which event are we looking at right now?
            switch(event->response_type & ~0x80)
            {
                case XCB_KEY_PRESS:
                case XCB_KEY_RELEASE:
                {
                    // TODO: Handle keyboard events on Linux
                }
                break;

                case XCB_BUTTON_PRESS:
                case XCB_BUTTON_RELEASE:
                {
                    // TODO: Handle mouse button events on Linux
                }
                break;

                case XCB_MOTION_NOTIFY:
                {
                    // TODO: Mouse movement events on Linux
                }
                break;

                case XCB_CONFIGURE_NOTIFY:
                {
                    // TODO: Resizing on Linux
                }
                break;

                case XCB_CLIENT_MESSAGE:
                {
                    theClientMessage = (xcb_client_message_event_t *)event;

                    // Lets close the window
                    if(theClientMessage->data.data32[0] == state->wmDeleteWin)
                    {
                        quitRequested = TRUE;
                    } 
                }
                break;

                default:
                {
                    // Something else has happened
                }
                break;
            }

            free(event);
        }

        return !quitRequested;
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
        // FATAL, ERROR, WARN, INFO, DEBUG, TRACE
        const char* colourStrings[] = {"0;41", "1;31", "1;33", "1;32", "1;34", "1;30"};
        printf("\033[%sm%s\033[0m", colourStrings[colour], message);
    }

    // Seperate function to write out as an error (just in case we want seperate stream for errors)
    void PLT_ConsoleOutError(const char* message, uint8 colour)
    {
        // FATAL, ERROR, WARN, INFO, DEBUG, TRACE
        const char* colourStrings[] = {"0;41", "1;31", "1;33", "1;32", "1;34", "1;30"};
        printf("\033[%sm%s\033[0m", colourStrings[colour], message);
    }

    // Get the time according to the platform we're on
    float64 PLT_GetAbsoluteTime()
    {
        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        return now.tv_sec + now.tv_nsec * 0.000000001;
    }

    // Call this function to tell the current thread to sleep for the provided number of ms
    void PLT_Sleep(uint64 ms)
    {
        #if _POSIX_C_SOURCE >= 199309L
            struct timespec theTimeSpec;
            theTimeSpec.tv_sec = ms / 1000;
            theTimeSpec.tv_nsec = (ms % 1000) * 1000;
            nanosleep(&theTimeSpec, 0);
        #else
            if(ms >= 1000)
            {
                sleep(ms / 1000);
            }
        
            unsleep(ms % 1000) * 1000);

        #endif
    }

#endif /* ACN_PLATFORM_LINUX */