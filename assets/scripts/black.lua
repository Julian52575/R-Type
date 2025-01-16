function movement(pos_x, pos_y, vel_x, vel_y)
    if (pos_x <= -75)  then
        pos_x = 825
    end
    return pos_x, pos_y, vel_x, vel_y
end

function attack(can_attack,pos_x, pos_y)
    if can_attack then
        return true
    end
    return false
end