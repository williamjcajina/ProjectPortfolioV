#include "Interpolator.h"



void Interpolator::SetTime(XTime &timer)
{
	double time = timer.TotalTime()*1000.0f;

	if (time > animation->duration)
	{
		timer.Restart();
		currentTime = 0.0f;
	}
	else
		currentTime = time;
}

JointData Interpolator::Interpolate(JointData & prev, JointData & next , float ratio)
{
	JointData out;
	out.parentIndex = prev.parentIndex;
	
	XMVECTOR rotPrev = XMVectorSet(prev.rotation.x, prev.rotation.y, prev.rotation.z, prev.rotation.w);
	XMVECTOR rotNext = XMVectorSet(next.rotation.x, next.rotation.y, next.rotation.z, next.rotation.w);
	
	XMVECTOR posPrev = XMVectorSet(prev.translation.x, prev.translation.y, prev.translation.z, prev.translation.w);
	XMVECTOR posNext = XMVectorSet(next.translation.x, next.translation.y, next.translation.z, next.translation.w);

	XMVECTOR scaPrev = XMVectorSet(prev.scale.x, prev.scale.y, prev.scale.z, prev.scale.w);
	XMVECTOR scaNext = XMVectorSet(next.scale.x, next.scale.y, next.scale.z, next.scale.w);
	
	XMStoreFloat4(&out.rotation, XMQuaternionSlerp(rotPrev, rotNext, ratio));
	XMStoreFloat4(&out.scale, XMVectorLerp(scaPrev, scaNext, ratio));
	XMStoreFloat4(&out.translation, XMVectorLerp(posPrev, posNext, ratio));
	return out;
}

std::vector<JointData> Interpolator::currentPose()
{
	KeyFrameData next;
	KeyFrameData prev;
	vector<JointData> pose;
	float ratio;
	float endTime = 0;
	float startTime = 0;



	for (unsigned int i = 1; i < animation->frames.size(); i++)
	{

		if ((animation->frames[i].time) > currentTime)
		{
			next = animation->frames[i];

			if (i == 1)
				prev = animation->frames[animation->frames.size() - 1];
			else
				prev = animation->frames[i - 1];

			break;
		}
	}

	startTime = prev.time;
	endTime = next.time;

	if (prev.time > next.time)
		startTime = 0;

	ratio = (currentTime - startTime) / (endTime - startTime);

	for (int j = 0; j < prev.joints.size(); j++)
	{
		
		pose.push_back(Interpolate(prev.joints[j], next.joints[j], ratio));
	}
	return pose;
}



Interpolator::Interpolator()
{
}


Interpolator::~Interpolator()
{
}
