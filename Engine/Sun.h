#pragma once

#include <time.h>
#include "DirectionalLight.h"

class Sun {
public:
	DirectionalLight* light;
	DirectionalLight* moon;
	float intensity;
	float mIntensity;
public:
	Sun(clock_t time);
	void updateLight(clock_t time);
	~Sun();
};