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
		shared_ptr<PCStaticDraw> m_ptrDraw; // 描画コンポーネント
		weak_ptr<TemplateObject> m_player; // プレイヤー
		Point2D<size_t> m_selectPoint; // 選択しているポイント
		vector<VertexPositionColor> m_vertices; // 頂点データ
		const vector<float> m_rotArray; // 角度配列
		const float m_deadZone; // 入力閾値

		float m_pastRotY;	// 前回の回転軸Y

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
			m_deadZone(0.1f),
			m_rotArray{ 0.0f, 45.0f, 90.0f, 135.0f, 180.0f, -45.0f, -90.0f, -135.0f, -180.0f }
		{
			m_pastRotY = 0.0f;
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
		@brief セレクターの位置に採取可能オブジェクトあるか確認する関数
		@return 存在するならtrue
		*/
		bool IsGatheringPoint() const;

		/*!
		@brief セレクターの位置にガイドレールあるか確認する関数
		@return 存在するならtrue
		*/
		bool IsGuideRailPoint() const;

		/*!
		@brief セレクターの位置が水場かどうか確認する関数
		@return 水場ならtrue
		*/
		bool IsWaterPoint() const;

		/*!
		@brief セレクターの位置に直線レールがあるか確認する関数
		@return 存在するならtrue
		*/
		bool IsStraightRailPoint() const;

		/*!
		@brief セレクターの位置に採取可能オブジェクトあるなら採取処理を行う関数
		@return 採取できていたらtrue
		*/
		int GatheringOrder() const;

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
		@brief セレクターの位置に踏切を設置可能なら設置処理を行う関数
		@return 設置できていたらtrue
		*/
		bool CrossingOrder() const;

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
		void UpdateCursolPosition(Vec3& position);

		/*!
		@brief コントローラー入力でカーソルの座標を求める関数
		@retrun カーソルの座標
		*/
		Vec3 ControllerPosition();

		/*!
		@brief マウスでカーソルの座標を求める関数
		@retrun カーソルの座標
		*/
		Vec3 MousePosition(const Vec3& playerPos);

		/*!
		@brief 選択ポイントの更新処理関数
		*/
		void UpdateSelectPoint();

		/*!
		@brief 選択ポイントの色設定関数
		@param 設定する色
		*/
		void SetVerticesColor(const Col4& color);
	};
}