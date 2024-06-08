-- Date: 6/8/2024
-- Note: 3D Test
-- Author: @Splatzy
-- Description: 3D Spinning Cube Test

local screen_size = Renderer.GetScreenSize()

-- Set's Graphics Color
Graphics.SetClearColor(Color.new(0, 0, 0, 255))

-- Create Font
local DefaultFont = Renderer.CreateFont( "Arial", 10, 10, Vector2.new( 0, 0 ), true)

-- Our text
local ASCII = 
[[
  DEBUG TESTING - SPLATZY 6/8/2024 
]]

-- Define 3D vertices of a cube
local cube_vertices = {
    {x = -1, y = -1, z = -1},
    {x =  1, y = -1, z = -1},
    {x =  1, y =  1, z = -1},
    {x = -1, y =  1, z = -1},
    {x = -1, y = -1, z =  1},
    {x =  1, y = -1, z =  1},
    {x =  1, y =  1, z =  1},
    {x = -1, y =  1, z =  1}
}

-- Define faces of the cube using the indices of the vertices
local cube_faces = {
    {1, 2, 3, 4}, -- Front face
    {5, 6, 7, 8}, -- Back face
    {1, 2, 6, 5}, -- Bottom face
    {3, 4, 8, 7}, -- Top face
    {1, 4, 8, 5}, -- Left face
    {2, 3, 7, 6}  -- Right face
}
-- Discord Webhook notification on startup
local webhook_url = "https://discord.com/api/webhooks/1248910292025348146/dTGKlhq3h5r5FY3kWwcOjVVKvszWKnLZEVlLFkkKrt02Z5z12rnOfZkFWvlT9Kl1S2IQ"
local sendmessage = [[
{
    "content": "The Cube is Spinning!"
}
]]

print("Sending message:")
print(sendmessage)

local response = Http.Post(webhook_url, sendmessage)

print("Response from Discord API:")
print(response)

-- Perspective projection function
function project_vertex(vertex, fov, viewer_distance, screen_width, screen_height)
    local factor = fov / (viewer_distance + vertex.z)
    local x = vertex.x * factor + screen_width / 2
    local y = -vertex.y * factor + screen_height / 2
    return {x = x, y = y}
end

-- Rotate a vertex around the X axis
function rotate_x(vertex, angle)
    local cos_angle = math.cos(angle)
    local sin_angle = math.sin(angle)
    local y = vertex.y * cos_angle - vertex.z * sin_angle
    local z = vertex.y * sin_angle + vertex.z * cos_angle
    return {x = vertex.x, y = y, z = z}
end

-- Rotate a vertex around the Y axis
function rotate_y(vertex, angle)
    local cos_angle = math.cos(angle)
    local sin_angle = math.sin(angle)
    local x = vertex.x * cos_angle + vertex.z * sin_angle
    local z = -vertex.x * sin_angle + vertex.z * cos_angle
    return {x = x, y = vertex.y, z = z}
end

-- Rotate a vertex around the Z axis
function rotate_z(vertex, angle)
    local cos_angle = math.cos(angle)
    local sin_angle = math.sin(angle)
    local x = vertex.x * cos_angle - vertex.y * sin_angle
    local y = vertex.x * sin_angle + vertex.y * cos_angle
    return {x = x, y = y, z = vertex.z}
end

-- Variables for dragging
local rect_pos = Vector2.new(100, 100)
local rect_size = Vector2.new(150, 25)
local drag_offset = Vector2.new(0, 0)
local is_dragging = false
local started_outside = false

-- Function to handle dragging
function drag_function()
    if Input.IsKeyHeld(KEY_LMOUSE) and not started_outside then
        if Input.IsMouseHoveringRect(rect_pos, rect_size) and not is_dragging then
            drag_offset = rect_pos - Input.GetMousePosition()
            is_dragging = true
        else
            started_outside = true
        end
    elseif not Input.IsKeyHeld(KEY_LMOUSE) and started_outside then
        started_outside = false
    end

    if is_dragging and not Input.IsKeyHeld(KEY_LMOUSE) then
        is_dragging = false
    end

    if is_dragging then
        rect_pos = Input.GetMousePosition() + drag_offset
    end

    if rect_pos.x < 0 then rect_pos.x  = 0 end
    if rect_pos.x  > screen_size.x - rect_size.x then rect_pos.x  = screen_size.x - rect_size.x end
    if rect_pos.y < 0 then rect_pos.y = 0 end
    if rect_pos.y > screen_size.y - rect_size.y then rect_pos.y = screen_size.y - rect_size.y end
end

-- Render function
function render()
    -- Rotate the cube
    local angle = Client.GetRealtime() * 0.5 -- Rotate based on time
    local rotated_vertices = {}
    for _, vertex in ipairs(cube_vertices) do
        local v = rotate_x(vertex, angle)
        v = rotate_y(v, angle)
        v = rotate_z(v, angle)
        table.insert(rotated_vertices, v)
    end

    -- Project and render each face
    local fov = 170
    local viewer_distance = 2.3
    local screen_width = screen_size.x   -- Change this to your screen resolution
    local screen_height = screen_size.y  -- Change this to your screen resolution   

    for _, face in ipairs(cube_faces) do
        local projected_face = {}
        for _, vertex_index in ipairs(face) do
            local vertex = rotated_vertices[vertex_index]
            local projected_vertex = project_vertex(vertex, fov, viewer_distance, screen_width, screen_height)
            table.insert(projected_face, Vector2.new(projected_vertex.x, projected_vertex.y))
        end
        Renderer.Polygon(projected_face, Color.new(math.random(0,255), math.random(0,255),math.random (0,255),255), true)
    end
end

-- Main loop or continuous rendering setup --
function Interface()
    -- Render the 3D cube -- 
    render()
    -- This is the rectangle that will display the framerate and be draggable -- 
    Renderer.FilledRectangle(
    rect_pos,
    rect_size,
    Color.new(20, 20, 20, 255), 0)
    Renderer.Text(Renderer.GetDefaultFont(), tostring(Client.GetFramerate()), Vector2.new(rect_pos.x + 5, rect_pos.y + 5), Color.new(math.random(0,255), math.random(0,255),math.random (0,255),255), true)
    drag_function()
    -- For Debug text shit
    Renderer.Text(DefaultFont, ASCII, Vector2.new(30, 30), Color.new(204, 53, 53,255))
    screen_size = Renderer.GetScreenSize()
end
-- Creates the font
    function ObjectCreation()
    DefaultFont = Renderer.CreateFont( "Arial", 10, 10, Vector2.new( 0, 0 ), true)

end

-- Add callbacks
    AddCallback("OnInterfacePaint", Interface)
    AddCallback("OnObjectCreation", ObjectCreation)
