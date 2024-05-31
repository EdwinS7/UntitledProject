-- Set background to Black (0R, 0G, 0B, 255A)
Graphics.SetClearColor(Color.new(0, 0, 0, 255))

-- Create Font
local DefaultFont = Renderer.CreateFont( "HalfLife2", 14, 400, Vector2.new( 0, 0 ), false)

-- Our text
local ASCII = [[ HALF-LIFE 2
]]

function ObjectCreation()
  DefaultFont = Renderer.CreateFont( "HalfLife2", 14, 400, Vector2.new( 0, 0 ), false)
end
    
function InterfacePaint ()   
	Renderer.Text(DefaultFont, ASCII, Vector2.new(5, 5), Color.new(204, 53, 53,255))
end

-- Callbacks { OnInterfacePaint, OnWorldPaint, OnInputUpdate, OnObjectCreation }
AddCallback("OnObjectCreation", ObjectCreation)
AddCallback("OnInterfacePaint", InterfacePaint)