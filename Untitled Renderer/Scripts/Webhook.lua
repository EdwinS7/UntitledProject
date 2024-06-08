local URL = "https://discord.com/api/webhooks/1248912358118526997/AHkLkSzb9TrSeGuZq2UGhYvH13vwbo-pz8oPQVTSlAXvH5qwIKdoxY8UtYsUfrcOtTa5"
local PostData = [[{
    "content": "Hello, this is a message sent from Lua!"
}]]

local response = Http.Post(URL, PostData)

print(response)