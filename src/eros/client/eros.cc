//////////////////////////////////////////////////////////////////////////
//
// Copyright(C)	2016, gjm_kevin.  All rights reserved.
// Author	:	gjm_kevin@163.com
//
//////////////////////////////////////////////////////////////////////////
#pragma comment(lib,"libprotobuf.lib") 
#include "IM.Message.pb.h"
#include "eros.h"
#include "../eros/base/PBHeader.h"
#include "../eros/base/PduPacketParse.h"

Eros::Eros(EventLoop* loop, const InetAddress& serverAddr)
	:loop_(loop),
	client_(loop, serverAddr, "Eros")
{
	client_.setConnectionCallback(std::bind(&Eros::onConnection, this, std::placeholders::_1));
	client_.setMessageCallback(std::bind(&Eros::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}
void Eros::connect() { client_.connect(); }
void Eros::onConnection(const TcpConnectionPtr& conn)
{
	//calm::eros::PBHeader header;
	//header.setFlag(1);
	//header.setModuleId(IM::BaseDefine::ServiceID::SID_OTHER);
	//header.setCommandId(IM::BaseDefine::OtherCmdID::CID_OTHER_HEARTBEAT);
	//header.setSeqNumber(888);
	//header.setReserved(999);
	//header.setLength(1000);
	//LOG_INFO << header.getLength();
	//LOG_INFO << header.getFlag();
	//LOG_INFO << header.getFlag();
	//LOG_INFO << header.getModuleId();
	//LOG_INFO << header.getCommandId();
	//LOG_INFO << header.getSeqNumber();
	//LOG_INFO << header.getReserved();

	//conn->send(header.getSerializeBuffer(),calm::eros::HEADER_LENGTH);
	IM::Message::IMMsgData msgData;
	msgData.set_from_user_id(10);
	msgData.set_to_session_id(11);
	msgData.set_msg_id(12);
	msgData.set_create_time(13);
	msgData.set_msg_type(IM::BaseDefine::MSG_TYPE_GROUP_TEXT);
	msgData.set_msg_data("hello eros");
	msgData.set_attach_data("kevin-gjm");
	calm::eros::PduPacketParse::sendPacket(conn, IM::BaseDefine::ServiceID::SID_MSG, IM::BaseDefine::MessageCmdID::CID_MSG_DATA, &msgData);
}
void Eros::onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp receiveTime)
{
	LOG_INFO << buf->retrieveAllAsString() << " time:" << receiveTime.toFormattedString();
	//conn->send("world");
	std::this_thread::sleep_for(std::chrono::seconds(3));

	if (buf->readableBytes() < 4)
		return;
	uint32_t length = buf->peekInt32();
	if (buf->readableBytes() < length + calm::eros::HEADER_LENGTH)
		return;
	calm::eros::PBHeader header;
	header.unSerialize(buf); // get and retrieve the header size
}