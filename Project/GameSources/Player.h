/*!
@file Player.h
@brief プレイヤーオブジェクト
@author 小澤博貴
*/

#pragma once
#include "TemplateObject.h"
#include "SelectIndicator.h"
#include "CraftManager.h"
#include "PlayerState.h"

namespace basecross
{
	// プレイヤーの状態
	enum class ePlayerStatus : uint16_t
	{
		IsIdle,		// 待機状態
		IsMove,		// 移動状態
		IsRotate,	// 回転状態
		IsMining,	// 採掘状態
		IsHaveWood,	// 木を所持中
		IsHaveStone,// 石を所持中
		IsCrafting,	// クラフト中
		IsCraftQTE,	// クラフトQTE中
		IsHaveRail,	// 線路所持中
	};

	// プレイヤーの状態ステートクラス(名前のみ宣言)
	class PlayerIdleState;		// 待機状態
	class PlayerMovingState;	// 移動状態
	class PlayerMiningState;	// 採掘状態
	class PlayerCraftingState;	// クラフト状態

	/*!
	@brief プレイヤー
	*/
	class Player : public TemplateObject
	{
		weak_ptr<SelectIndicator> m_indicator; // セレクトインディケーター
		unique_ptr<CraftManager> m_craft;      // クラフトマネージャー

		shared_ptr<PNTStaticDraw> m_ptrDraw;   // 描画コンポーネント
		shared_ptr<CollisionObb> m_ptrColl;    // コリジョンOBBコンポーネント
		Bool16_t<ePlayerStatus> m_status;	   // フラグ管理クラス
		
		map<wstring, eItemType> m_miningMap;     // 採掘対象と取得アイテムタイプ

		// ステートマシン
		unique_ptr<PlayerStateMachine> m_playerState; 

		const float m_moveSpeed; // 移動速度
		const float m_rotSpeed;  // 回転速度

		Vec3 m_rotTarget; // 回転先
		Vec3 m_currentRot;  // 前回の回転軸

		// フレンド化(ステートマシンからメンバ関数を呼び出すため)
		friend PlayerIdleState;
		friend PlayerMovingState;
		friend PlayerMiningState;
		friend PlayerCraftingState;

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		Player(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr, Vec3(0.0f, 3.0f, 0.0f), Vec3(0.0f), Vec3(1.0f, 1.5f, 1.0f)),
			m_moveSpeed(5.0f), // 今後CSVから速度等のステータスを取得予定
			m_rotSpeed(0.5f)  // 今後CSVから速度等のステータスを取得予定
		{
			m_status = 0; // 状態フラグは0で初期化
			m_rotTarget.zero(); // 回転先は0.0fで初期化
			m_currentRot.zero(); // 回転先は0.0fで初期化

			// 採掘オブジェクトのタグと採掘時に加算するアイテムのタイプ
			m_miningMap.insert(make_pair(L"Tree", eItemType::Wood));	// タグか木ならアイテムタイプは木材
			m_miningMap.insert(make_pair(L"Rock", eItemType::Stone)); // タグが岩ならアイテムタイプは石材
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~Player() 
		{
			m_craft.reset();
		}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

	private:

		/*!
		@brief インディケーターへの取得と呼び出し関数
		*/
		void IndicatorOrder();

		/*!
		@brief 採掘時に呼び出される関数
		@param 採掘されるオブジェクトのポインタ
		*/
		void MiningProcces(const shared_ptr<TemplateObject>& miningObj);

		/*!
		@brief レール設置時に呼び出される関数
		@param レールを設置する座標
		*/
		void AddRailed(const Vec3& railPosition);

		/*!
		@brief クラフト画面切り替え関数
		*/
		void SwitchCraftWindow();

		/*!
		@brief クラフトQTEが終わっているかの確認関数
		*/
		void CheckedCraftQTE();

		/*!
		@brief アイテム状態の更新関数
		*/
		void UpdateStatus();

		/*!
		@brief 移動更新関数
		*/
		void UpdateMove();

		/*!
		@brief 回転更新関数
		*/
		void UpdateRotation();

		/*!
		@brief 回転先設定関数
		@param Lスティック入力量
		*/
		void SetRotateTarget(const Vec3& stickValue);

		/*!
		@brief コントローラー移動関数
		@param Lスティック入力量
		*/
		void ControllerMovement(const Vec3& stickValue);

		/*!
		@brief State変更関数
		@param 新しいステートのポインタ
		*/
		void SetState(const shared_ptr<PlayerState>& newState)
		{
			m_playerState->SetState(newState);
		}
			
		/*!
		@brief アイテム数取得関数
		@param アイテムタイプenum
		@return アイテム数
		*/
		int GetItemCount(eItemType type)
		{
			return m_craft->GetItemCount(type);
		}

		/*!
		@brief アイテム数追加関数
		@param アイテムタイプenum
		@param 追加数(デフォ1)
		*/
		void AddItemCount(eItemType type, int addNum = 1)
		{
			m_craft->AddItemCount(type, addNum);
		}

	public:

		/*!
		@brief 状態取得関数
		@param プレイヤーの状態enum
		@return その状態になっているかの真偽
		*/
		bool GetStatus(ePlayerStatus status) const
		{
			return m_status(status);
		}
	};
}