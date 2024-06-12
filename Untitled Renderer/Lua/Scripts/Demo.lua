Client.Log(LogLevel.Information, "List of usable fonts")

local fonts = Client.GetUsableFonts()

for i, font in ipairs(fonts) do
    Client.Log(LogLevel.Normal, font)
end