#pragma once
#include "Mesh.h"

#define FLAGPOLE_TRUCK_TOP            0.5f
#define FLAGPOLE_TRUCK_CROWN          0.41f
#define FLAGPOLE_TRUCK_BOTTOM         0.38f
#define FLAGPOLE_SHAFT_TOP            0.3775f
#define FLAGPOLE_SHAFT_BOTTOM        -1.0f
#define FLAGPOLE_TRUCK_TOP_RADIUS     0.005f
#define FLAGPOLE_TRUCK_CROWN_RADIUS   0.020f
#define FLAGPOLE_TRUCK_BOTTOM_RADIUS  0.015f
#define FLAGPOLE_SHAFT_RADIUS         0.010f
#define FLAGPOLE_SHININESS            4.0f

class Flagpole : public Mesh {
public:
	Flagpole(char* filename);
	~Flagpole(void);
};