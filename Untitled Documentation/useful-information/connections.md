---
description: >-
  Callbacks are called every time the (specified function) is ran in the
  program, kind of like a hook
---

# 📋 Connections

### List of available connections :

```lua
"OnInterfacePaint", "__OnInterfacePaint",
"OnWorldPaint", "__OnWorldPaint",
"OnInputUpdate", "__OnInputUpdate",
"OnObjectCreation" "__OnInterfacePaint"
```

{% hint style="info" %}
Connections with \_\_ before their name are prioritized and ran first!
{% endhint %}

### Connecting a function :

```lua
Connect("OnInterfacePaint", function()
    --Do something!
end)
```

