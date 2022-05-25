#pragma once

#define MakeState(ClassName, StateName) #StateName, std::bind(&ClassName::start##StateName, this, std::placeholders::_1) \
		, std::bind(&ClassName::update##StateName, this, std::placeholders::_1)

#define MakeStateWithEnd(ClassName, StateName) #StateName, std::bind(&ClassName::start##StateName, this, std::placeholders::_1) \
		, std::bind(&ClassName::update##StateName, this, std::placeholders::_1) \
		, std::bind(&ClassName::end##StateName, this, std::placeholders::_1)