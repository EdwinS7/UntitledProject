local Fonts = Client.GetFontList()

for i, v in ipairs(Fonts) do
    Client.Log(Client.Information, ("Registry Name:", v))
end

Client.Log(Client.Information, "Fonts")