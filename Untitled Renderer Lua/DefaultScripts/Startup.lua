function math.clamp(x, min, max)
    return math.max(math.min(x, max), min)
end

function Paint()
    local TextSize = Renderer.GetTextSize(Renderer.GetDefaultFont(), tostring(Client.GetFramerate()))
    local ScreenSize = Renderer.GetScreenSize()

    Renderer.FilledRectangle(Vector2.new((ScreenSize.x - TextSize.x) - 9, 5), Vector2.new(TextSize.x + 4, TextSize.y), Color.new(0, 0, 0, 255), 0, Enums.CornerFlags.None)
    Renderer.Text(Renderer.GetDefaultFont(), tostring(Client.GetFramerate()), Vector2.new(ScreenSize.x - 8 - TextSize.x, 5), Color.new(255, 255, 255, 255))
end

Connect("__OnInterfacePaint", Paint)