/*!
@file Input.h
@brief コントローラー入力取得
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	/*!
	@brief コントローラー入力取得関数群
	*/
	namespace Input
	{
		/*!
		@brief コントローラーステート取得関数
		@return CONTROLER_STATE
		*/
		inline const CONTROLER_STATE& GetPad()
		{
			// アプリケーションオブジェクトから入力デバイスを取得して返す
			return App::GetApp()->GetInputDevice().GetControllerVec()[0];
		}

		/*!
		@brief キーボードステート取得関数
		@return KEYBOARD_STATE
		*/
		inline const KEYBOARD_STATE& GetKeyboard()
		{
			// アプリケーションオブジェクトから入力デバイスを取得して返す
			return App::GetApp()->GetInputDevice().GetKeyState();
		}

		/*!
		@brief コントローラーが接続されているかの取得関数
		@return GetPad().bConnected
		*/
		inline bool GetPadConected()
		{
			return GetPad().bConnected;
		}

		/*!
		@brief 左クリックを押した瞬間の取得関数
		@return GetAsyncKeyState(VK_LBUTTON)
		*/
		inline bool GetPushLClick()
		{
			// 静的変数の宣言
			static unsigned int pastFrame = 0;	// 前回呼び出し時のフレーム
			static bool isPush = false;	// 押した瞬間かの真偽

			// 現在の経過フレーム
			unsigned int currentFrame = App::GetApp()->GetFrameCount();

			// 入力状態の取得
			SHORT keyState = GetAsyncKeyState(VK_LBUTTON);
			bool isDown = keyState & 0x8000; // 押されているか
			bool isUp = keyState & 0x0001;   // (現在のフレームで)押された瞬間か

			// フレームが違うならフレームを保持し、(押しているか && 押された瞬間か)を返す
			if (pastFrame != currentFrame)
			{
				pastFrame = currentFrame;
				return (isPush = isDown && isUp);
			}

			// フレームが同じなら(押した瞬間か && 押されているか)を返す
			return isPush && isDown;
		}

		/*!
		@brief Bボタンを押した瞬間の取得関数
		@return GetPad().wPressedButtons & XINPUT_GAMEPAD_B
		*/
		inline bool GetPushB()
		{
			// コントローラーが接続されてるなら
			if (GetPadConected())
			{
				// Aボタンが入力された瞬間かを返す
				return GetPad().wPressedButtons & XINPUT_GAMEPAD_B;
			}
			return GetPushLClick();
		}

		/*!
		@brief Aボタンを押した瞬間の取得関数
		@return GetPad().wPressedButtons & XINPUT_GAMEPAD_A
		*/
		inline bool GetPush()
		{
			// コントローラーが接続されてるなら
			if (GetPadConected())
			{
				// ボタンが入力された瞬間かを返す
				return GetPad().wPressedButtons;
			}

			// 右クリックが入力された瞬間かを返す
			if (GetPushLClick()) return true;

			// キーボードを取得し
			auto& key = GetKeyboard();
			for (size_t i = 0; i < key.MAX_KEYVCODE; i++)
			{
				// 何かしら入力があれば返す
				if (key.m_bPressedKeyTbl[i]) return true;
			}

			return false;
		}

		/*!
		@brief Aボタンを押した瞬間の取得関数
		@return GetPad().wPressedButtons & XINPUT_GAMEPAD_A
		*/
		inline bool GetButtonB()
		{
			// コントローラーが接続されてるなら
			if (GetPadConected())
			{
				// Aボタンが入力された瞬間かを返す
				return GetPad().wButtons & XINPUT_GAMEPAD_B;
			}
			return GetAsyncKeyState(VK_LBUTTON) & 0x8000;
		}

		/*!
		@brief Aボタンを押した瞬間の取得関数
		@return GetPad().wPressedButtons & XINPUT_GAMEPAD_A
		*/
		inline bool GetPushA()
		{
			// コントローラーが接続されてるなら
			if (GetPadConected())
			{
				// Aボタンが入力された瞬間かを返す
				return GetPad().wPressedButtons & XINPUT_GAMEPAD_A;
			}
			return GetKeyboard().m_bPressedKeyTbl[VK_RBUTTON];
		}

		/*!
		@brief Xボタンを押した瞬間の取得関数
		@return GetPad().wPressedButtons & XINPUT_GAMEPAD_X
		*/
		inline bool GetPushX()
		{
			// コントローラーが接続されてるなら
			if (GetPadConected())
			{
				// Aボタンが入力された瞬間かを返す
				return GetPad().wPressedButtons & XINPUT_GAMEPAD_X;
			}
			return GetKeyboard().m_bPressedKeyTbl['Q'];
		}

		/*!
		@brief Yボタンを押した瞬間の取得関数
		@return GetPad().wPressedButtons & XINPUT_GAMEPAD_A
		*/
		inline bool GetPushY()
		{
			// コントローラーが接続されてるなら
			if (GetPadConected())
			{
				// Aボタンが入力された瞬間かを返す
				return GetPad().wPressedButtons & XINPUT_GAMEPAD_Y;
			}
			return GetKeyboard().m_bPressedKeyTbl[VK_MBUTTON];
		}

		/*!
		@brief RBボタンを押している間の取得関数
		@return GetPad().wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER
		*/
		inline bool GetButtonRB()
		{
			if (GetPadConected())
			{
				// RBボタンが入力された瞬間かを返す
				return GetPad().wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
			}
			return false;
		}

		/*!
		@brief スタートボタンを押した瞬間の取得関数
		@return GetPad().wPressedButtons & XINPUT_GAMEPAD_START
		*/
		inline bool GetStartPush()
		{
			if (GetPadConected())
			{
				// スタートボタンが入力された瞬間かを返す
				return GetPad().wPressedButtons & XINPUT_GAMEPAD_START;
			}
			return GetKeyboard().m_bPressedKeyTbl[VK_TAB];
		}

		/*!
		@brief WASDの入力量取得関数
		@return Vec2(AD, WS)
		*/
		inline Vec2 GetWASDValue()
		{
			// キーボードWASDでの入力を設定
			Vec2 keyInput;
			auto& key = GetKeyboard();
			if (key.m_bPushKeyTbl['W']) keyInput.y += 1.0f;
			if (key.m_bPushKeyTbl['S']) keyInput.y -= 1.0f;
			if (key.m_bPushKeyTbl['A']) keyInput.x -= 1.0f;
			if (key.m_bPushKeyTbl['D']) keyInput.x += 1.0f;

			// 斜め入力なら
			if (keyInput.length() > 1.0f) keyInput *= 0.7f;

			return keyInput;
		}

		/*!
		@brief Lスティックの入力量取得関数
		@return Vec2(pad.fThumbLX, pad.fThumbLY)
		*/
		inline Vec2 GetLStickValue()
		{
			// コントローラーが接続されているなら
			if (GetPadConected())
			{
				// コントローラーを取得
				const auto& pad = GetPad();

				// コントローラーのLスティック入力量をVec2にして返す
				return Vec2(pad.fThumbLX, pad.fThumbLY);
			}

			// WASDでの入力量を返す
			return GetWASDValue();
		}

		/*!
		@brief Lスティックの入力されているかの真偽取得関数
		@return GetLStickValue().length() > 0.0f
		*/
		inline bool IsInputLStick()
		{
			// Lスティックの入力量の長さが0より大きいかの真偽返す
			return GetLStickValue().length() > 0.0f;
		}

		/*!
		@brief Lスティックの入力されているかの真偽をX軸のみで取得する関数
		@return stick.length() > 0.0f
		*/
		inline bool IsInputLStickX()
		{
			// Lスティックの入力量を取得
			Vec2 stick = GetLStickValue();
			stick.y = 0.0f; // Y軸を0に
			return stick.length() > 0.0f; // X軸のみで長さを比較
		}

		/*!
		@brief Lスティックの入力されているかの真偽をL軸のみで取得する関数
		@return stick.length() > 0.0f
		*/
		inline bool IsInputLStickY()
		{
			// Lスティックの入力量を取得
			Vec2 stick = GetLStickValue();
			stick.x = 0.0f; // X軸を0に
			return stick.length() > 0.0f; // Y軸のみでで長さを比較
		}

		/*!
		@brief 画面上のマウスの2D座標(Basecrossの座標系)を取得する関数
		@return マウス座標
		*/
		inline Vec2 GetMousePosition()
		{
			// キーステートからマウス座標を取得
			Vec2 mousePos = Vec2(0.0f);
			auto& keyState = GetKeyboard();

			// マウスが正常じゃなければ終了
			if (!keyState.m_MouseCheck) return mousePos;

			// 座標系が0.0中心であるため、画面サイズの半分を引く
			mousePos.x = float(keyState.m_MouseClientPoint.x - (int(WINDOW_WIDTH) / 2));
			mousePos.y = float(keyState.m_MouseClientPoint.y - (int(WINDOW_HEIGHT) / 2));
			mousePos.y *= -1.0f;

			// 返す
			return mousePos;
		}
	}
}