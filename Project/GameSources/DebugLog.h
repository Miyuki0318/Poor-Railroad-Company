/*!
@file DebugLog.h
@brief �f�o�b�O�p������\��
@author ���V���M
*/

#pragma once
#include "stdafx.h"

// Debug::Log�p�N���X(Unity�̗l�ɋL�q���A��ʍ���ɕ������\������N���X)
class Debug
{
private:

	/*!
	@brief �R���X�g���N�^
	*/
	Debug();

	/*!
	@brief �f�X�g���N�^
	*/
	~Debug() {}

	/*!
	@brief �l��wstring�^�ɕϊ����Ԃ��֐�
	@param template�^�̕ϊ�����l
	@return wstring�^�ɕϊ����ꂽ�l
	*/
	static wstring GetWss();
	static wstring GetWss(const bool str);
	static wstring GetWss(const int str);
	static wstring GetWss(const int str[]);
	static wstring GetWss(const float str);
	static wstring GetWss(const float str[]);
	static wstring GetWss(const double str);
	static wstring GetWss(const double str[]);
	static wstring GetWss(const char str);
	static wstring GetWss(const char str[]);
	static wstring GetWss(const wchar_t str);
	static wstring GetWss(const wchar_t str[]);
	static wstring GetWss(const string& str);
	static wstring GetWss(const wstring& str);
	static wstring GetWss(const size_t& str);
	static wstring GetWss(const uint16_t& str);
	static wstring GetWss(const UINT& str);
	static wstring GetWss(const Flt2& str);
	static wstring GetWss(const Flt3& str);
	static wstring GetWss(const Flt4& str);

	/*!
	@brief �l��wstringstream�^�ɕϊ����Ԃ��֐�
	@param template�^�̕ϊ�����l
	@return wstringstream�^�ɕϊ����ꂽ�l
	*/
	template <class T>
	static wstringstream SetWss(const T& str)
	{
		wstringstream wstr;
		wstr << Debug::GetWss(str);
		return wstr;
	}

	/*!
	@brief �l��wstringstream�^�ɕϊ����Ԃ��֐�
	@param template�^�̕ϊ�����l���
	@return wstringstream�^�ɕϊ����ꂽ�l
	*/
	template <class T, class U>
	static wstringstream SetWss(const T& str1, const U& str2)
	{
		wstringstream wstr;
		wstr << Debug::GetWss(str1) << Debug::GetWss(str2);
		return wstr;
	}

	/*!
	@brief �l��wstringstream�^�ɕϊ����Ԃ��֐�
	@param template�^�̕ϊ�����l�O��
	@return wstringstream�^�ɕϊ����ꂽ�l
	*/
	template <class T, class U, class V>
	static wstringstream SetWss(const T& str1, const U& str2, const V& str3)
	{
		wstringstream wstr;
		wstr << Debug::GetWss(str1) << Debug::GetWss(str2) << Debug::GetWss(str3);
		return wstr;
	}


	/*!
	@brief �l��wstringstream�^�ɕϊ����Ԃ��֐�
	@param template�^�̕ϊ�����l�l��
	@return wstringstream�^�ɕϊ����ꂽ�l
	*/
	template <class T, class U, class V, class W>
	static wstringstream SetWss(const T& str1, const U& str2, const V& str3, const W& str4)
	{
		wstringstream wstr;
		wstr << Debug::GetWss(str1) << Debug::GetWss(str2) << Debug::GetWss(str3) << Debug::GetWss(str4);
		return wstr;
	}

public:

	/*!
	@brief �l��wstringstream�^�ɕϊ����A����ɕ������\������
	@param template�^�̕ϊ�����l
	*/
	template <class T>
	static void Log(const T& param)
	{
		wstringstream wss = Debug::SetWss(param);
		wss << std::endl;
		auto scene = basecross::App::GetApp()->GetScene<basecross::Scene>();
		auto dstr = scene->GetDebugString();
		scene->SetDebugString(dstr + wss.str());
	}

	/*!
	@brief �l��wstringstream�^�ɕϊ����A����ɕ������\������
	@param template�^�̕ϊ�����l���
	*/
	template <class T, class U>
	static void Log(const T& param1, const U& param2)
	{
		wstringstream wss = Debug::SetWss(param1, param2);
		wss << std::endl;
		auto scene = basecross::App::GetApp()->GetScene<basecross::Scene>();
		auto dstr = scene->GetDebugString();
		scene->SetDebugString(dstr + wss.str());
	}

	/*!
	@brief �l��wstringstream�^�ɕϊ����A����ɕ������\������
	@param template�^�̕ϊ�����l�O��
	*/
	template <class T, class U, class V>
	static void Log(const T& param1, const U& param2, const V& param3)
	{
		wstringstream wss = Debug::SetWss(param1, param2, param3);
		wss << std::endl;
		auto scene = basecross::App::GetApp()->GetScene<basecross::Scene>();
		auto dstr = scene->GetDebugString();
		scene->SetDebugString(dstr + wss.str());
	}

	/*!
	@brief �l��wstringstream�^�ɕϊ����A����ɕ������\������
	@param template�^�̕ϊ�����l�l��
	*/
	template <class T, class U, class V, class W>
	static void Log(const T& param1, const U& param2, const V& param3, const W& param4)
	{
		wstringstream wss = Debug::SetWss(param1, param2, param3, param4);
		wss << std::endl;
		auto scene = basecross::App::GetApp()->GetScene<basecross::Scene>();
		auto dstr = scene->GetDebugString();
		scene->SetDebugString(dstr + wss.str());
	}
};