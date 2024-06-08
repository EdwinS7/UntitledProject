function OnPaint()
    local points = {
        Vector2.new(100, 100),
        Vector2.new(100, 150),
        Vector2.new(200, 150),
        Vector2.new(300, 200),
        Vector2.new(350, 100)
    }

    Renderer.Polygon(points, Color.new(255, 255, 255, 255))
end

AddCallback("OnInterfacePaint", OnPaint)