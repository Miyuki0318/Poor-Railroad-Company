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

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		BridgeManager(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr)
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