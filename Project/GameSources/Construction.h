/*!
@file Construction.h
@brief �H������̐ݒ�(�Q�[���X�^�[�g)
@author �V�Ȉ�
*/

#pragma once
#include "TemplateObject.h"

namespace basecross {
	class Construction :public TemplateObject
	{
		const Vec3 m_position = Vec3(65.0f, 1.0f, 0.0f);
		
		const wstring tagName = App::GetApp()->GetScene<Scene>()->GetTagName();
		
		Mat4x4 m_spanMat;

		shared_ptr<Transform> transComp;
		shared_ptr<PNTStaticDraw> drawComp;

		shared_ptr<Sprite> m_fadeSprite;

	public:
		Construction(const shared_ptr<Stage>& stagePtr // �X�e�[�W�̃|�C���^
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
		* �Q�[���X�^�[�g
		*/
		void StartCountDown();
	};
}