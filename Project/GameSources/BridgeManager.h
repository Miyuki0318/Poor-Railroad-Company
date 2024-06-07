/*!
@file BridgeManager.h
@brief 木の足場管理クラス
@author 小澤博貴
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	/*!
	@brief 木の足場管理用クラス
	*/
	class BridgeManager : public GameObject
	{
		shared_ptr<PNTStaticInstanceDraw> m_ptrDraw; // 描画コンポーネント

		const Mat4x4 m_mtxScale;	// 行列計算に使うスケール
		const Mat4x4 m_mtxRotation;	// 行列計算に使うローテーション

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		BridgeManager(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr),
			m_mtxScale((Mat4x4)XMMatrixScalingFromVector(Vec3(0.35f))),
			m_mtxRotation((Mat4x4)XMMatrixRotationRollPitchYawFromVector(Vec3(0.0f, XM_PIDIV2, 0.0f)))
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~BridgeManager() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 初期化関数
		*/
		void ResetBridge();

		/*!
		@brief レール追加関数
		@param 選択ポイント
		*/
		void AddBridge(const Point2D<size_t>& point);

		/*!
		@brief 現在のポイントの位置が水場になっているか取得関数
		@param 選択ポイント
		@return 一致してたらtrue
		*/
		bool GetIsWaterPoint(const Point2D<size_t>& point) const;
	};
}