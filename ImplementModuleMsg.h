#pragma once

#include "ImplementTypes.h"

#define MSG_HEADER 0xdeadbeef

#pragma pack(push,1)

enum class enumImpModuleMsgId
{
	ACTIVATE_IMP = 0x11,
	DEACTIVATE_IMP = 0x12,
	QUERY_IMP_FUEL = 0x13,
	QUERY_IMP_STATUS = 0x14,
};

struct ImpModuleMsg
{
	ImpModuleMsg()
		: m_msgHeader(MSG_HEADER), m_msgId(), m_id(), m_param()
	{}

	ImpModuleMsg(enumImpModuleMsgId msgId, enumImplement id, int param = 0)
		: m_msgHeader(MSG_HEADER), m_msgId(msgId), m_id(id), m_param(param)
	{}

	int m_msgHeader;
	enumImpModuleMsgId m_msgId;
	enumImplement m_id;
	int m_param;
};



#pragma pack(pop)

