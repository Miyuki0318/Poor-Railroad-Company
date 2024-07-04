/*!
@file Company.h
@brief 会社の設定(メニュー画面)
@author 新妻葵
*/

#pragma once
#include "TemplateObject.h"
#include "Sprite.h"
#include "Scene.h"

namespace basecross {
	class Company : public TemplateObject
	{
		const Vec3 m_position = Vec3(10.0f, 1.0f, -10.0f);

		const Vec2 m_scale = Vec2(400.0f);

		const wstring tagName = App::GetApp()->GetScene<Scene>()->GetTagName();
		
		Mat4x4 m_spanMat;

		shared_ptr<Transform> transComp;
		shared_ptr<PNTStaticDraw> drawComp;

		shared_ptr<Sprite> m_menuSprite;

	public:
		Company(const shared_ptr<Stage>& stagePtr // ステージのポインタ
		) :
			TemplateObject(stagePtr)
		{
			m_spanMat.affineTransformation(
				Vec3(1.8f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f)
			);
		}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		/*
		* オプション画面の処理
		*/
		void OptionMenu();
	};
}