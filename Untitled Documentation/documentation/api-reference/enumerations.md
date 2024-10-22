---
description: Enumerations table
---

# ⚙️ Enumerations

## How do I use the enumerations?

```lua
Enums.Keys.Mouse1
```

### Keys :

```lua
Mouse1 = 0x01, Mouse2 = 0x02, Cancel = 0x03,
Mouse3 = 0x04, Mouse4 = 0x05, Mouse5 = 0x06,
Backspace = 0x08, Tab = 0x09, Clear = 0x0C,
Enter = 0x0D, Shift = 0x10, Ctrl = 0x11,
Alt = 0x12, PauseBreak = 0x13, CapsLock = 0x14,
Escape = 0x1B, Space = 0x20, PageUp = 0x21,
PageDown = 0x22, End = 0x23, Home = 0x24,
LeftArrow = 0x25, UpArrow = 0x26, RightArrow = 0x27,
DownArrow = 0x28, Select = 0x29, PrintScreen = 0x2C,
Insert = 0x2D, Delete = 0x2E, Help = 0x2F,
Num0 = 0x30, Num1 = 0x31, Num2 = 0x32,
Num3 = 0x33, Num4 = 0x34, Num5 = 0x35,
Num6 = 0x36, Num7 = 0x37, Num8 = 0x38,
Num9 = 0x39, A = 0x41, B = 0x42,
C = 0x43, D = 0x44, E = 0x45, F = 0x46,
G = 0x47, H = 0x48, I = 0x49, J = 0x4A,
K = 0x4B, L = 0x4C, M = 0x4D, N = 0x4E,
O = 0x4F, P = 0x50, Q = 0x51, R = 0x52,
S = 0x53, T = 0x54, U = 0x55, V = 0x56,
W = 0x57, X = 0x58, Y = 0x59, Z = 0x5A,
LeftWin = 0x5B, RightWin = 0x5C, Apps = 0x5D,
Numpad0 = 0x60, Numpad1 = 0x61, Numpad2 = 0x62,
Numpad3 = 0x63, Numpad4 = 0x64, Numpad5 = 0x65,
Numpad6 = 0x66, Numpad7 = 0x67, Numpad8 = 0x68,
Numpad9 = 0x69, Multiply = 0x6A, Add = 0x6B,
Separator = 0x6C, Subtract = 0x6D, Decimal = 0x6E,
Divide = 0x6F, F1 = 0x70, F2 = 0x71,
F3 = 0x72, F4 = 0x73, F5 = 0x74,
F6 = 0x75, F7 = 0x76, F8 = 0x77,
F9 = 0x78, F10 = 0x79, F11 = 0x7A,
F12 = 0x7B, NumLock = 0x90,
ScrollLock = 0x91, LShift = 0xA0,
RShift = 0xA1, LCtrl = 0xA2,
RCtrl = 0xA3, LAlt = 0xA4,
RAlt = 0xA5, BrowserBack = 0xA6,
BrowserForward = 0xA7, BrowserRefresh = 0xA8,
BrowserStop = 0xA9, BrowserSearch = 0xAA,
BrowserFavorites = 0xAB, BrowserHome = 0xAC,
VolumeMute = 0xAD, VolumeDown = 0xAE,
VolumeUp = 0xAF, MediaNextTrack = 0xB0,
MediaPrevTrack = 0xB1, MediaStop = 0xB2,
MediaPlayPause = 0xB3, LaunchMail = 0xB4,
LaunchMediaSelect = 0xB5, LaunchApp1 = 0xB6,
LaunchApp2 = 0xB7, Semicolon = 0xBA,
Equal = 0xBB, Comma = 0xBC,
Minus = 0xBD, Period = 0xBE,
Slash = 0xBF, GraveAccent = 0xC0,
LeftBracket = 0xDB, Backslash = 0xDC,
RightBracket = 0xDD, Apostrophe = 0xDE
```

### CursorStyle :

```lua
Arrow = 32512, IBeam = 32513, Wait = 32514,
Cross = 32515, UpArrow = 32516, Size = 32640,
Icon = 32641, SizeNWSE = 32642,
SizeNESW = 32643, SizeWE = 32644,
SizeNS = 32645, SizeAll = 32646, No = 32648,
Hand = 32649, AppStarting = 32650, Help = 32651,
Pin = 32671, Person = 32672
```

### CornerFlags :

```lua
None = 0,
TopLeft = 1,
TopRight = 2,
BottomLeft = 4,
BottomRight = 8,

Top = 3,
Right = 10,
Bottom = 12,
Left = 5,

All = 15
```

### LogLevel :

```lua
Normal = 0,
Information = 1,
Warning = 2,
Error = 3,
Success = 4,
Unknown = 5,
All = 6 --Only used with Client.GetLogs(LogLevel)
```

{% hint style="info" %}
Only to be used with Client.Log(LogLevel, message, ...) or Client.GetLogs(LogLevel)
{% endhint %}

### PrimitiveType

```lua
PointList = 1,
LineList = 2,
LineStrip = 3,
TriangleList = 4,
TriangleStrip = 5,
TriangleFan = 6
```
