/* 
* untitled-renderer setup config.
* feel free to modify anything to your liking.
* all < optional > features are listed below this paragraph, use DirectX 9 for any source engine game, use DirectX 11 / Vulkan ( unsupported ) for any source 2 engine game.
* use UNTITLED_USE_WIN32 & ( gfx api of choice ) for a simple demo external window rendering environment.
* entry point can be found in .cpp/ctx/ctx.cpp in case you are having troubles finding it.
* 
* this project was never intended on being released so if you read this then you have unrightfully gained access!
*/

// #define UNTITLED_USE_CONSOLE ( creates a debug console used for logging )
// #define UNTITLED_USE_INPUT ( allows usage for the input library built for uis )
// #define UNTITLED_USE_WIN32 ( creates a Win32 window for rendering )
// #define UNTITLED_USE_LOGS ( logs for device creation, object releasing, window creation, errors, warnings )
// #define UNTITLED_USE_DX9 ( DirectX 9, ( source engine ) )
// #define UNTITLED_USE_DX11 ( DirectX 11 ( source 2 engine ) )
// #define UNTITLED_USE_DX12 ( DirectX 12 ( unreal engine ) )

#define UNTITLED_USE_CONSOLE
#define UNTITLED_USE_WIN32
#define UNTITLED_USE_LOGS

#define UNTITLED_USE_INPUT
#define UNTITLED_USE_DX9