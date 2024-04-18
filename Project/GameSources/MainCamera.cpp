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
		auto ptrTargetPosition = GetTargetObject()->GetComponent<Transform>()->GetPosition();

		if (GetTargetObject() == nullptr) return;

		Vec3 newEye = Vec3(ptrTargetPosition.x, m_cameraArm.y, m_cameraArm.z);
		Vec3 newAt = ptrTargetPosition;

		SetAt(newAt);
		SetEye(newEye);
		Camera::OnUpdate();
	}

	void ZoomIn()
	{
	}
}