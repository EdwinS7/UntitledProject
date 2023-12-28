# DirectX9 GUI Renderer

A lightweight and easy-to-use DirectX9 renderer for building graphical user interfaces (GUIs). This project provides a simple and efficient way to integrate GUI elements into your DirectX9 applications.

## Features

- **DirectX9 Integration:** Seamlessly integrates with DirectX9 applications.
- **GUI Components:** Provides a variety of GUI components such as buttons, labels, textboxes, etc.
- **Customization:** Easily customize the appearance and behavior of GUI elements.
- **Efficient Rendering:** Optimized for performance to ensure smooth rendering in your applications.

## Getting Started

Follow these steps to integrate the DirectX9 GUI renderer into your project:

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/your-username/directx9-gui-renderer.git
   ```

2. **Include the Renderer in Your Project:**
   - Add the necessary files to your project.
   - Ensure that your project is configured to use DirectX9.

3. **Initialize the Renderer:**
   - Initialize the DirectX9 GUI renderer in your application.
   - Configure and create GUI components as needed.

4. **Build and Run:**
   - Build your project.
   - Run your application to see the GUI in action.

## Example Code

```cpp
#include "Common.hpp"

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prev_instance, PSTR cmd, int show_cmd ) {
    gWin32->CreateRenderEnvironment( "Untitled Renderer", { 1280, 720 } );
    gGraphics->Init( gWin32->GetHwnd( ) );
    gBuffer->Init( true );

    while ( gWin32->Dispatch( ) ) {
        gContext->Update( );
        // Your code here. ( On Paint Callback )
        gGraphics->Draw( );
    }

    gGraphics->Release( );
}
```

## Contributing

Contributions are welcome! If you have any ideas, bug fixes, or improvements, feel free to open an [issue](https://github.com/EdwinS7/untitled_project/issues) or create a [pull request](https://github.com/EdwinS7/untitled_project/pulls).
