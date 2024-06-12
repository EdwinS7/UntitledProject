function OnInputUpdate()
    if Input.IsKeyPressed(Keys.F11) then
        Window.SetFullscreen(not Window.GetFullscreen())
    end
end

local IsConsoleOpen = false
local PushDown = 0

function OnPaint()
    local TextSize = Renderer.GetTextSize(Renderer.GetDefaultFont(), tostring(Client.GetFramerate()))
    local ScreenSize = Renderer.GetScreenSize()

    Renderer.FilledRectangle(Vector2.new((ScreenSize.x - TextSize.x) - 9, 5), Vector2.new(TextSize.x + 4, TextSize.y), Color.new(0, 0, 0, 255), 0, CornerFlags.None)
    Renderer.Text(Renderer.GetDefaultFont(), tostring(Client.GetFramerate()), Vector2.new(ScreenSize.x - 8 - TextSize.x, 5), Color.new(255, 255, 255, 255))

    if Input.IsKeyPressed(Keys.F1) then
        IsConsoleOpen = not IsConsoleOpen
    end

    if IsConsoleOpen then
        local Logs = Client.GetLogs(LogLevel.All)
        local combinedLog = table.concat(Logs, "\n")

        Renderer.Text(Renderer.GetDefaultFont(), combinedLog, Vector2.new(2, 2), Color.new(0, 0, 0, 255))
    end
end

AddCallback("OnInputUpdate", OnInputUpdate)
AddCallback("OnInterfacePaint", OnPaint)

LoadScript("Demo.lua")