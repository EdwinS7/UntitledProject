local MyOwnRenderer = {} MyOwnRenderer.__index = MyOwnRenderer

function MyOwnRenderer:FilledRectangle(position, size, color)
    local Vertices = {
        Vertex.new(position.x, position.y, 0, 1, color.Hex, 0, 0),
        Vertex.new(position.x + size.x, position.y, 0, 1, color.Hex, 0, 0),
        Vertex.new(position.x + size.x, position.y + size.y, 0, 1, color.Hex, 0, 0),
        Vertex.new(position.x, position.y + size.y, 0, 1, color.Hex, 0, 0)
    }

    Renderer.WriteToBuffer(Enums.PrimitiveType.TriangleFan, Vertices)
end

function MyOwnRenderer:FilledCircle(center, radius, color, segments)
    local Vertices = {}

    table.insert(Vertices, Vertex.new(center.x, center.y, 0, 1, color.Hex, 0, 0))

    for i = 0, segments do
        local angle = (i / segments) * math.pi * 2
        local x = center.x + radius * math.cos(angle)
        local y = center.y + radius * math.sin(angle)
        table.insert(Vertices, Vertex.new(x, y, 0, 1, color.Hex, 0, 0))
    end

    Renderer.WriteToBuffer(Enums.PrimitiveType.TriangleFan, Vertices)
end

Connect("OnInterfacePaint", function()
    MyOwnRenderer:FilledRectangle(Vector2.new(5, 5), Vector2.new(100, 100), Color.new(255, 255, 255, 255))
    MyOwnRenderer:FilledCircle(Vector2.new(200, 200), 50, Color.new(255, 255, 255, 255), 32)
end)