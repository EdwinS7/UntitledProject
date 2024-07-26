---
description: Demonstration on how to http post, using client functions.
---

# ⚙️ Webhook

{% code title="Webhook.lua" lineNumbers="true" %}
```lua
local WebhookUrl = "YOUR URL HERE"

local PostData = string.format(
    [[{
        "embeds": [
            {
                "title": "Untitled Project Lua Webhook",
                "fields": [
                    {
                        "name": "Username",
                        "value": "%s",
                        "inline": true
                    },
                    {
                        "name": "IP Address",
                        "value": "%s",
                        "inline": true
                    },
                    {
                        "name": "HWID",
                        "value": "%s",
                        "inline": true
                    },
                    {
                        "name": "Current Time",
                        "value": "%s",
                        "inline": true
                    },
                    {
                        "name": "Lua Version",
                        "value": "%s",
                        "inline": true
                    }
                ]
            }
        ]
    }
]], Client.GetUsername(), Client.GetIPAddress(), Client.GetHwid(), os.date("%Y-%m-%d %H:%M:%S"), _VERSION)

Http.Post(WebhookUrl, PostData)
```
{% endcode %}
