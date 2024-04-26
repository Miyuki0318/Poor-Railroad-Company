/*!
@file Company.h
@brief ��Ђ̐ݒ�(���j���[���)
@author �V�Ȉ�
*/

#pragma once
#include "TemplateObject.h"

namespace basecross {
	class Company : public TemplateObject
	{
		const Vec3 m_position = Vec3(35.0f, 1.0f, 10.0f);
		
		const wstring tagName = App::GetApp()->GetScene<Scene>()->GetTagName();
		
		Mat4x4 m_spanMat;

		shared_ptr<Transform> transComp;
		shared_ptr<PNTStaticDraw> drawComp;

		shared_ptr<Sprite> m_menuSprite;

	public:
		Company(const shared_ptr<Stage>& stagePtr // �X�e�[�W�̃|�C���^
		) :
			TemplateObject(stagePtr)
		{
			m_spanMat.affineTransformation(
				Vec3(1.0f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f)
			);
		}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		/*
		* �I�v�V������ʂ̏���
		*/
		void OptionMenu();
	};
}