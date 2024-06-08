local Pos, Size, Difference, StartedOutside, DraggingActive = Vector2.new(100, 100), Vector2.new(100, 100), Vector2.new(0, 0), false, false

function DragFunction()
    if Input.IsKeyHeld( KEY_LMOUSE ) and not StartedOutside then
        if Input.IsMouseHoveringRect( Pos, Size ) and not DraggingActive then
            Difference = Pos - Input.GetMousePosition()
            DraggingActive = true
        else
            StartedOutside = true
        end
    elseif not Input.IsKeyHeld( KEY_LMOUSE ) and StartedOutside then
        StartedOutside = false
    end

    if DraggingActive and not Input.IsKeyHeld( KEY_LMOUSE ) then
        DraggingActive = false
    end

    if DraggingActive then
        Pos = Input.GetMousePosition() + Difference
    end
end

function OnPaint()
    Renderer.FilledRectangle(Pos, Size, Color.new(255, 255, 255, 255), 0)
    DragFunction()
end

AddCallback("OnInterfacePaint", OnPaint)