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
		shared_ptr<PCTStaticInstanceDraw> m_ptrDraw;

		VertexData m_vertex;  // 頂点データ
		Mat4x4 m_mtxScale;	  // インスタンス描画用のスケール
		Mat4x4 m_mtxRotation; // インスタンス描画用のローテーション

		float m_posY; // ポジションY軸

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		RailGuide(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr),
			m_posY(1.5f)
		{
			Quat quatRot;
			quatRot.rotationRollPitchYawFromVector(Vec3(0.0f));
			m_mtxRotation.rotation(quatRot);
			m_mtxScale.scale(Vec3(1.0f));
			Utility::SimpleVerticesIndices(m_vertex);
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
		@brief ビルボードの更新処理関数
		*/
		void UpdateBillboard();

		/*!
		@brief ガイドの配置更新処理関数
		*/
		void UpdateGuide();
	};
}