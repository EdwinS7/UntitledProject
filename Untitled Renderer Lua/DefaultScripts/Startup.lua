print("Startup time: " .. Client.GetRealtime())
print("Getting fonts from registry")

local Fonts = Client.GetUsableFonts()

for i, v in pairs(Fonts) do
    print(v)
end

function F11Fullscreen()
    if Input.IsKeyPressed(Enums.Keys.F11) then
        Window.SetFullscreen(not Window.GetFullscreen())
    end
end

function Paint()
    -- Framerate Counter (Top-Right)
    local TextSize = Renderer.GetTextSize(Renderer.GetDefaultFont(), tostring(Client.GetFramerate()))
    local ScreenSize = Renderer.GetScreenSize()

    Renderer.FilledRectangle(Vector2.new((ScreenSize.x - TextSize.x) - 9, 5), Vector2.new(TextSize.x + 4, TextSize.y), Color.new(0, 0, 0, 255), 0, Enums.CornerFlags.None)
    Renderer.Text(Renderer.GetDefaultFont(), tostring(Client.GetFramerate()), Vector2.new(ScreenSize.x - 8 - TextSize.x, 5), Color.new(255, 255, 255, 255))
end

Connect("__OnInputUpdate", F11Fullscreen)
Connect("__OnInterfacePaint", Paint)

print("Framerate is only extemely low because of Renderer.Text, its rendering each letter with a texture at the moment!")