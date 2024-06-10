function OnInputUpdate()
    if Input.IsKeyPressed(Keys.F11) then
        Window.SetFullscreen(not Window.GetFullscreen())
    end
end

local Console = {
    Active = true,
    Pos = Vector2.new(50, 50),
    Size = Vector2.new(600, 400),
    HeaderSize = 25,

    -- For Dragging & Resizing
    Difference = Vector2.new(0, 0),
    DraggingStartedOutside = false, 
    ResizingStartedOutside = false, 
    DraggingActive = false,
    ResizingActive = false
}

function OnPaint()
    local TextSize = Renderer.GetTextSize(Renderer.GetDefaultFont(), tostring(Client.GetFramerate()))

    Renderer.FilledRectangle(Vector2.new(5, 5), TextSize, Color.new(0, 0, 0, 255), 0, CornerFlags.None)
    Renderer.Text(Renderer.GetDefaultFont(), tostring(Client.GetFramerate()), Vector2.new(5, 5), Color.new(255, 255, 255, 255))

    if Input.IsKeyPressed(Keys.F1) then
        Console.Active = not Console.Active
    end

    if Console.Active then
        local HeaderSize = Vector2.new(Console.Size.x, Console.HeaderSize)
        
        if Input.IsKeyHeld( Keys.Mouse1 ) and not Console.DraggingStartedOutside then
            if Input.IsMouseHoveringRect( Console.Pos, HeaderSize ) and not Console.DraggingActive then
                Console.Difference = Console.Pos - Input.GetMousePosition()
                Console.DraggingActive = true
            else
                Console.DraggingStartedOutside = true
            end
        elseif not Input.IsKeyHeld( Keys.Mouse1 ) and Console.DraggingStartedOutside then
            Console.DraggingStartedOutside = false
        end
    
        if Console.DraggingActive and not Input.IsKeyHeld( Keys.Mouse1 ) then
            Console.DraggingActive = false
        end
    
        if Console.DraggingActive then
            Console.Pos = Input.GetMousePosition() + Console.Difference
        end

        local ResizeBoxSize = Vector2.new( 10, 10 )

        if Input.IsKeyHeld( Keys.Mouse1 ) and not Console.ResizingStartedOutside then
            if Input.IsMouseHoveringRect( Console.Pos + Console.Size - ResizeBoxSize, ResizeBoxSize ) and not Console.ResizingActive then
                Console.Difference = ( Console.Pos + Console.Size - ResizeBoxSize ) - Input.GetMousePosition()
                Console.ResizingActive = true
            else
                Console.ResizingStartedOutside = true
            end
        elseif not Input.IsKeyHeld( Keys.Mouse1 ) and Console.ResizingStartedOutside then
            Console.ResizingStartedOutside = false
        end
    
        if Console.ResizingActive and not Input.IsKeyHeld( Keys.Mouse1 ) then
            Console.ResizingActive = false
        end
    
        if Console.ResizingActive then
            Console.Size = Input.GetMousePosition() - ( Console.Pos - ResizeBoxSize ) + Console.Difference
            Console.Size = Console.Size:Max(Vector2.new(100, 100))
        end

        Renderer.FilledRectangle(Console.Pos, Console.Size, Color.new(35, 35, 35, 255), 0, CornerFlags.None)
        Renderer.FilledRectangle(Console.Pos, HeaderSize, Color.new(15, 15, 15, 255), 0, CornerFlags.None)
        Renderer.Rectangle(Console.Pos, Console.Size, Color.new(0, 0, 0, 255), 0, CornerFlags.None)
        
        Renderer.Text(Renderer.GetDefaultFont(), "Console", Console.Pos + Vector2.new(4, 4), Color.new(225, 225, 225, 255))
    end
end

AddCallback("OnInputUpdate", OnInputUpdate)
AddCallback("OnInterfacePaint", OnPaint)

--LoadScript("Testing.lua")