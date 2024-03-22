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
	@brief プレイヤーフラグ
	*/
	enum class ePlayerFlags : uint8_t
	{
		IsClafting,	// クラフト中か？
		IsMining, // 採掘中か？
		IsDash,	// ダッシュしたか？
	};

	/*!
	@brief プレイヤー
	*/
	class Player : public TemplateObject
	{
		weak_ptr<TemplateObject> m_indicator; // セレクトインディケーター

		shared_ptr<PNTStaticDraw> m_ptrDraw;  // 描画コンポーネント
		shared_ptr<CollisionObb> m_ptrColl;   // コリジョンOBBコンポーネント
		Bool8_t<ePlayerFlags> m_flags;		  // フラグ管理クラス

		const float m_speed; // 速度

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		Player(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr, Vec3(0.0f, 1.5f, 0.0f), Vec3(0.0f), Vec3(1.0f, 2.0f, 1.0f)),
			m_speed(5.0f)
		{
			m_flags = 0;
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~Player() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief 採掘時に呼び出される関数
		*/
		void OnMining();

		/*!
		@brief クラフト時に呼び出される関数
		*/
		void OnCraft();

		/*!
		@brief 移動更新関数
		*/
		void UpdateMove();

		/*!
		@brief コントローラー回転関数
		*/
		void ControllerRotation(const Vec3& stickValue);

		/*!
		@brief コントローラー移動関数
		*/
		void ControllerMovement(const Vec3& stickValue);
	};
}