/*!
@file Player.h
@brief プレイヤーオブジェクト
@author 小澤博貴
*/

#pragma once
#include "TemplateObject.h"

namespace basecross
{
	// プレイヤーのアニメーションキータイプ
	enum class ePAKey : char
	{
		Wait,			// 待機
		Walk,			// 移動
		Harvesting,		// 採取
		CraftStart,		// クラフト開始
		Crafting,		// クラフト中
		CraftFinish,	// クラフト終了
		QTESucces,		// QTE成功
		QTEFailed,		// QTE失敗
	};

	/*!
	@brief アニメーションデータ構造体
	*/
	struct AnimationMap
	{
		wstring animeKey;	// アニメーションキー
		UINT flameNum;		// フレーム数
		bool loopActive;	// ループするかの真偽

		/*!
		@brief コンストラクタ
		@param アニメーションキー
		@param 開始時のフレーム
		@param 終了時のフレーム
		@param ループするか(デフォルトはfalse)
		*/
		AnimationMap(wstring aniKey, UINT frame, bool loop = false) :
			animeKey(aniKey),
			flameNum(frame),
			loopActive(loop)
		{
		}
	};

	/*!
	@brief プレイヤー
	*/
	class Player : public TemplateObject
	{
	protected:

		// 描画コンポーネント
		shared_ptr<BcPNTBoneModelDraw> m_ptrDraw; 
		shared_ptr<Shadowmap> m_ptrShadow; 

		const float m_radius;	 // 衝突判定用半径
		const float m_moveSpeed; // 移動速度
		const float m_rotSpeed;  // 回転速度

		// 通れないステージIDリスト
		set<eStageID> m_impassableSet; 

		// アニメーションマップ
		map<ePAKey, AnimationMap> m_animationMap;

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

			// アニメーションキー
			m_animationMap.emplace(ePAKey::Wait, AnimationMap(L"WAIT", 24, true));			  // 待機
			m_animationMap.emplace(ePAKey::Walk, AnimationMap(L"WALK", 12, true));			  // 移動
			m_animationMap.emplace(ePAKey::Harvesting, AnimationMap(L"HARVESTING", 24, false)); // 伐採
			m_animationMap.emplace(ePAKey::CraftStart, AnimationMap(L"C_START", 10, false));	  // クラフト開始
			m_animationMap.emplace(ePAKey::Crafting, AnimationMap(L"C_NOW", 15, true));		  // クラフト中
			m_animationMap.emplace(ePAKey::CraftFinish, AnimationMap(L"C_END", 10, false));	  // クラフト終了
			m_animationMap.emplace(ePAKey::QTESucces, AnimationMap(L"SUCCES", 24, false));	  // QTE成功
			m_animationMap.emplace(ePAKey::QTEFailed, AnimationMap(L"FAILED", 24, false));	  // QTE失敗

			// スケールだけ、Y軸方向に2倍にする
			m_modelMat.affineTransformation(
				Vec3(0.5f),
				Vec3(0.0f),
				Vec3(0.0f, -XM_PIDIV2, 0.0f),
				Vec3(0.0f, -1.05f, 0.0f)
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
		@brief アニメーションメッシュの更新
		@param animationKey
		*/
		virtual void SetAnimationMesh(ePAKey animation, float start = 0.0f);

		/*!
		@brief 指定したアニメーションかのチェック
		@param animationKey
		@return 一致してたらtrue
		*/
		virtual bool IsAnimation(ePAKey animation);

		/*!
		@brief 指定したアニメーションが終了しているかのチェック
		@param animationKey
		@return 終了してたらtrue
		*/
		virtual bool IsAnimeEnd(ePAKey animation);

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
		virtual void GridHitFlontResponse(Vec3& pos, const vector<vector<Vec3>>& posMap);

		/*!
		@brief 移動出来ないグリッド(下)埋まり解除関数
		@param ポジション
		@param ステージマップ
		*/
		virtual void GridHitBackResponse(Vec3& pos, const vector<vector<Vec3>>& posMap);

		/*!
		@brief 移動出来ないグリッド(左)埋まり解除関数
		@param ポジション
		@param ステージマップ
		*/
		virtual void GridHitLeftResponse(Vec3& pos, const vector<vector<Vec3>>& posMap);

		/*!
		@brief 移動出来ないグリッド(右)埋まり解除関数
		@param ポジション
		@param ステージマップ
		*/
		virtual void GridHitRightResponse(Vec3& pos, const vector<vector<Vec3>>& posMap);

		/*!
		@brief 移動出来ないグリッド(左上)埋まり解除関数
		@param ポジション
		@param ステージマップ
		*/
		virtual void GridHitLeftFlontResponse(Vec3& pos, const vector<vector<Vec3>>& posMap);

		/*!
		@brief 移動出来ないグリッド(左下)埋まり解除関数
		@param ポジション
		@param ステージマップ
		*/
		virtual void GridHitLeftBackResponse(Vec3& pos, const vector<vector<Vec3>>& posMap);

		/*!
		@brief 移動出来ないグリッド(右上)埋まり解除関数
		@param ポジション
		@param ステージマップ
		*/
		virtual void GridHitRightFlontResponse(Vec3& pos, const vector<vector<Vec3>>& posMap);

		/*!
		@brief 移動出来ないグリッド(右下)埋まり解除関数
		@param ポジション
		@param ステージマップ
		*/
		virtual void GridHitRightBackResponse(Vec3& pos, const vector<vector<Vec3>>& posMap);

		/*!
		@brief ステージの範囲外へ行かない様にする関数
		@param ポジション
		@param ステージマップ
		*/
		virtual void GridRangeResponse(Vec3& pos, const vector<vector<int>>& stageMap);
	};
}