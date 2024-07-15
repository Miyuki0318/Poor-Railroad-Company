/*!
@file Input.h
@brief �R���g���[���[���͎擾
*/

#pragma once
#include "stdafx.h"
#include "OzawaUtility.h"

namespace basecross
{
	/*!
	@brief �R���g���[���[���͎擾�֐��Q
	*/
	namespace Input
	{
		/*!
		@brief �R���g���[���[�X�e�[�g�擾�֐�
		@return CONTROLER_STATE
		*/
		inline const CONTROLER_STATE& GetPad()
		{
			// �A�v���P�[�V�����I�u�W�F�N�g������̓f�o�C�X���擾���ĕԂ�
			return App::GetApp()->GetInputDevice().GetControllerVec()[0];
		}

		/*!
		@brief �L�[�{�[�h�X�e�[�g�擾�֐�
		@return KEYBOARD_STATE
		*/
		inline const KEYBOARD_STATE& GetKeyboard()
		{
			// �A�v���P�[�V�����I�u�W�F�N�g������̓f�o�C�X���擾���ĕԂ�
			return App::GetApp()->GetInputDevice().GetKeyState();
		}

		/*!
		@brief �R���g���[���[���ڑ�����Ă��邩�̎擾�֐�
		@return GetPad().bConnected
		*/
		inline bool GetPadConected()
		{
			return GetPad().bConnected;
		}

		/*!
		@brief ���N���b�N���������u�Ԃ̎擾�֐�
		@return GetAsyncKeyState(VK_LBUTTON)
		*/
		inline bool GetPushLClick()
		{
			// �ÓI�ϐ��̐錾
			static unsigned int pastFrame = 0;	// �O��Ăяo�����̃t���[��
			static bool isPush = false;	// �������u�Ԃ��̐^�U

			// ���݂̌o�߃t���[��
			unsigned int currentFrame = App::GetApp()->GetFrameCount();

			// ���͏�Ԃ̎擾
			SHORT keyState = GetAsyncKeyState(VK_LBUTTON);
			bool isDown = keyState & 0x8000; // ������Ă��邩
			bool isUp = keyState & 0x0001;   // (���݂̃t���[����)�����ꂽ�u�Ԃ�

			// �t���[�����Ⴄ�Ȃ�t���[����ێ����A(�����Ă��邩 && �����ꂽ�u�Ԃ�)��Ԃ�
			if (pastFrame != currentFrame)
			{
				pastFrame = currentFrame;
				return (isPush = isDown && isUp);
			}

			// �t���[���������Ȃ�(�������u�Ԃ� && ������Ă��邩)��Ԃ�
			return isPush && isDown;
		}

		/*!
		@brief B�{�^�����������u�Ԃ̎擾�֐�
		@return GetPad().wPressedButtons & XINPUT_GAMEPAD_B
		*/
		inline bool GetPushB()
		{
			// �R���g���[���[���ڑ�����Ă�Ȃ�
			if (GetPadConected())
			{
				// A�{�^�������͂��ꂽ�u�Ԃ���Ԃ�
				return GetPad().wPressedButtons & XINPUT_GAMEPAD_B;
			}
			return GetPushLClick();
		}

		/*!
		@brief A�{�^�����������u�Ԃ̎擾�֐�
		@return GetPad().wPressedButtons & XINPUT_GAMEPAD_A
		*/
		inline bool GetPush()
		{
			// �R���g���[���[���ڑ�����Ă�Ȃ�
			if (GetPadConected())
			{
				// �{�^�������͂��ꂽ�u�Ԃ���Ԃ�
				return GetPad().wPressedButtons;
			}

			// �E�N���b�N�����͂��ꂽ�u�Ԃ���Ԃ�
			if (GetPushLClick()) return true;

			// �L�[�{�[�h���擾��
			auto& key = GetKeyboard();
			for (size_t i = 0; i < key.MAX_KEYVCODE; i++)
			{
				// ����������͂�����ΕԂ�
				if (key.m_bPressedKeyTbl[i]) return true;
			}

			return false;
		}

		/*!
		@brief A�{�^�����������u�Ԃ̎擾�֐�
		@return GetPad().wPressedButtons & XINPUT_GAMEPAD_A
		*/
		inline bool GetButtonB()
		{
			// �R���g���[���[���ڑ�����Ă�Ȃ�
			if (GetPadConected())
			{
				// A�{�^�������͂��ꂽ�u�Ԃ���Ԃ�
				return GetPad().wButtons & XINPUT_GAMEPAD_B;
			}
			return GetAsyncKeyState(VK_LBUTTON) & 0x8000;
		}

		/*!
		@brief A�{�^�����������u�Ԃ̎擾�֐�
		@return GetPad().wPressedButtons & XINPUT_GAMEPAD_A
		*/
		inline bool GetPushA()
		{
			// �R���g���[���[���ڑ�����Ă�Ȃ�
			if (GetPadConected())
			{
				// A�{�^�������͂��ꂽ�u�Ԃ���Ԃ�
				return GetPad().wPressedButtons & XINPUT_GAMEPAD_A;
			}
			return GetKeyboard().m_bPressedKeyTbl[VK_RBUTTON];
		}

		/*!
		@brief X�{�^�����������u�Ԃ̎擾�֐�
		@return GetPad().wPressedButtons & XINPUT_GAMEPAD_X
		*/
		inline bool GetPushX()
		{
			// �R���g���[���[���ڑ�����Ă�Ȃ�
			if (GetPadConected())
			{
				// A�{�^�������͂��ꂽ�u�Ԃ���Ԃ�
				return GetPad().wPressedButtons & XINPUT_GAMEPAD_X;
			}
			return GetKeyboard().m_bPressedKeyTbl['Q'];
		}

		/*!
		@brief Y�{�^�����������u�Ԃ̎擾�֐�
		@return GetPad().wPressedButtons & XINPUT_GAMEPAD_A
		*/
		inline bool GetPushY()
		{
			// �R���g���[���[���ڑ�����Ă�Ȃ�
			if (GetPadConected())
			{
				// A�{�^�������͂��ꂽ�u�Ԃ���Ԃ�
				return GetPad().wPressedButtons & XINPUT_GAMEPAD_Y;
			}
			return GetKeyboard().m_bPressedKeyTbl[VK_MBUTTON];
		}

		/*!
		@brief RB�{�^���������Ă���Ԃ̎擾�֐�
		@return GetPad().wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER
		*/
		inline bool GetButtonRB()
		{
			if (GetPadConected())
			{
				// RB�{�^�������͂��ꂽ�u�Ԃ���Ԃ�
				return GetPad().wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
			}
			return false;
		}

		/*!
		@brief �X�^�[�g�{�^�����������u�Ԃ̎擾�֐�
		@return GetPad().wPressedButtons & XINPUT_GAMEPAD_START
		*/
		inline bool GetStartPush()
		{
			if (GetPadConected())
			{
				// �X�^�[�g�{�^�������͂��ꂽ�u�Ԃ���Ԃ�
				return GetPad().wPressedButtons & XINPUT_GAMEPAD_START;
			}
			return GetKeyboard().m_bPressedKeyTbl[VK_TAB];
		}

		/*!
		@brief WASD�̓��͗ʎ擾�֐�
		@return Vec2(AD, WS)
		*/
		inline Vec2 GetWASDValue()
		{
			// �L�[�{�[�hWASD�ł̓��͂�ݒ�
			Vec2 keyInput;
			auto& key = GetKeyboard();
			if (key.m_bPushKeyTbl['W']) keyInput.y += 1.0f;
			if (key.m_bPushKeyTbl['S']) keyInput.y -= 1.0f;
			if (key.m_bPushKeyTbl['A']) keyInput.x -= 1.0f;
			if (key.m_bPushKeyTbl['D']) keyInput.x += 1.0f;

			// �΂ߓ��͂Ȃ�
			if (keyInput.length() > 1.0f) keyInput *= 0.7f;

			return keyInput;
		}

		/*!
		@brief L�X�e�B�b�N�̓��͗ʎ擾�֐�
		@return Vec2(pad.fThumbLX, pad.fThumbLY)
		*/
		inline Vec2 GetLStickValue()
		{
			// �R���g���[���[���ڑ�����Ă���Ȃ�
			if (GetPadConected())
			{
				// �R���g���[���[���擾
				const auto& pad = GetPad();

				// �R���g���[���[��L�X�e�B�b�N���͗ʂ�Vec2�ɂ��ĕԂ�
				return Vec2(pad.fThumbLX, pad.fThumbLY);
			}

			// WASD�ł̓��͗ʂ�Ԃ�
			return GetWASDValue();
		}

		/*!
		@brief L�X�e�B�b�N�̓��͂���Ă��邩�̐^�U�擾�֐�
		@return GetLStickValue().length() > 0.0f
		*/
		inline bool IsInputLStick()
		{
			// L�X�e�B�b�N�̓��͗ʂ̒�����0���傫�����̐^�U�Ԃ�
			return GetLStickValue().length() > 0.0f;
		}

		/*!
		@brief L�X�e�B�b�N�̓��͂���Ă��邩�̐^�U��X���݂̂Ŏ擾����֐�
		@return stick.length() > 0.0f
		*/
		inline bool IsInputLStickX()
		{
			// L�X�e�B�b�N�̓��͗ʂ��擾
			Vec2 stick = GetLStickValue();
			stick.y = 0.0f; // Y����0��
			return stick.length() > 0.0f; // X���݂̂Œ������r
		}

		/*!
		@brief L�X�e�B�b�N�̓��͂���Ă��邩�̐^�U��L���݂̂Ŏ擾����֐�
		@return stick.length() > 0.0f
		*/
		inline bool IsInputLStickY()
		{
			// L�X�e�B�b�N�̓��͗ʂ��擾
			Vec2 stick = GetLStickValue();
			stick.x = 0.0f; // X����0��
			return stick.length() > 0.0f; // Y���݂̂łŒ������r
		}

		/*!
		@brief ��ʏ�̃}�E�X��2D���W(Basecross�̍��W�n)���擾����֐�
		@return �}�E�X���W
		*/
		inline Vec2 GetMousePosition()
		{
			// �L�[�X�e�[�g����}�E�X���W���擾
			Vec2 mousePos = Vec2(0.0f);
			auto& keyState = GetKeyboard();

			// �}�E�X�����킶��Ȃ���ΏI��
			if (!keyState.m_MouseCheck) return mousePos;

			// ���W�n��0.0���S�ł��邽�߁A��ʃT�C�Y�̔���������
			mousePos.x = float(keyState.m_MouseClientPoint.x - (int(WINDOW_WIDTH) / 2));
			mousePos.y = float(keyState.m_MouseClientPoint.y - (int(WINDOW_HEIGHT) / 2));
			mousePos.y *= -1.0f;

			// �Ԃ�
			return mousePos;
		}
	}
}