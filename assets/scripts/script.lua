function move(x,y,speed_x,speed_y,dt)
    if x < 0 or x > 800 then
        speed_x = -speed_x
    end
    return x + speed_x * dt, y + speed_y *dt,speed_x,speed_y
end

function attack()
    return "Laser",3
end