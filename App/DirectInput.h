#pragma once

#include "stdafx.h"

class DirectInput: public ISubSystem
{
	friend class Console;
private:
	IDirectInput8*				m_pDInput;
	IDirectInputDevice8*		m_pKeyboard;
	char						m_KeyState[256];
	bool						m_KeyLock[256];
	IDirectInputDevice8*		m_pMouse;
	DIMOUSESTATE2				m_MouseState;
	bool						m_Lock;
	
protected:
	//bool SysKeyDown(char key);
	//bool MouseButtonDown(int button);
public:
	DirectInput();
	~DirectInput();

	void Init(Context* c);
	void ShutDown();
	void Update();
	bool KeyDown(char key);
	// Support look key from user
	void LookKey(char key, bool v);
	void LookAll();
	bool MouseButtonDown(int button);
	float mouseDX();
	float mouseDY();
	float mouseDZ();

private:
	DirectInput(const DirectInput& other);
	DirectInput& operator=(const DirectInput& other);

};
