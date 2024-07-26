local Particles = {}

Graphics.SetClearColor(Color.new(25, 25, 25, 255))

local WindowSize = Window.GetSize()

function RandomizePoints(i, i2)
    return {
        pos = Vector2.new(math.random(10 * (i - 1), 10 * i), math.random(10 * (i2 - 1), 10 * i2)),
    }
end

function CreateParticle(y)
    local args = {
        points = {}
    }

    for i = 1, 6 do
        args.points[i] = RandomizePoints(i, i % 3)
    end

    args.pos = Vector2.new(math.random(0, WindowSize.x), not y and math.random(0, WindowSize.y) or 0)
    args.clock = os.clock()
    
    return args
end

for i = 1, 100 do
    Particles[i] = CreateParticle()
end

Connect("OnInterfacePaint", function()
    for idx,v in next, Particles do
        local Vertices = {}
        
        if os.clock() - v.clock > 0.01 then
            v.pos.y = v.pos.y + 1

            v.clock = os.clock()
        end

        for _,point in next, v.points do
            Vertices[#Vertices + 1] = Vertex.new(v.pos.x + point.pos.x, v.pos.y + point.pos.y, 0, 1, Color.new(Math.RandomInt(0, 255), Math.RandomInt(0, 255), Math.RandomInt(0, 255), 255).Hex, 0, 0)
        end

        Renderer.WriteToBuffer(Enums.PrimitiveType.TriangleStrip, Vertices)
        
        if v.pos.y > WindowSize.y then
            Particles[idx] = CreateParticle(true)
        end
    end
end)