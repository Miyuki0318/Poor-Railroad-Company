/*!
@file MainCamera.h
@brief �J����
@prod ��I�t
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class MainCamera : public Camera {
		weak_ptr<GameObject> m_targetObject;	// �ڕW�ƂȂ�I�u�W�F�N�g
		shared_ptr<Transform> m_targetTrans;	// �ڕW�̃g�����X�t�H�[��

		Vec3 m_cameraArm;

	public:
		// �R���X�g���N�^�ƃf�X�g���N�^
		MainCamera() : Camera(),
			m_cameraArm(Vec3(0.0f, 20.0f, -22.0f))
		{
		}
		~MainCamera() {}

		void OnCreate() override;
		void OnUpdate() override;

		void ZoomIn();

		// �J�������ǔ�����I�u�W�F�N�g���擾����֐�
		shared_ptr<GameObject> GetTargetObject() const {
			if (!m_targetObject.expired()) {
				return m_targetObject.lock();
			}
			return nullptr;
		}
		// �J�������ǔ�����I�u�W�F�N�g��ݒ肷��֐�
		void SetTargetObject(const shared_ptr<GameObject>& Obj) {
			m_targetObject = Obj;
		}
	};
}