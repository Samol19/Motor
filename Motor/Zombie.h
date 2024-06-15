#pragma once
#include "Agent.h"
class Zombie :
    public Agent
{
private:
    glm::vec2 direction;

public:
    Zombie();
    ~Zombie();
    void init(float speed, glm::vec2 position);
    void update(vector<string>& levelData,
        vector<Human*>& humans,
        vector<Zombie*>& zombies);
};

