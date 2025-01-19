EntityActionTypeMove = 0x00
EntityActionTypeShoot1 = 0x01
EntityActionTypeShoot2 = 0x02
EntityActionTypeShoot3 = 0x03
EntityActionTypeDodge = 0x04
EntityActionTypeBarrier = 0x05
EntityActionTypeDeath = 0x06
EntityActionTypeUltimate = 0x07
EntityActionTypeInvalid = 0xff
time = 0.0

function updateDeltatime(deltatime)  -- float deltatime
    time = time + deltatime
end  -- return values are ignored

function move(pos_x, pos_y)
    return {0, 0} -- return x, y  -- float get converted to int automatically by the api
end

function shoot(shoot1Deltatime, shoot2Deltatime, shoot3Deltatime)
    if shoot1Deltatime > 5.0
    then
        return EntityActionTypeShoot1
    end

    if shoot2Deltatime > 3.0
    then
        return EntityActionTypeShoot1
    end

    if shoot1Deltatime > 1.0
    then
        return EntityActionTypeShoot1
    end

    return EntityActionTypeInvalid
end
