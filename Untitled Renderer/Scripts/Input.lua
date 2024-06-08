function InputUpdate()
    if (Input.IsKeyHeld(KEY_LMOUSE)) then
        print("Mouse1 Held!")
    end

    if (Input.IsKeyPressed(KEY_RMOUSE)) then
        print("Mouse2 Pressed!")
    end
end

AddCallback("OnInputUpdate", InputUpdate)