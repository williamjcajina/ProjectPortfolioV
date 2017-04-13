#pragma once

#include "Model.h"
class Interpolator
{
public:
	
	AnimationData* animation;
	void SetTime(XTime &timer);
	JointData Interpolate(JointData & prev, JointData & next, float ratio);
	std::vector<JointData> currentPose();
	std::vector<JointData> pose;
	float currentTime;
	Interpolator();

	~Interpolator();
};

