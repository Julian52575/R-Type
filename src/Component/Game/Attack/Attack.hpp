#pragma once

class Attack {
    public:
        Attack(int damage, float attackSpeed);
        ~Attack();
        void update(float deltaTime);
        bool canAttack();
        void reset();
        int dealDamage();

    private:
        int damage;
        float attackSpeed;
        float currentCooldown;
};