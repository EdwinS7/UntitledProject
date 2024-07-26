---
description: Renderer table
---

# ⚙️ Renderer

### Line (function) :

```lua
Renderer.Line(from: Vector2, to: Vector2, color: Color)
```

### Polyline (function) :

```lua
Renderer.Polyline([pos: Vector2, ...], color: Color)
```

### Polygon (function) :

```lua
Renderer.Polygon([pos: Vector2, ...], color: Color)
```

### Rectangle (function) :

```lua
Renderer.Rectangle(pos: Vector2, size: Vector2, color: Color, rounding: int)
```

### Filled Rectangle (function) :

```lua
Renderer.FilledRectangle(pos: Vector2, size: Vector2, color: Color, rounding: int)
```

### Gradient (function) :

```lua
Renderer.Gradient(pos: Vector2, size: Vector2, color: Color, vertical: bool)
```

### Filled Gradient (function) :

```lua
Renderer.FilledGradient(pos: Vector2, size: Vector2, color: Color, vertical: bool)
```

### Gradient4 (function) :

```lua
Renderer.Gradient4(pos: Vector2, size: Vector2, color: Color, color: Color, color: Color, color: Color)
```

### Filled Gradient4 (function) :

```lua
Renderer.FilledGradient4(pos: Vector2, size: Vector2, color: Color, color: Color, color: Color, color: Color)
```

### Triangle (function) :

```lua
Renderer.Triangle(p1: Vector2, p2: Vector2, p3: Vector2, color: Color)
```

### Filled Triangle (function) :

```lua
Renderer.FilledTriangle(p1: Vector2, p2: Vector2, p3: Vector2, color: Color)
```

### Circle (function) :

```lua
Renderer.Cirle(pos: Vector2, radius: int, color: Color)
```

### Filled Circle (function) :

```lua
Renderer.FilledCircle(pos: Vector2, radius: int, color: Color)
```

### Text (function) :

```lua
Renderer.Text(font: Font, text: string, pos: Vector2, color: Color)
```

### Get Text Size (Vector2) :

```lua
Renderer.GetTextSize(font: Font, text: string)
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
Renderer.CreateFont(font_name: string, size: int, weight: int, padding: int, antialiasing: bool)
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
Renderer.PushClip(position: Vector2, size: Vector2)
```

### Pop Clip (function)

```lua
Renderer.PopClip()
```

### WriteToBuffer (function)

```lua
Renderer.WriteToBuffer(primitive: PrimitiveType, [vertex: Vertex, ...], [index: int, ...])
```

{% hint style="info" %}
I highly recommend you read on how geometric primitives work, reference here:

[https://learn.microsoft.com/en-us/windows/win32/direct3d9/primitives](https://learn.microsoft.com/en-us/windows/win32/direct3d9/primitives)
{% endhint %}
