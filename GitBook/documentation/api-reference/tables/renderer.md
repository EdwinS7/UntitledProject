---
description: Renderer table
---

# ⚙️ Renderer

### Line (function) :

```lua
Renderer.Line(from, to, color)
```

### Polyline (function) :

```lua
Renderer.Polyline([pos, ...], color)
```

### Polygon (function) :

```lua
Renderer.Polygon([pos, ...], color)
```

### Rectangle (function) :

```lua
Renderer.Rectangle(pos, size, color, rounding)
```

### Filled Rectangle (function) :

```lua
Renderer.FilledRectangle(pos, size, color, rounding)
```

### Gradient (function) :

```lua
Renderer.Gradient(pos, size, color, color, vertical)
```

### Filled Gradient (function) :

```lua
Renderer.FilledGradient(pos, size, color, color, vertical)
```

### Gradient4 (function) :

```lua
Renderer.Gradient4(pos, size, color, color, color, color)
```

### Filled Gradient4 (function) :

```lua
Renderer.FilledGradient4(pos, size, color, color, color, color)
```

### Triangle (function) :

```lua
Renderer.Triangle(p1, p2, p3, color)
```

### Filled Triangle (function) :

```lua
Renderer.FilledTriangle(p1, p2, p3, color)
```

### Circle (function) :

```lua
Renderer.Cirle(pos, radius, color)
```

### Filled Circle (function) :

```lua
Renderer.FilledCirle(pos, radius, color)
```

### Text (function) :

```lua
Renderer.Text(font, text, pos, color)
```

### Get Text Size (Vector2) :

```lua
Renderer.GetTextSize(font, text)
```

### Get Screen Size (Vector2) :

```lua
Renderer.GetScreenSize()
```

### Get Default Font (Font) :

<pre class="language-lua"><code class="lang-lua"><strong>Renderer.GetDefaultFont()
</strong></code></pre>

### Create Font (function) :

```lua
Renderer.CreateFont(font_name, size, weight, padding, antialiasing)
```

### Get Vertices Count (int) :

```lua
Renderer.GetVerticesCount()
```

### Get Indices Count (int) :

```lua
Renderer.GetIndicesCount()
```

### Push Clip (function) :

```lua
Renderer.PushClip(position, size)
```

### Pop Clip (function)

```lua
Renderer.PopClip()
```
