local Speed = 0.1

local XPos, XDir = 0, Speed
local YPos, YDir = 0, Speed

local function Paint()
    local PenisSize = Vector2.new(35, 100)
    local BallSize = Vector2.new(30, 30)

    local WindowSize = Window.GetSize()

    local AbsWindowSize = WindowSize - Vector2.new(BallSize.x * 2, BallSize.y)

    if XPos >= AbsWindowSize.x then
        XDir = -Speed
    elseif XPos <= 0 then
        XDir = Speed
    end

    if YPos >= AbsWindowSize.y then
        YDir = -Speed
    elseif YPos <= PenisSize.y + BallSize.y then
        YDir = Speed
    end

    XPos = XPos + XDir
    YPos = YPos + YDir

    local PenisPosition = Vector2.new(XPos, YPos)

    Renderer.FilledRectangle(PenisPosition, BallSize, Color.new(255, 255, 255, 255), 0)

    Renderer.FilledRectangle(PenisPosition + Vector2.new(BallSize.x + 5, 0), BallSize, Color.new(255, 255, 255, 255), 0)

    Renderer.FilledRectangle(PenisPosition + Vector2.new(BallSize.x / 2, -(PenisSize.y + 5)), PenisSize, Color.new(255, 255, 255, 255), 0)
end

Connect("__OnInterfacePaint", Paint)