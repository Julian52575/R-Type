#pragma once

class Health {
    public:
        Health(int health, int maxHealth);
        Health(int health);
        ~Health();
        void takeDamage(int damage);
    
    private:
        int health;
        int maxHealth;
};