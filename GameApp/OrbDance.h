#pragma once

class OrbDance
{
public:
	OrbDance(); 
	~OrbDance();

	OrbDance(const OrbDance& _other) = delete; 
	OrbDance(OrbDance&& _other) = delete; 

	OrbDance& operator=(const OrbDance& _other) = delete;
	OrbDance& operator=(const OrbDance&& _other) = delete;

};

