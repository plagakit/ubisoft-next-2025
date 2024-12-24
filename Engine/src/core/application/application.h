#pragma once

#include "core/input/input.h"

struct ApplicationCmdLineArgs 
{
	int m_argc = 0;
	char** m_argv = nullptr;
};

class Application 
{
public:
	void __Init__();
	void __Shutdown__();
	void __Update__(float dt);
	void __Render__();

protected:
	virtual void Init() = 0;
	virtual void Shutdown() = 0;
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;

	Input input;

private:
	void PreInit();
	void PreUpdate(float dt);

};