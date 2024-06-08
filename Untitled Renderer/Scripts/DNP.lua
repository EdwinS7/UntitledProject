Graphics.SetClearColor(Color.new(24, 24, 24, 255))

local DefaultFont = Renderer.CreateFont("Segoe UI", 12, 400, Vector2.new(0, 0), false)

local Theme = {
    Accent            = Color.new(156, 175, 170, 255),
    Background        = Color.new(28, 28, 28, 255),
    Group             = Color.new(34, 34, 34, 255),
    GroupOutline      = Color.new(39, 39, 39, 255),
    BackgroundOutline = Color.new(15, 15, 15, 255),
    Inactive          = Color.new(175, 175, 175, 255),
    Active            = Color.new(255, 255, 255, 255),
}

local UI = {}

function UI.DragBar(pos, size)
    local delta = Vector2.new()
    local drag = false
    local move = false
    local held = false

    local m_pos = Input.GetMousePosition()

    if not Input.IsKeyPressed(KEY_LMOUSE) and not Input.IsKeyHeld(KEY_LMOUSE) then
        held = false
    end

    if m_pos.x >= pos.x and m_pos.y >= pos.y and m_pos.x <= pos.x + size.x and m_pos.y <= pos.y + size.y and Input.IsKeyHeld(KEY_LMOUSE) --[[(held or Input.IsKeyPressed(KEY_LMOUSE))]] then
        held = true
        drag = true
        if not move then
            delta.x = m_pos.x - pos.x
            delta.y = m_pos.y - pos.y
            move = true
        end
    end

    if drag then
        pos.x = m_pos.x - delta.x
        pos.y = m_pos.y - delta.y
    end

    if not held then
        drag = false
        move = false
    end

    if held then
        print("Dragging")
    end
end

screen_size = Renderer.GetScreenSize()
size = Vector2.new(600, 420)
pos = Vector2.new(10, 10)

function UI.Window()
    Renderer.FilledRectangle(Vector2.new(pos.x - 1, pos.y - 1), Vector2.new(size.x + 2, size.y + 2), Theme.BackgroundOutline, 0)
    Renderer.FilledRectangle(pos, size, Theme.Background, 0)
    Renderer.Line(Vector2.new(pos.x, pos.y + 20), Vector2.new(pos.x + size.x, pos.y + 20), Theme.GroupOutline, 0)
    UI.DragBar(pos, Vector2.new(size.x, 20))

    Renderer.FilledTriangle(
        Vector2.new(pos.x + size.x - 14, pos.y + size.y),
        Vector2.new(pos.x + size.x, pos.y + size.y - 14),
        Vector2.new(pos.x + size.x, pos.y + size.y),
        Theme.GroupOutline
    )
    Renderer.FilledTriangle(
        Vector2.new(pos.x + size.x - 12, pos.y + size.y - 1),
        Vector2.new(pos.x + size.x - 1, pos.y + size.y - 12),
        Vector2.new(pos.x + size.x - 1, pos.y + size.y - 1),
        Theme.Group
    )
    UI.DragBar(Vector2.new(pos.x + size.x - 14, pos.y + size.y - 14), Vector2.new(14, 14))
end

function Interface ()
    -- debug info
    Renderer.FilledRectangle(Vector2.new(0, 0), Vector2.new(60, 60), Color.new(10, 10, 10, 255), 0)
    Renderer.Text(DefaultFont, Renderer.GetVerticesCount(), Vector2.new(5, 5), Color.new(175, 175, 175 ,255))
    Renderer.Text(DefaultFont, Renderer.GetIndicesCount(), Vector2.new(5, 20), Color.new(175, 175, 175 ,255))

    local m_pos = Input.GetMousePosition()
    Renderer.FilledRectangle(m_pos, Vector2.new(2, 2), Color.new(255, 255, 255, 255), 0)

    UI.Window()
end

AddCallback("OnInterfacePaint", Interface)