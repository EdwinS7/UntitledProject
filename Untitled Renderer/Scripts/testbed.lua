-- Author: Distinguished
-- Description: Testbed for the new renderer 
-- Date: 6/8/2024
Graphics.SetClearColor(Color.new(0, 0, 0, 255))

current_script = 0

x, y = 100, 100

function Interface()
    local screen_size = Renderer.GetScreenSize()

    if Input.IsKeyPressed(KEY_RIGHT_ARROW) then
        current_script = current_script + 1
    elseif Input.IsKeyPressed(KEY_LEFT_ARROW) then
        current_script = current_script - 1
    end

    if current_script == 0 then
        for i = 0, (screen_size.x / 4) do
            Renderer.Line(Vector2.new(i*4, 0), Vector2.new((i*4) * 4, screen_size.y), Color.new(math.random(0, 255), math.random(0, 255), math.random(0, 255), 255))
        end
        for i = 0, (screen_size.y / 4) do
            Renderer.Line(Vector2.new(0, i*4), Vector2.new(screen_size.x, (i*4) * 4), Color.new(math.random(0, 255), math.random(0, 255), math.random(0, 255), 255))
        end
    elseif current_script == 1 then
        for i = 0, 100 do
            Renderer.Line(Vector2.new(math.random(0, screen_size.x), math.random(0, screen_size.y)), Vector2.new(math.random(0, screen_size.x), math.random(0, screen_size.y)), Color.new(math.random(0, 255), math.random(0, 255), math.random(0, 255), 255))
        end
    elseif current_script == 2 then
        for y = 0, (screen_size.y / 15) do
            for x = 0, (screen_size.x / 15) do
                v = math.random(0, 1)
                if v == 0 then
                    Renderer.Text(Renderer.GetDefaultFont(), "0", Vector2.new(x*15, y*15), Color.new(0, 0, Math.RandomInt(0, 255), 255))
                else
                    Renderer.Text(Renderer.GetDefaultFont(), "1", Vector2.new(x*15, y*15), Color.new(0, 0, Math.RandomInt(0, 255), 255))
                end
                --[[Renderer.FilledRectangle(
                    Vector2.new(x * 10, y * 10),
                    Vector2.new(1, 1),
                    Color.new(0, math.random(0, 255), 0, 255),
                    0
                )]]
            end
        end
    elseif current_script == 3 then
        alive = true
        local m_pos = Input.GetMousePosition()

        Renderer.FilledRectangle(Vector2.new(m_pos.x - 5, m_pos.y - 5), Vector2.new(10, 10), Color.new(255, 255, 255, 255), 0)

        function kill_box(pos, size)
            Renderer.FilledRectangle(pos, size, Color.new(255, 110, 110, 255), 0)
            if Input.IsMouseHoveringRect(Vector2.new(pos.x - 5, pos.y - 5), Vector2.new(size.x + 10, size.y + 10)) then
                alive = false
                Input.SetMousePosition(Vector2.new(50, 70))
            end
        end

        if not alive then
            alive = true
        end
        -- maze
        kill_box(Vector2.new(100, 0), Vector2.new(70, 100))
        kill_box(Vector2.new(100, 120), Vector2.new(50, 100))
        kill_box(Vector2.new(170, 0), Vector2.new(50, 150))
        kill_box(Vector2.new(150, 170), Vector2.new(100, 50))
    elseif current_script == 4 then
        

    elseif current_script == 5 then
        local screen_size = Renderer.GetScreenSize()

        if Input.IsKeyHeld(KEY_W) and Input.IsKeyHeld(KEY_D) then
            y = y - 0.05
            x = x + 0.05
        elseif Input.IsKeyHeld(KEY_D) and Input.IsKeyHeld(KEY_S) then
            y = y + 0.05
            x = x + 0.05
        elseif Input.IsKeyHeld(KEY_S) and Input.IsKeyHeld(KEY_A) then
            y = y + 0.05
            x = x - 0.05
        elseif Input.IsKeyHeld(KEY_A) and Input.IsKeyHeld(KEY_W) then
            y = y - 0.05
            x = x - 0.05
        elseif Input.IsKeyHeld(KEY_W) then
            y = y - 0.05
        elseif Input.IsKeyHeld(KEY_A) then
            x = x - 0.05
        elseif Input.IsKeyHeld(KEY_S) then
            y = y + 0.05
        elseif Input.IsKeyHeld(KEY_D) then
            x = x + 0.05
        end

        if x < 0 then x = 0 end
        if x > screen_size.x - 10 then x = screen_size.x - 10 end
        if y < 0 then y = 0 end
        if y > screen_size.y - 10 then y = screen_size.y - 10 end

        Renderer.FilledRectangle(Vector2.new(x, y), Vector2.new(10, 10), Color.new(255, 255, 255, 255), 0)

        --[[function enemny(x, y)
            Renderer.FilledRectangle(Vector2.new(x, y), Vector2.new(10, 10), Color.new(255, 110, 110, 255), 0)
        end]]

        --[[for i = 0, 10 do
            enemny(Math.RandomInt(0, screen_size.x - 10), Math.RandomInt(0, screen_size.y - 10))
            sleep(1000)
        end]]
    end

    Renderer.FilledRectangle(
        Vector2.new(0, 0),
        Vector2.new(Renderer.GetTextSize(Renderer.GetDefaultFont(), tostring(Client.GetFramerate())).x + 10, 25),
        Color.new(20, 20, 20, 255),
        0
    )
    Renderer.Text(Renderer.GetDefaultFont(), tostring(Client.GetFramerate()), Vector2.new(5, 5), Color.new(255, 255, 255,255))
end

AddCallback("OnInterfacePaint", Interface)