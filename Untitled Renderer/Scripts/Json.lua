-- Define a Lua table
data = {
    name = "John",
    age = 30,
    city = "New York",
    interests = {"hiking", "reading", "coding"}
}

-- Serialize the Lua table to JSON
json_str = Json.Serialize(data)

-- Print the JSON string
print("Serialized JSON:")
print(json_str)

-- Deserialize the JSON string back to Lua table
decoded_data = Json.Deserialize(json_str)

-- Print the deserialized Lua table
print("\nDeserialized Data:")
for k, v in pairs(decoded_data) do
    if type(v) == "table" then
        print(k, table.concat(v, ", "))
    else
        print(k, v)
    end
end
