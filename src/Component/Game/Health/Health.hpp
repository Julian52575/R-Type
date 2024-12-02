#pragma once

class Health {
    public:
        Health(int health, int maxHealth);
        explicit Health(int health);
        ~Health();
        void takeDamage(int damage);
    private:
        int health;
        int maxHealth;
};

