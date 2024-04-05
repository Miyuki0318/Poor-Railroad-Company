/*!
@file Building.h
@brief �����̐ݒ�
@author �V�Ȉ�
*/

#pragma once
#include "TemplateObject.h"

namespace basecross {
	class Company : public TemplateObject
	{
		const Vec3 m_position;

	public:
		Company::Company(const shared_ptr<Stage>& stagePtr, // �X�e�[�W�̃|�C���^
			const Vec3 position
		) :
			TemplateObject(stagePtr),
			m_position(position)
		{
		}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}