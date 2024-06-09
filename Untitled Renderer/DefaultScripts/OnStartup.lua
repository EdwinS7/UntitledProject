--Framerate indicator

function OnPaint()
    local TextSize = Renderer.GetTextSize(Renderer.GetDefaultFont(), tostring(Client.GetFramerate()))

    Renderer.FilledRectangle(Vector2.new(5, 5), TextSize, Color.new(0, 0, 0, 255), 0)
    Renderer.Text(Renderer.GetDefaultFont(), tostring(Client.GetFramerate()), Vector2.new(5, 5), Color.new(255, 255, 255, 255))
end

AddCallback("OnInterfacePaint", OnPaint)

LoadString("Client.Print(LogLevel.Normal, ':) - LoadString Function')")
LoadScript("Demo.lua")