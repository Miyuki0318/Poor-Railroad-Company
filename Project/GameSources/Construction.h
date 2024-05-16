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
		const Vec3 m_position = Vec3(40.0f, 1.0f, -25.0f);
		
		const wstring tagName = App::GetApp()->GetScene<Scene>()->GetTagName();
		
		Mat4x4 m_spanMat;

		shared_ptr<PNTStaticDraw> drawComp;

		shared_ptr<Sprite> m_fadeSprite;

		shared_ptr<GameObject> m_obj;

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

		// ゲームスタートさせる関数
		void GameStart();

		// 電車の移動処理関数
		void MoveTrain();
	};
}