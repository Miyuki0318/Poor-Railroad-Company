/*!
@file Train.h
@brief 列車の親
@author 矢吹悠葉
*/

#pragma once
#include "stdafx.h"
#include "TemplateObject.h"

#define FLONT_CSV Vec2(0.0f, 1.0f)
#define BACK_CSV Vec2(0.0f, -1.0f)
#define RIGHT_CSV Vec2(1.0f, 0.0f)
#define LEFT_CSV Vec2(-1.0f, 0.0f)

namespace basecross {

	enum class eDirection
	{
		Right,	// 右
		Left,	// 左
		Flont,	// 前
		Back,	// 奥
	};

	class Train : public TemplateObject
	{
	protected:
		enum class State {
			None,		// 待機
			GameStart,	// ゲームスタート
			OnRail,		// 線路上
			Derail,		// 脱線
			Arrival,	// 到着
		};
		State m_state;

		const Vec3 m_DefaultPosition;
		const Vec3 m_DefaultScale;
		const float m_defSpeed;
		float m_moveSpeed;

		pair<Vec3, Vec3> m_movePos;
		string m_railPos;
		float m_moveRatio;
		eDirection m_direction;
		map<float, eDirection> m_radMap;
		map<eDirection, vector<Vec2>> m_drMap;
		Mat4x4 m_modelMat;

		Vec3 m_moveDirection;

		shared_ptr<PNTStaticModelDraw> m_ptrDraw;
		shared_ptr<CollisionObb> m_ptrColl;

	public:
		Train(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr),
			m_DefaultPosition(Vec3(3.0f, 1.0f, -7.0f)),
			m_DefaultScale(Vec3(1.0f, 1.0f, 1.0f)),
			m_defSpeed(0.2f),
			m_moveDirection(Vec3(0.0f))
		{
			m_state = State::None;

			m_moveSpeed = m_defSpeed;
			m_moveRatio = 0.0f;
			m_railPos = { 0, 0 };
			m_direction = eDirection::Right;
			m_movePos.first.zero();
			m_movePos.second.zero();

			m_radMap.insert(make_pair(XM_PI, eDirection::Right));
			m_radMap.insert(make_pair(0.0f, eDirection::Left));
			m_radMap.insert(make_pair(XM_PIDIV2, eDirection::Flont));
			m_radMap.insert(make_pair(-XM_PIDIV2, eDirection::Back));

			m_drMap.insert(make_pair(eDirection::Right, vector<Vec2>{RIGHT_CSV, FLONT_CSV, BACK_CSV}));
			m_drMap.insert(make_pair(eDirection::Left, vector<Vec2>{LEFT_CSV, FLONT_CSV, BACK_CSV}));
			m_drMap.insert(make_pair(eDirection::Flont, vector<Vec2>{RIGHT_CSV, LEFT_CSV, FLONT_CSV}));
			m_drMap.insert(make_pair(eDirection::Back, vector<Vec2>{RIGHT_CSV, LEFT_CSV, BACK_CSV}));

			m_modelMat.affineTransformation(
				Vec3(0.03f),
				Vec3(0.0f),
				Vec3(0.0f, -XM_PIDIV2, 0.0f),
				Vec3(-0.5f, 0.0f, -0.075f)
			);
		}
		Train(const shared_ptr<Stage>& stagePtr, const Vec3& position) :
			TemplateObject(stagePtr),
			m_DefaultPosition(position),
			m_DefaultScale(Vec3(1.0f, 1.0f, 1.0f)),
			m_moveDirection(Vec3(0.0f)),
			m_defSpeed(0.2f)
		{
			m_state = State::None;
			m_moveSpeed = m_defSpeed;
			m_moveRatio = 0.0f;
			m_railPos = { 0, 0 };
			m_direction = eDirection::Right;
			m_movePos.first.zero();
			m_movePos.second.zero();

			m_radMap.insert(make_pair(XM_PI, eDirection::Right));
			m_radMap.insert(make_pair(0.0f, eDirection::Left));
			m_radMap.insert(make_pair(XM_PIDIV2, eDirection::Flont));
			m_radMap.insert(make_pair(-XM_PIDIV2, eDirection::Back));

			m_drMap.insert(make_pair(eDirection::Right, vector<Vec2>{RIGHT_CSV, FLONT_CSV, BACK_CSV}));
			m_drMap.insert(make_pair(eDirection::Left, vector<Vec2>{LEFT_CSV, FLONT_CSV, BACK_CSV}));
			m_drMap.insert(make_pair(eDirection::Flont, vector<Vec2>{RIGHT_CSV, LEFT_CSV, FLONT_CSV}));
			m_drMap.insert(make_pair(eDirection::Back, vector<Vec2>{RIGHT_CSV, LEFT_CSV, BACK_CSV}));

			m_modelMat.affineTransformation(
				Vec3(0.03f),
				Vec3(0.0f),
				Vec3(0.0f, -XM_PIDIV2, 0.0f),
				Vec3(-0.5f, 0.0f, -0.075f)
			);
		}

		~Train() {}

		virtual void OnCreate() override;

		/// <summary>
		/// 移動処理
		/// </summary>
		/// <param name="nextState">移動先が見つからない場合に移行するステート</param>
		void MoveProcess(State nextState);

		/// <summary>
		/// 次のレールを検索する処理
		/// </summary>
		/// <returns>見つかったかどうか</returns>
		virtual bool SearchNextRail();

		/// <summary>
		/// 次のレールへの方向を設定する処理
		/// </summary>
		virtual void SetNextRailDirection();

		/// <summary>
		/// 初期位置の取得
		/// </summary>
		/// <returns>初期位置</returns>
		virtual Vec3 GetDefaultPosition()
		{
			return m_DefaultPosition;
		}
	};
}
