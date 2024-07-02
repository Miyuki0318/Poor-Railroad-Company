/*!
@file Input.h
@brief �R���g���[���[���͎擾
*/

#pragma once
#include "stdafx.h"

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

			// �L�[�{�[�h���擾���A
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
		inline bool GetPushB()
		{
			// �R���g���[���[���ڑ�����Ă�Ȃ�
			if (GetPadConected())
			{
				// A�{�^�������͂��ꂽ�u�Ԃ���Ԃ�
				return GetPad().wPressedButtons & XINPUT_GAMEPAD_B;
			}
			return GetKeyboard().m_bPressedKeyTbl[VK_LBUTTON];
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
			return GetKeyboard().m_bPushKeyTbl[VK_LBUTTON];
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
		@return GetPad().wPressedButtons & XINPUT_GAMEPAD_A
		*/
		inline bool GetPushX()
		{
			// �R���g���[���[���ڑ�����Ă�Ȃ�
			if (GetPadConected())
			{
				// A�{�^�������͂��ꂽ�u�Ԃ���Ԃ�
				return GetPad().wPressedButtons & XINPUT_GAMEPAD_X;
			}
			return GetKeyboard().m_bPressedKeyTbl[VK_TAB];
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
	}
}