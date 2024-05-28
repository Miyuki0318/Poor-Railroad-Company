/*!
@file SelectIndicator.h
@brief セレクトインディケーター
@author 小澤博貴
*/

#pragma once
#include "TemplateObject.h"

namespace basecross
{
	/*!
	@brief 選択肢描画
	*/
	class SelectIndicator : public TemplateObject
	{
		VertexData m_vertex; // 頂点データ
		shared_ptr<PCTStaticDraw> m_ptrDraw; // 描画コンポーネント
		weak_ptr<TemplateObject> m_player; // プレイヤー
		Point2D<size_t> m_selectPoint; // 選択しているポイント
		vector<float> m_rotArray;

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param プレイヤーポインタ
		*/
		SelectIndicator(const shared_ptr<Stage>& stagePtr,
			const shared_ptr<TemplateObject>& playerPtr
		) :
			TemplateObject(stagePtr, Vec3(0.0f, 1.5f, 0.0f), Vec3(0.0f), Vec3(1.0f)),
			m_player(playerPtr),
			m_rotArray{0.0f, 45.0f, 90.0f, 135.0f, 180.0f, -45.0f, -90.0f, -135.0f, -180.0f}
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~SelectIndicator() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief セレクターの位置に採掘オブジェクトがあれば採掘処理を行う関数
		@return 採掘可能オブジェクトタグセットを返す
		*/
		set<wstring> MiningOrder() const;

		/*!
		@brief セレクターの位置にレールを設置可能なら設置処理を行う関数
		@return 設置できていたらtrue
		*/
		bool RailedOrder() const;

		/*!
		@brief セレクターの位置にレールを設置可能なら設置処理を行う関数
		@return 設置できていたらtrue
		*/
		bool BridgeOrder() const;

		/*!
		@brief セレクターの位置を取得する関数
		@return m_selectPoint
		*/
		const Point2D<size_t>& GetSelectPoint() const
		{
			return m_selectPoint;
		}

	private:

		/*!
		@brief 座標の更新処理関数
		*/
		void UpdatePosition();

		/*!
		@brief プレイヤーに隣接する場所に座標を設定する関数
		@param 現在の座標
		*/
		void PlayerAdjoinPosition(Vec3& position);

		/*!
		@brief 選択ポイントの更新処理関数
		*/
		void UpdateSelectPoint();
	};
}