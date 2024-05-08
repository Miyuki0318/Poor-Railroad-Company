/*!
@file Player.h
@brief プレイヤーオブジェクト
@author 小澤博貴
*/

#pragma once
#include "TemplateObject.h"

namespace basecross
{
	/*!
	@brief プレイヤー
	*/
	class Player : public TemplateObject
	{
	protected:

		shared_ptr<BcPNTBoneModelDraw> m_ptrDraw; // 描画コンポーネント

		const float m_radius;	 // 衝突判定用半径
		const float m_moveSpeed; // 移動速度
		const float m_rotSpeed;  // 回転速度

		set<eStageID> m_impassableSet; // 通れないステージIDリスト
		Vec3 m_rotTarget;	// 回転先
		Vec3 m_currentRot;  // 前回の回転軸
		Mat4x4 m_modelMat;	// モデルとトランスフォーム差分行列

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		Player(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr, Vec3(0.0f, 3.0f, 0.0f), Vec3(0.0f), Vec3(1.0f)),
			m_moveSpeed(5.0f), // 今後CSVから速度等のステータスを取得予定
			m_rotSpeed(0.5f),  // 今後CSVから速度等のステータスを取得予定
			m_radius(1.0f)
		{
			m_rotTarget.zero(); // 回転先は0.0fで初期化
			m_currentRot.zero(); // 回転先は0.0fで初期化

			// スケールだけ、Y軸方向に2倍にする
			m_modelMat.affineTransformation(
				Vec3(0.6f),
				Vec3(0.0f),
				Vec3(0.0f, -XM_PIDIV2, 0.0f),
				Vec3(0.0f, -1.0f, 0.0f)
			);
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~Player() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		virtual void OnCreate() override;

	protected:

		/*!
		@brief コンポーネントの生成関数
		*/
		virtual void CreateComponent() = 0;

		/*!
		@brief 移動更新関数
		*/
		virtual void UpdateMove() = 0;

		/*!
		@brief 回転更新関数
		*/
		virtual void UpdateRotation();

		/*!
		@brief 回転先設定関数
		@param Lスティック入力量
		*/
		virtual void SetRotateTarget(const Vec3& stickValue);

		/*!
		@brief コントローラー移動関数
		@param Lスティック入力量
		*/
		virtual void ControllerMovement(const Vec3& stickValue);

		/*!
		@brief 移動出来ないグリッド埋まり解除関数
		@param ポジション
		*/
		virtual void GridHitResponse(Vec3& pos);

	private:

		/*!
		@brief 指定した行列が衝突判定を取るグリッドかの真偽取得関数
		@param row
		@param col
		@return bool 衝突判定を取るグリッドならtrue,取らないグリッドならfalse
		*/
		virtual bool GetIsImpassable(size_t row, size_t col);

		/*!
		@brief 移動出来ないグリッド(上)埋まり解除関数
		@param ポジション
		@param ステージマップ
		*/
		virtual void GridHitFlontResponse(Vec3& pos);

		/*!
		@brief 移動出来ないグリッド(下)埋まり解除関数
		@param ポジション
		@param ステージマップ
		*/
		virtual void GridHitBackResponse(Vec3& pos);

		/*!
		@brief 移動出来ないグリッド(左)埋まり解除関数
		@param ポジション
		@param ステージマップ
		*/
		virtual void GridHitLeftResponse(Vec3& pos);

		/*!
		@brief 移動出来ないグリッド(右)埋まり解除関数
		@param ポジション
		@param ステージマップ
		*/
		virtual void GridHitRightResponse(Vec3& pos);
	};
}