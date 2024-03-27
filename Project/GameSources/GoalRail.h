/*!
@file GoalRail.h
@brief �S�[���n�_�I�u�W�F�N�g
@author �����
*/

#pragma once
#include "stdafx.h"
#include "TemplateObject.h"

namespace basecross {
	class GoalRail : public TemplateObject
	{
		// ���g�̃h���[�R���|�[�l���g�̃|�C���^
		shared_ptr<PNTStaticDraw> m_ptrDraw;
		// ���g�̃R���W�����̃|�C���^
		shared_ptr<CollisionObb> m_ptrColl;
		// �J�n�������������������֐��p��bool
		bool m_setFlg;

	public:

		GoalRail(const shared_ptr<Stage>& stagePtr, const Vec3& position) :
			TemplateObject(stagePtr, position, Vec3(0.0f), Vec3(1.0f, 0.2f, 1.0f))
		{
			// �ϐ��̏�����
			m_setFlg = true;
		}

		~GoalRail() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief	 ���̃R���W�����ƏՓ˂������Ɏ��s�����֐�
		@param[in]	Other �Փ˂����I�u�W�F�N�g�̃|�C���^
		*/
		void OnCollisionEnter(shared_ptr<GameObject>& Other) override;

	};
}
