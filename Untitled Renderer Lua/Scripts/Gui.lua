-- This Is not a ModuleScript, Simple GUI.
-- Developed & Maintained by @fuckuneedthisfor

local Dragging = require("Dragging")

local Gui = {
    Title = "Untitled Project",

    Position = Vector2.new(100, 100),
    Size = Vector2.new(400, 300),
    
    -- Color settings
    AccentColor = Color.new(255, 120, 140, 255),
    FrameColor = Color.new(35, 35, 35, 255),
    HeaderColor = Color.new(25, 25, 25, 255),
    BorderColor = Color.new(0, 0, 0, 255),

    -- Style settings
    FrameRounding = 8,
    HeaderSize = 22
}

-- Update GUI Minimum size, 1/2 of default
Dragging.MinimumFrameSize = Vector2.new(Gui.Size.x / 2, Gui.Size.y / 2)
Dragging.DragUsingHeader = false

Connect("OnInterfacePaint", function()
    Dragging.HeaderSize = Vector2.new(Gui.Size.x, Gui.HeaderSize)
    Gui.Position, Gui.Size = Dragging:HandleFrame("Part1", Gui.Position, Gui.Size)

    -- Frame
    Renderer.FilledRectangle(Gui.Position, Gui.Size, Gui.FrameColor, Gui.FrameRounding, Enums.CornerFlags.All)

    -- Header / Header line / Header shadow
    local HeaderSize = Vector2.new(Gui.Size.x, 22)
    Renderer.FilledRectangle(Gui.Position, HeaderSize, Gui.HeaderColor, Gui.FrameRounding, Enums.CornerFlags.Top)
    Renderer.FilledGradient(Gui.Position + Vector2.new(0, 22), Vector2.new(Gui.Size.x, 1), Gui.AccentColor, Color.new(0, 0, 0, 0), false)
    Renderer.FilledGradient(Gui.Position + Vector2.new(0, 23), HeaderSize, Color.new(0, 0, 0, 122), Color.new(0, 0, 0, 0), true)
    
    -- Frame Outline, After header to avoid z overlapping.
    Renderer.Rectangle(Gui.Position, Gui.Size, Gui.BorderColor, Gui.FrameRounding, Enums.CornerFlags.All)
    Renderer.Text(Renderer.GetDefaultFont(), Gui.Title, Gui.Position + Vector2.new(4, 4), Color.new(255, 255, 255, 255))
end)