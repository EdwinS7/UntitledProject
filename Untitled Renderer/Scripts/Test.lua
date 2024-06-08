Graphics.SetClearColor(Color.new(255, 255, 255, 255))

local DefaultFont = Renderer.CreateFont( "Segoe UI", 16, 400, Vector2.new( 0, 0 ), false)

local Theme = {
    Accent = Color.new(156, 175, 170, 255),
    Background = Color.new(28, 28, 28, 255),
    Group = Color.new(34, 34, 34, 255),
    GroupOutline = Color.new(39, 39, 39, 255),
    BackgroundOutline = Color.new(15, 15, 15, 255),
    Inactive = Color.new(175, 175, 175, 255),
    Active = Color.new(255, 255, 255, 255),
}

local UI = {
    title = "CheatName",
    ending = ".cc",
    game = "Game Name",
    build = "Release",
    pos = Vector2.new(20, 20),
    size = Vector2.new(600, 420),
}

--[[function UI::DragBar(pos, size)
    local m_pos = Input.GetMousePosition()
    if m_pos == pos and m_pos >= size then
        return m_pos
    end
end]]--

function UI::Window(pos, size)
    if pos or size == nil then return end

    Renderer.FilledRectangle(Vector2.new(pos.x - 1, pos.y - 1), Vector2.new(size.x + 1, size.y + 1), Theme::BackgroundOutline, 0)
    Renderer.FilledRectangle(Vector2.new(pos), Vector2.new(size), Theme::Background, 0)
end

function Interface ()
    UI::Window(UI::pos, UI::size)
end

AddCallback("OnInterfacePaint", Interface)