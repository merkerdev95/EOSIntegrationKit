// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#include "EIK_Lobby_LeaveRTCRoom.h"
#include "Async/Async.h"
#include "OnlineSessionEOS.h" 
#include "OnlineSubsystemEOS.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_Lobby_LeaveRTCRoom* UEIK_Lobby_LeaveRTCRoom::EIK_Lobby_LeaveRTCRoom(FEIK_Lobby_LeaveRTCRoomOptions Options)
{
	UEIK_Lobby_LeaveRTCRoom* Node = NewObject<UEIK_Lobby_LeaveRTCRoom>();
	Node->Var_Options = Options;
	return Node;
}

void UEIK_Lobby_LeaveRTCRoom::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_LeaveRTCRoomOptions Options = Var_Options.ToEOSOptions();
			EOS_Lobby_LeaveRTCRoom(EOSRef->SessionInterfacePtr->LobbyHandle, &Options, this, &UEIK_Lobby_LeaveRTCRoom::OnLeaveRTCRoomComplete);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_Lobby_LeaveRTCRoom::Activate: Unable to get EOS SDK"));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, FEIK_LobbyId());
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_Lobby_LeaveRTCRoom::OnLeaveRTCRoomComplete(const EOS_Lobby_LeaveRTCRoomCallbackInfo* Data)
{
	if (UEIK_Lobby_LeaveRTCRoom* Node = static_cast<UEIK_Lobby_LeaveRTCRoom*>(Data->ClientData))
	{
		AsyncTask(ENamedThreads::GameThread, [Node, Data]()
		{
			if (Data->ResultCode == EOS_EResult::EOS_Success)
			{
				UE_LOG(LogEIK, Log, TEXT("Successfully left RTC room for lobby"));
			}
			else if (Data->ResultCode == EOS_EResult::EOS_NoChange)
			{
				UE_LOG(LogEIK, Log, TEXT("RTC room leave had no effect (not in room or no RTC room)"));
			}
			else
			{
				UE_LOG(LogEIK, Warning, TEXT("Failed to leave RTC room: %s"), *FString(EOS_EResult_ToString(Data->ResultCode)));
			}

			Node->OnCallback.Broadcast(static_cast<EEIK_Result>(Data->ResultCode), Data->LobbyId);
			Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
			Node->MarkAsGarbage();
#else
			Node->MarkPendingKill();
#endif
		});
	}
}
