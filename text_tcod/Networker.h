#pragma once

#include "concrt.h"

class Networker {
public:
	Networker();
	~Networker();

	bool Ready();
	bool Failed();

private:
	Concurrency::critical_section _cs_init;
	Concurrency::critical_section _cs;

	bool _ready;
	bool _failed;
};