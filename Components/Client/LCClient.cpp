//
// Created by dany on 14.05.23.
//

#include "LCClient.h"

LCClient::LCClient(QObject* parent)
{
    m_client = new Client(this);
}

LCClient::~LCClient()
{

}