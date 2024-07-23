-- Made by @Splatzy
-- Maintained by @fuckuneedthisfor

-- Set background to pitch black
Graphics.SetClearColor(Color.new(0, 0, 0, 255))

-- Our demo text
local ASCII = [[
    ,'\
    _.----.        ____         ,'  _\   ___    ___     ____
_,-'       `.     |    |  /`.   \,-'    |   \  /   |   |    \  |`.
\      __    \    '-.  | /   `.  ___    |    \/    |   '-.   \ |  |
 \.    \ \   |  __  |  |/    ,','_  `.  |          | __  |    \|  |
   \    \/   /,' _`.|      ,' / / / /   |          ,' _`.|     |  |
    \     ,-'/  /   \    ,'   | \/ / ,`.|         /  /   \  |     |
     \    \ |   \_/  |   `-.  \    `'  /|  |    ||   \_/  | |\    |
      \    \ \      /       `-.`.___,-' |  |\  /| \      /  | |   |
       \    \ `.__,'|  |`-._    `|      |__| \/ |  `.__,'|  | |   |
        \_.-'       |__|    `-._ |              '-.|     '-.| |   |
                                `'                            '-._|
]]
    
function Paint ()   
	Renderer.Text(Renderer.GetDefaultFont(), ASCII, Vector2.new(5, 5), Color.new(255, 255, 255, 255))
end

-- Will remove the need to connect fonts to this in the future.
Connect("OnInterfacePaint", Paint)