---
description: Graphics table
---

# ⚙️ Graphics

### Set Vertical Sync (function) :

```lua
Graphics.SetVerticalSync(state: bool) --off by default
```

### Set Clear Color (function) :

```lua
Graphics.SetClearColor(color)
```

{% hint style="warning" %}
Due to DirectX9 limitations, you cannot set the alpha value of the clear color.
{% endhint %}

### Get Clear Color (Color) :

<pre class="language-lua"><code class="lang-lua"><strong>Graphics.GetClearColor() -- Default { 160: r, 215: g, 232: b, 255: a }
</strong></code></pre>

{% hint style="info" %}
What is clear color? It's the color that the window resets to each frame, essentially serving as the background color.
{% endhint %}
