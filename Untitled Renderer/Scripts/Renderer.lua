function OnPaint()
    local points = {
        Vector2.new(100, 100),
        Vector2.new(150, 150),
        Vector2.new(200, 200),
        Vector2.new(200, 100),
        Vector2.new(150, 125),
        Vector2.new(100, 100)
    }

    Renderer.Polyline(points, Color.new(255, 255, 255, 255))
end

AddCallback("OnInterfacePaint", OnPaint)