function OnPaint()
    local TextSize = Renderer.GetTextSize(Renderer.GetDefaultFont(), tostring(Client.GetFramerate()))
    Renderer.FilledRectangle(Vector2.new(5, 5), TextSize, Color.new(0, 0, 0, 255), 0, CornerFlags.None)
    Renderer.Text(Renderer.GetDefaultFont(), tostring(Client.GetFramerate()), Vector2.new(5, 5), Color.new(255, 255, 255, 255))
end

function OnInputUpdate()
    if Input.IsKeyPressed(Keys.F11) then
        Window.SetFullscreen(not Window.GetFullscreen())
    end
end

AddCallback("OnInputUpdate", OnInputUpdate)
AddCallback("OnInterfacePaint", OnPaint)

--LoadScript("Demo.lua")
--LoadString("Client.Print(LogLevel.Normal, ':) - LoadString Function')")
LoadScript("Testing.lua")