-- Dragging ModuleScript
-- Developed & Maintained by @fuckuneedthisfor

local Dragging = {
    MaximumFrameSize = Renderer.GetScreenSize(),
    MinimumFrameSize = Vector2.new(100, 100),
    ResizeBoxSize = Vector2.new(10, 10),

    DragUsingHeader = true,
    HeaderSize = Vector2.new(),

    IsDragging = {},
    IsResizing = {},

    -- Shared (Dragging & Resizing)
    Difference = {}
} 

Dragging.__index = Dragging

function Dragging:HandleDragging(id, position, size)
    for _, v in pairs(self.IsResizing) do
        if v then
            return position
        end
    end

    for k, v in pairs(self.IsDragging) do
        if k ~= id and v then 
            return position
        end
    end

    local MouseHoveringRect = Input.IsMouseHoveringRect(position, self.DragUsingHeader and size or self.HeaderSize)

    if MouseHoveringRect or self.IsDragging[id] then
        Input.SetCursorStyle(Enums.CursorStyle.SizeAll)
    end

    if not self.IsDragging[id] and MouseHoveringRect and Input.IsKeyHeld(Enums.Keys.Mouse1) then
        self.Difference[id] = Input.GetMousePosition() - position
        self.IsDragging[id] = true
    end

    if self.IsDragging[id] then
        if not Input.IsKeyHeld(Enums.Keys.Mouse1) then
            self.Difference[id] = Vector2.new()
            self.IsDragging[id] = false
        else
            position = Input.GetMousePosition() - self.Difference[id]
        end
    end

    return position
end

function Dragging:HandleResizing(id, position, size)
    for _, v in pairs(self.IsDragging) do
        if v then 
            return size
        end
    end

    for k, v in pairs(self.IsResizing) do
        if k ~= id and v then 
            return size
        end
    end

    local MouseHoveringRect = Input.IsMouseHoveringRect(position + (size - self.ResizeBoxSize), self.ResizeBoxSize)

    if MouseHoveringRect or self.IsResizing[id] then
        Input.SetCursorStyle(Enums.CursorStyle.SizeNWSE)
    end

    if not self.IsResizing[id] and MouseHoveringRect and Input.IsKeyHeld(Enums.Keys.Mouse1) then
        self.Difference[id] = Input.GetMousePosition() - position - size
        self.IsResizing[id] = true
    end

    if self.IsResizing[id] then
        if not Input.IsKeyHeld(Enums.Keys.Mouse1) then
            self.Difference[id] = Vector2.new()
            self.IsResizing[id] = false
        else
            size = Input.GetMousePosition() - position - self.Difference[id]

            size = Vector2.new( -- Prevents undersizing and oversizing
                Math.Clamp(size.x, self.MinimumFrameSize.x, self.MaximumFrameSize.x),
                Math.Clamp(size.y, self.MinimumFrameSize.y, self.MaximumFrameSize.y)
            )
        end
    end

    return size
end

function Dragging:HandleFrame(id, position, size)
    size = self:HandleResizing(id, position, size)
    position = self:HandleDragging(id, position, size)

    return position, size
end

return Dragging