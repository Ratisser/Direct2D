#pragma once

class MiniFlowerBullet
{
public:
	MiniFlowerBullet(); 
	~MiniFlowerBullet();

	MiniFlowerBullet(const MiniFlowerBullet& _other) = delete; 
	MiniFlowerBullet(MiniFlowerBullet&& _other) = delete; 

	MiniFlowerBullet& operator=(const MiniFlowerBullet& _other) = delete;
	MiniFlowerBullet& operator=(const MiniFlowerBullet&& _other) = delete;

};

