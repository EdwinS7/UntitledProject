-- Library.lua
Client.Print("Library.lua loaded")
local UI = {}

function UI.Print(str)
    Client.Print(LogLevel.Normal, str)
end

return UI
