/*!
@file Signboard.h
@brief �Ŕ�(�l���I��)�̐ݒ�
@author �V�Ȉ�
*/

#pragma once
#include "TemplateObject.h"

namespace basecross {
	class SignBoard : public TemplateObject
	{
		// �^�O���擾
		const wstring tagName = App::GetApp()->GetScene<Scene>()->GetTagName();

		const Vec3 m_scale = Vec3(3.0f);
		const Vec3 m_position = Vec3(15.0f,1.0f, -9.0f);

		Mat4x4 m_spanMat;
	
	public :
		SignBoard(const shared_ptr<Stage>& stagePtr) :
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
		@brief �l���ǉ�
		*/
		void BoardSelect();
	};
}