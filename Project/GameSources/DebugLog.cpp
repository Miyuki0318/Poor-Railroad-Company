/*!
@file DebugLog.cpp
@brief �f�o�b�O�p������\��
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

/*!
@brief �l��wstring�^�ɕϊ����Ԃ��֐�(���������͉��s)
@return wstring�^�ɕϊ����ꂽ�l
*/
wstring Debug::GetWss()
{
	wstringstream wstr;
	wstr << std::endl;
	return wstr.str();
}

/*!
@brief �l��wstring�^�ɕϊ����Ԃ��֐�
@param int�^�̕ϊ�����l
@return wstring�^�ɕϊ����ꂽ�l
*/
wstring Debug::GetWss(const int str)
{
	wstringstream wstr;
	wstr << str;
	return wstr.str();
}

/*!
@brief �l��wstring�^�ɕϊ����Ԃ��֐�
@param int�^�z��̕ϊ�����l
@return wstring�^�ɕϊ����ꂽ�l
*/
wstring Debug::GetWss(const int str[])
{
	wstringstream wstr;
	wstr << str;
	return wstr.str();
}

/*!
@brief �l��wstring�^�ɕϊ����Ԃ��֐�
@param float�^�̕ϊ�����l
@return wstring�^�ɕϊ����ꂽ�l
*/
wstring Debug::GetWss(const float str)
{
	wstringstream wstr;
	wstr << str;
	return wstr.str();
}

/*!
@brief �l��wstring�^�ɕϊ����Ԃ��֐�
@param float�^�z��̕ϊ�����l
@return wstring�^�ɕϊ����ꂽ�l
*/
wstring Debug::GetWss(const float str[])
{
	wstringstream wstr;
	wstr << str;
	return wstr.str();
}

/*!
@brief �l��wstring�^�ɕϊ����Ԃ��֐�
@param double�^�̕ϊ�����l
@return wstring�^�ɕϊ����ꂽ�l
*/
wstring Debug::GetWss(const double str)
{
	wstringstream wstr;
	wstr << str;
	return wstr.str();
}

/*!
@brief �l��wstring�^�ɕϊ����Ԃ��֐�
@param double�^�z��̕ϊ�����l
@return wstring�^�ɕϊ����ꂽ�l
*/
wstring Debug::GetWss(const double str[])
{
	wstringstream wstr;
	wstr << str;
	return wstr.str();
}

/*!
@brief �l��wstring�^�ɕϊ����Ԃ��֐�
@param char�^�̕ϊ�����l
@return wstring�^�ɕϊ����ꂽ�l
*/
wstring Debug::GetWss(const char str)
{
	wstringstream wstr;
	wstr << str;
	return wstr.str();
}

/*!
@brief �l��wstring�^�ɕϊ����Ԃ��֐�
@param char�^�z��̕ϊ�����l
@return wstring�^�ɕϊ����ꂽ�l
*/
wstring Debug::GetWss(const char str[])
{
	wstringstream wstr;
	wstr << str;
	return wstr.str();
}

/*!
@brief �l��wstring�^�ɕϊ����Ԃ��֐�
@param wchar_t�^�̕ϊ�����l
@return wstring�^�ɕϊ����ꂽ�l
*/
wstring Debug::GetWss(const wchar_t str)
{
	wstringstream wstr;
	wstr << str;
	return wstr.str();
}

/*!
@brief �l��wstring�^�ɕϊ����Ԃ��֐�
@param wchar_t�^�z��̕ϊ�����l
@return wstring�^�ɕϊ����ꂽ�l
*/
wstring Debug::GetWss(const wchar_t str[])
{
	wstringstream wstr;
	wstr << str;
	return wstr.str();
}

/*!
@brief �l��wstring�^�ɕϊ����Ԃ��֐�
@param bool�^��ϊ�����l
@return wstring�^�ɕϊ����ꂽ�l
*/
wstring Debug::GetWss(const bool str)
{
	wstringstream wstr;
	wstr << (str ? L"true" : L"false");
	return wstr.str();
}

/*!
@brief �l��wstring�^�ɕϊ����Ԃ��֐�
@param string�^�̕ϊ�����l
@return wstring�^�ɕϊ����ꂽ�l
*/
wstring Debug::GetWss(const string& str)
{
	wstring wstr;
	basecross::Util::ConvertUtf8toWstring(str, wstr);
	return wstr;
}

/*!
@brief �l��wstring�^�ɕϊ����Ԃ��֐�
@param wstring�^�̕ϊ�����l
@return wstring�^�ɕϊ����ꂽ�l
*/
wstring Debug::GetWss(const wstring& str)
{
	return str;
}

/*!
@brief �l��wstring�^�ɕϊ����Ԃ��֐�
@param size_t�^�̕ϊ�����l
@return wstring�^�ɕϊ����ꂽ�l
*/
wstring Debug::GetWss(const size_t& str)
{
	wostringstream ostr;
	ostr << std::dec << str;
	return ostr.str();
}

/*!
@brief �l��wstring�^�ɕϊ����Ԃ��֐�
@param uint16_t�^�̕ϊ�����l
@return wstring�^�ɕϊ����ꂽ�l
*/
wstring Debug::GetWss(const uint16_t& str)
{
	wostringstream ostr;
	ostr << std::dec << str;
	return ostr.str();
}

/*!
@brief �l��wstring�^�ɕϊ����Ԃ��֐�
@param UINT�^�̕ϊ�����l
@return wstring�^�ɕϊ����ꂽ�l
*/
wstring Debug::GetWss(const UINT& str)
{
	wostringstream ostr;
	ostr << std::dec << str;
	return ostr.str();
}

/*!
@brief �l��wstring�^�ɕϊ����Ԃ��֐�
@param Flt2�^�̕ϊ�����l
@return wstring�^�ɕϊ����ꂽ�l
*/
wstring Debug::GetWss(const Flt2& str)
{
	wstringstream wstr;
	wstr << str.x << L", " << str.y;
	return wstr.str();
}

/*!
@brief �l��wstring�^�ɕϊ����Ԃ��֐�
@param Flt3�^�̕ϊ�����l
@return wstring�^�ɕϊ����ꂽ�l
*/
wstring Debug::GetWss(const Flt3& str)
{
	wstringstream wstr;
	wstr << str.x << L", " << str.y << L", " << str.z;
	return wstr.str();
}

/*!
@brief �l��wstring�^�ɕϊ����Ԃ��֐�
@param Flt4�^�̕ϊ�����l
@return wstring�^�ɕϊ����ꂽ�l
*/
wstring Debug::GetWss(const Flt4& str)
{
	wstringstream wstr;
	wstr << str.x << L", " << str.y << L", " << str.z << L", " << str.w;
	return wstr.str();
}