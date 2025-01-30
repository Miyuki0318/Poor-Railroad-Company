#include "stdafx.h"
#include "NetworkComponent.h"
#include "PPDataConnecter.h"
#include <stdexcept>

namespace basecross
{
    NetworkComponent::NetworkComponent(const std::shared_ptr<GameObject>& GameObjectPtr)
        : Component(GameObjectPtr)
    {
    }

    NetworkComponent::~NetworkComponent() {}

    // GetNetworkPtrを使用してP2P通信クラスにアクセス
    PPDataConnecter* NetworkComponent::GetNetwork() const
    {
        auto network = PPDataConnecter::GetNetworkPtr();
        if (!network)
        {
            throw std::runtime_error("Network instance is not available");
        }
        return network;
    }

    void NetworkComponent::AddDataToSendBuffer(const std::string& header, const std::string& data) const
    {
        try 
        {
            GetNetwork()->AddToSendBuffer(header, data);
        }
        catch (...) 
        {
            throw;
        }
    }

    bool NetworkComponent::GetDataFromReceiveBuffer(const string& header, string& outData) const
    {
        try 
        {
            BufferedData buffer;
            if (GetNetwork()->GetFromRecvBufferByHeader(header, buffer))
            {
                outData = buffer.data;
                return true;
            }

            return false;
        }
        catch (...) 
        {
            throw;
        }
    }
}