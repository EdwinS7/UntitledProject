local Dragging = {ResizeBoxSize = Vector2.new(10, 10), IsDragging = {}, IsResizing = {}, Difference = {}} Dragging.__index = Dragging

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

    if not self.IsDragging[id] and Input.IsMouseHoveringRect(position, size) and Input.IsKeyHeld(Enums.Keys.Mouse1) then
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

    if not self.IsResizing[id] and Input.IsMouseHoveringRect(position + (size - self.ResizeBoxSize), self.ResizeBoxSize) and Input.IsKeyHeld(Enums.Keys.Mouse1) then
        self.Difference[id] = Input.GetMousePosition() - position - size
        self.IsResizing[id] = true
    end

    if self.IsResizing[id] then
        if not Input.IsKeyHeld(Enums.Keys.Mouse1) then
            self.Difference[id] = Vector2.new()
            self.IsResizing[id] = false
        else
            size = Input.GetMousePosition() - position - self.Difference[id]
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