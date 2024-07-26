---
description: Guide to writing Lua scripts for Untitled Project
---

# 📲 Getting started

### Using Lua scripts

1. Save your Lua script in the Lua\Scripts folder, then run the executable.
2. Type your Lua script name in with the respected file extension.

### Text editor

The best text editors make it simple and easy to code without formatting issues corrupting your script.

| Text editor                      | Link                                                    |
| -------------------------------- | ------------------------------------------------------- |
| :first\_place:Visual studio code | [code.visualstudio.com](https://code.visualstudio.com/) |
| :second\_place:Sublime Text      | [sublimetext.com](https://www.sublimetext.com/)         |
| :third\_place:Notepad++          | [notepad-plus-plus.org](https://notepad-plus-plus.org/) |

{% hint style="info" %}
**the API automatically includes the base, package, coroutine**, **string, math, table, debug, bit32, utf8, ffi, jit and os library for ease of use!**
{% endhint %}

{% hint style="warning" %}
You should keep most of your scripts functions and variables as local to avoid collision with other scripts. Global variables should only be used to write libraries or scripts that interact with each other.
{% endhint %}

### :beginner:Beginner's guide

If you're new to Lua scripting, we'd recommend to take a look at the «[Lua in 5 minutes](https://learnxinyminutes.com/docs/lua/)» guide
