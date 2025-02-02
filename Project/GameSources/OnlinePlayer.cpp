#include "stdafx.h"
#include "OnlinePlayer.h"

namespace basecross
{
	void OnlinePlayer::OnCreate()
	{
		Player::OnCreate();
	}

	void OnlinePlayer::OnUpdate()
	{
		const auto netPtr = m_ptrNet->GetNetworkPtr();
		Debug::Log(L"‘Ò‹@‚µ‚Ä‚¢‚é‚© : ", netPtr->isWaiting);
		Debug::Log(L"Ú‘±‚µ‚Ä‚¢‚é‚© : ", netPtr->isConnected);
		Debug::Log(L"ID : ", netPtr->EncodeAndReverseIPPort(netPtr->GetLocalIPAddress(), netPtr->GetPortNumber()));
	}
}