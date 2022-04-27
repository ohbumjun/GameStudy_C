#pragma once
#include "Animation/AnimationSequenceInstance.h"

class CPlayerAnimation :
    public CAnimationSequenceInstance
{
	friend class CPlayer;
public:
	CPlayerAnimation();
	CPlayerAnimation(const CPlayerAnimation& Anim);
	~CPlayerAnimation();

public:
	virtual bool Init();
	virtual CPlayerAnimation* Clone();
};

