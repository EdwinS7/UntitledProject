local Fonts = Client.GetFontList()

for i, v in ipairs(Fonts) do
    Client.Print(0, ("Registry Name:", v))
end

Client.Print(0, "Fonts")