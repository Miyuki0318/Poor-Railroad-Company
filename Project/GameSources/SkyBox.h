/*!
@file SkyBox.h
@brief �X�J�C�{�b�N�X�I�u�W�F�N�g
@author ��I�t
*/

#pragma once
#include "stdafx.h"
#include "TemplateObject.h"

namespace basecross {
	class SkyBox : public TemplateObject
	{
		shared_ptr<PNTStaticDraw> m_ptrDraw;
		shared_ptr<MainCamera> m_camera;

	public:
		SkyBox(const shared_ptr<Stage>& stagePtr, const shared_ptr<MainCamera>& camera, const shared_ptr<Train>& train) :
			TemplateObject(stagePtr, Vec3(camera->GetEye()), Vec3(0.0f), Vec3(2.0f)),
			m_camera(camera)
		{
		}
		~SkyBox() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

	};
}
