/*!
@file RailGuide.h
@brief レールを設置する場所のガイド表示
@author 小澤博貴
*/

#pragma once
#include "TemplateObject.h"

namespace basecross
{
	/*!
	@brief レール設置ガイド
	*/
	class RailGuide : public TemplateObject
	{
		// 描画コンポーネント
		shared_ptr<PNTStaticInstanceDraw> m_ptrDraw;

		Mat4x4 m_mtxScale;	   // インスタンス描画用のスケール
		Mat4x4 m_mtxRotation;  // インスタンス描画用のローテーション
		const Col4 m_defColor; // デフォルトの色

		float m_posY;		// ポジションY軸
		float m_blinkRatio;	// 点滅割合
		
		const float m_blinkTime; // 点滅する時間

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		RailGuide(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr),
			m_defColor(1.0f, 1.0f, 1.0f, 0.5f),
			m_posY(1.0f),
			m_blinkRatio(0.0f),
			m_blinkTime(0.5f)
		{
			Quat quatRot;
			quatRot.rotationRollPitchYawFromVector(Vec3(0.0f, XM_PIDIV2, 0.0f));
			m_mtxRotation.rotation(quatRot);
			m_mtxScale.scale(Vec3(0.675f));
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~RailGuide() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief 点滅の更新処理関数
		*/
		void UpdateBlinking();

		/*!
		@brief ガイドの配置更新処理関数
		*/
		void UpdateGuide();
	};
}