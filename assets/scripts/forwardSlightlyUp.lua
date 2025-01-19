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

function updateDeltatime(deltatime)  -- float deltatime
end  -- return values are ignored

function move(pos_x, pos_y)
    return {-10, -2}
end

function shoot(shoot1Deltatime, shoot2Deltatime, shoot3Deltatime)
    return EntityActionTypeInvalid
end
