/*!
@file CraftingQTE.h
@brief クラフト中のQTE
@author 小澤博貴
*/

#pragma once
#include "Sprite.h"

namespace basecross
{
	/*!
	@brief クラフトQTE
	*/
	class CraftingQTE : public Sprite
	{
		weak_ptr<Sprite> m_barFlame; // バーのフレーム
		weak_ptr<Sprite> m_qtePoint; // QTEのタイミング範囲

		const Vec2 m_barScale;  // QTEバーのスケール
		const float m_barSpeed; // QTEバーの速度
		float m_barRatio; // バーの進んだ割合
		float m_qteRatio; // QTEの成功割合

	public:

		/*!
		@brief コンストラクタ
		*/
		CraftingQTE(const shared_ptr<Stage>& stagePtr) :
			Sprite(stagePtr, L"WHITE_TX", Vec2(0.0f, 25.0f), Vec3(0.0f)),
			m_barScale(Vec2(200.0f, 10.0f)),
			m_barSpeed(1.0f)
		{
			m_barRatio = 0.0f;
			m_qteRatio = 0.0f;
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~CraftingQTE() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief QTEの更新関数
		*/
		void UpdateQTE();

		/*!
		@brief QTE開始関数
		@param クラフトウィンドウの座標
		*/
		void StartQTE(const Vec3& windowPos);

		/*!
		@brief QTE停止とQTE結果取得関数
		@return 成功したかの真偽
		*/
		bool StopQTE();
	};
}