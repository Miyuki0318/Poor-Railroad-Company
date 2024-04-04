/*!
@file MainCamera.cpp
@brief ƒJƒƒ‰
@prod –î—I—t
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void MainCamera::OnCreate() {

	}

	void MainCamera::OnUpdate() {
		auto ptrTarget = GetTargetObject();

		if (GetTargetObject() == nullptr) return;

		Vec3 newEye = GetEye();
		Vec3 newAt = ptrTarget->GetComponent<Transform>()->GetPosition();

		SetAt(newAt);
		SetEye(newEye);
		Camera::OnUpdate();
	}
}