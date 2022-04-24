#pragma once

class OrbFire
{
public:
	OrbFire(); 
	~OrbFire();

	OrbFire(const OrbFire& _other) = delete; 
	OrbFire(OrbFire&& _other) = delete; 

	OrbFire& operator=(const OrbFire& _other) = delete;
	OrbFire& operator=(const OrbFire&& _other) = delete;

};

