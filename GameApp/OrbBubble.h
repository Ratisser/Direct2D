#pragma once

class OrbBubble
{
public:
	OrbBubble(); 
	~OrbBubble();

	OrbBubble(const OrbBubble& _other) = delete; 
	OrbBubble(OrbBubble&& _other) = delete; 

	OrbBubble& operator=(const OrbBubble& _other) = delete;
	OrbBubble& operator=(const OrbBubble&& _other) = delete;

};

