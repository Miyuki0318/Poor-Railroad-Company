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
		shared_ptr<PNTStaticInstanceDraw> m_ptrDraw;

	public:
		SkyBox(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr, Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f), Vec3(1.0f, 1.0f, 1.0f))
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
