direction_has_changed = false

function movement(pos_x, pos_y, vel_x, vel_y)
    direction_has_changed = false
    if (pos_x <= -75)  then
        pos_x = 825
    end
    
    if (pos_y >= 550 or pos_y <= 0 )  then
        vel_y = vel_y * -1
        direction_has_changed = true
    end
    return pos_x, pos_y, vel_x, vel_y
end

function attack(can_attack,pos_x, pos_y)
    if can_attack then
        return true
    end
    return false
end