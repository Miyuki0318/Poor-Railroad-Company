/*!
@file Construction.h
@brief 工事現場の設定(ゲームスタート)
@author 新妻葵
*/

#pragma once
#include "TemplateObject.h"

namespace basecross {
	class Construction :public TemplateObject
	{
		const Vec3 m_position = Vec3(65.0f, 1.0f, 0.0f);
		Mat4x4 m_spanMat;

		shared_ptr<Transform> transComp;
		shared_ptr<PNTStaticDraw> drawComp;

		shared_ptr<Sprite> m_fadeSprite;

		bool m_isSelect;

	public:
		Construction(const shared_ptr<Stage>& stagePtr // ステージのポインタ
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
		* ゲームスタート
		*/
		void StartCountDown();

		void SetSelectFlag(bool flag)
		{
			m_isSelect = flag;
		}
	};
}