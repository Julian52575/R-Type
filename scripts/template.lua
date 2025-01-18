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

function move(pos_x, pos_y)
    return {EntityActionTypeShoot1, 20.0, 0} -- float get converted to int automatically by the api
end

function shoot()
    return 1 + (math.random() % 0x06);
end
