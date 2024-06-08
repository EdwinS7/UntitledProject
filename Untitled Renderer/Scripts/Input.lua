function InputUpdate()
    if (Input.IsKeyHeld(KEY_LMOUSE)) then
        print("lmao")
    end
end

AddCallback("OnInputUpdate", InputUpdate)