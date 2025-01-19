EntityActionTypeMove = 0x00
EntityActionTypeShoot1 = 0x01
EntityActionTypeShoot2 = 0x02
EntityActionTypeShoot3 = 0x03
EntityActionTypeDodge = 0x04
EntityActionTypeBarrier = 0x05
EntityActionTypeDeath = 0x06
EntityActionTypeUltimate = 0x07
EntityActionTypeInvalid = 0xff

math.randomseed(os.time())

DirectionUp = -1
DirectionDown = 1
DirectionLeft = -1
DirectionRight = 1

x_dir = DirectionUp
y_dir = DirectionRight

function updateDeltatime(deltatime)  -- float deltatime
end  -- return values are ignored

function move(pos_x, pos_y)

    if pos_x <= 10.0
    then
        x_dir = DirectionRight
    elseif pos_x >= 1900.0
    then
        x_dir = DirectionLeft
    end

    if pos_y <= 10.0
    then
        y_dir = DirectionDown
    elseif pos_y >= 1070.0
    then
        y_dir = DirectionUp
    end
    return {80.0 * x_dir, 80.0 * y_dir}
end

function shoot(shoot1Deltatime, shoot2Deltatime, shoot3Deltatime)
    if shoot3Deltatime > 5.0
    then
        return EntityActionTypeShoot3
    end

    if shoot2Deltatime > 3.0
    then
        return EntityActionTypeShoot2
    end

    if shoot1Deltatime > 1.0
    then
        return EntityActionTypeShoot1
    end
    return EntityActionTypeInvalid
end
