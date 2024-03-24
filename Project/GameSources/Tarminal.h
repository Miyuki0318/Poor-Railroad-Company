/*!
@file Tarminal.h
@brief ���H�I���I�u�W�F�N�g
@author ��I�t
*/

#pragma once
#include "stdafx.h"
#include "TemplateObject.h"

namespace basecross {
	class Tarminal : public TemplateObject
	{
		shared_ptr<PNTStaticDraw> m_ptrDraw;
		shared_ptr<CollisionObb> m_ptrColl;

	public:
		Tarminal(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr, Vec3(-1.0f, 1.0f, 0.0f), Vec3(0.0f), Vec3(2.0f, 0.5f, 1.0f))
		{
		}

		Tarminal(const shared_ptr<Stage>& stagePtr, const Vec3& position) :
			TemplateObject(stagePtr, position, Vec3(0.0f), Vec3(1.0f, 0.2f, 1.0f))
		{
		}

		~Tarminal() {}

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
