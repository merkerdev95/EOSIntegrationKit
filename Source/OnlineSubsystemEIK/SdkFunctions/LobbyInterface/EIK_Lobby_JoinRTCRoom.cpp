// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#include "EIK_Lobby_JoinRTCRoom.h"
#include "Async/Async.h"
#include "OnlineSubsystemEOS.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_Lobby_JoinRTCRoom* UEIK_Lobby_JoinRTCRoom::EIK_Lobby_JoinRTCRoom(FEIK_Lobby_JoinRTCRoomOptions Options)
{
	UEIK_Lobby_JoinRTCRoom* Node = NewObject<UEIK_Lobby_JoinRTCRoom>();
	Node->Var_Options = Options;
	return Node;
}

void UEIK_Lobby_JoinRTCRoom::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_JoinRTCRoomOptions Options = Var_Options.ToEOSOptions();
			EOS_Lobby_JoinRTCRoom(EOSRef->SessionInterfacePtr->LobbyHandle, &Options, this, &UEIK_Lobby_JoinRTCRoom::OnJoinRTCRoomComplete);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_Lobby_JoinRTCRoom::Activate: Unable to get EOS SDK"));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, FEIK_LobbyId());
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_Lobby_JoinRTCRoom::OnJoinRTCRoomComplete(const EOS_Lobby_JoinRTCRoomCallbackInfo* Data)
{
	if (UEIK_Lobby_JoinRTCRoom* Node = static_cast<UEIK_Lobby_JoinRTCRoom*>(Data->ClientData))
	{
		AsyncTask(ENamedThreads::GameThread, [Node, Data]()
		{
			if (Data->ResultCode == EOS_EResult::EOS_Success)
			{
				UE_LOG(LogEIK, Log, TEXT("Successfully joined RTC room for lobby"));
			}
			else if (Data->ResultCode == EOS_EResult::EOS_NoChange)
			{
				UE_LOG(LogEIK, Log, TEXT("RTC room join had no effect (already joined or no RTC room)"));
			}
			else
			{
				UE_LOG(LogEIK, Warning, TEXT("Failed to join RTC room: %s"), *FString(EOS_EResult_ToString(Data->ResultCode)));
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
