#pragma once

#include <time.h>
#include "DirectionalLight.h"

enum Cicle {
	day, night, day_night, night_day
};

class Sun {
public:
	DirectionalLight* light;
	DirectionalLight* moon;
	float intensity;
	float mIntensity;
	float lastDraw;
	float transition;
	Cicle state;
	bool stop;
	
public:
	Sun(clock_t time);
	void updateLight(float time);
	~Sun();
};