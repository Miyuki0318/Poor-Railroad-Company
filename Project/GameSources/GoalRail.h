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
		// �g�����X�t�H�[���ƃ��f���̍����s��
		Mat4x4 m_modelMat;


	public:

		GoalRail(const shared_ptr<Stage>& stagePtr, const Vec3& position) :
			TemplateObject(stagePtr, Vec3(position.x, 1.0f, position.z), Vec3(0.0f), Vec3(0.8f, 0.2f, 0.8f))
		{
			// �ϐ��̏�����
			m_setFlg = true;
			m_modelMat.affineTransformation(
				Vec3(0.85f, 3.375f, 0.85f),
				Vec3(0.0f),
				Vec3(0.0f, XM_PIDIV2, 0.0f),
				Vec3(0.0f)
			);
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
