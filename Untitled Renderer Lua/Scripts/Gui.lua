local Dragging = require("Dragging")

local Storage = {
    Gui = {
        Position = Vector2.new(100, 100),
        Size = Vector2.new(100, 100)
    },

    Gui2 = {
        Position = Vector2.new(250, 100),
        Size = Vector2.new(100, 100)
    }
}

Connect("OnInterfacePaint", function()
    Storage.Gui.Position, Storage.Gui.Size = Dragging:HandleFrame("Part1", Storage.Gui.Position, Storage.Gui.Size)
    Renderer.FilledRectangle(Storage.Gui.Position, Storage.Gui.Size, Color.new(0, 0, 0, 255), 10)

    Storage.Gui2.Position, Storage.Gui2.Size = Dragging:HandleFrame("Part2", Storage.Gui2.Position, Storage.Gui2.Size)
    Renderer.FilledRectangle(Storage.Gui2.Position, Storage.Gui2.Size, Color.new(255, 255, 255, 255), 10)
end)