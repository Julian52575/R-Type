do_action = false
do_action_charge = false

function movement(pos_x, pos_y, vel_x, vel_y)
    local function charge()
        vel_x = vel_x - 3
        if (pos_x <= -500)  then
            pos_x = 800
            vel_x = 0
            do_action_charge = false
            do_action = false
        end
    end

    math.randomseed(os.time())
    if math.random() < 0.1 and do_action == false then
        do_action_charge = true
        do_action = true
    end
    if do_action_charge then
        charge()
    end

    return pos_x, pos_y, vel_x, vel_y
end

function attack(can_attack,pos_x, pos_y)
    return true
end