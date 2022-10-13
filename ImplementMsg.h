#pragma once
#ifndef IMPLEMENT_MSG_H_
#define IMPLEMENT_MSG_H_

#include "ImplementTypes.h"

#define MSG_HEADER 0xdeadbeef

#pragma pack(push,1)

enum class enumImpMsgId
{
	ACTIVATE_IMP_MSG = 0x11,
	DEACTIVATE_IMP_MSG = 0x12,
	IMP_FUEL_MSG = 0x13,
	IMP_STATUS_MSG = 0x14,
};

struct ImpMsg
{
	ImpMsg()
		: m_msgHeader(MSG_HEADER), m_msgId(), m_param()
	{}

	ImpMsg(enumImpMsgId msgId, int param = 0)
		: m_msgHeader(MSG_HEADER), m_msgId(msgId), m_param(param)
	{}

	int m_msgHeader;
	enumImpMsgId m_msgId;
	int m_param;
};



#pragma pack(pop)

#endif