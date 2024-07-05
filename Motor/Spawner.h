#pragma once
#include "Agent.h"
class Spawner :
    public Agent
{
private:
    int timeGeneration;
    int timeForZombie;
    bool spawn;
public:
    Spawner();
    ~Spawner();
    void update();
    void init(int timeGeneration, glm::vec2 position);
    void checkSpawnZombie(vector<Zombie*>& zombies, float speed);
};

