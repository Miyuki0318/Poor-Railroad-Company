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

	public:
		SkyBox(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr, Vec3(3.0f, -5.0f, -7.0f), Vec3(0.0f), Vec3(2.0f))
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
