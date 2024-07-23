---
description: Input table
---

# ⚙️ Input

### Is Mouse Hovering Rectangle (bool) :

```lua
Input.IsMouseHoveringRect(pos, size)
```

### Get Mouse Position (Vec2) :

```lua
Input.GetMousePosition()
```

### Set Mouse Position (function) :

```lua
Input.SetMousePosition(pos)
```

### Is Any Key Pressed (bool) :

```lua
Input.IsAnyKeyPressed()
```

### Is Key Pressed (bool) :

```lua
Input.IsKeyPressed(key)
```

### Is Key Held (bool) :

```lua
Input.IsKeyHeld(key)
```

### Set Cursor Style (function) :

<pre class="language-lua"><code class="lang-lua"><strong>Input.SetCursorStyle(CursorStyle)
</strong></code></pre>

### Get Cursor Style (CursorStyle) :

```lua
-- Ghetto asf rn, setting also uses hcursor. I will change this soon and define them.
Input.GetCursorStyle()
```
