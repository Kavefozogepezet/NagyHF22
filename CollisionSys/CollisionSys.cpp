// CollisionSys.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define COLLSYS_MAIN

//#include "debug/memtrace.h"

#include "tests/tests.h"
#include "Sandbox.h"

#ifdef COLLSYS_LIB
void CollSys::sandboxInit(CollSys::Sandbox& sandbox) {
	/*
	auto& sList = sandbox.getShapeList();
	BezierCurve* bezier = new BezierCurve("bezier");
	bezier->scale(0.3, 0.3);
	sList.push_back(bezier);
	sandbox.openWindow();
	*/
}
#endif
