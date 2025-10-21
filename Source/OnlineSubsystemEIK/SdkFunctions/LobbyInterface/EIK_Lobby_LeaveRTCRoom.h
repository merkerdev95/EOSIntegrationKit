// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "eos_lobby_types.h"
#include "EIK_Lobby_LeaveRTCRoom.generated.h"

/**
 * Options for leaving an RTC room associated with a lobby
 * New in EOS SDK 1.17 - Manual RTC Room Management
 */
USTRUCT(BlueprintType)
struct FEIK_Lobby_LeaveRTCRoomOptions
{
	GENERATED_BODY()

	/** The ID of the lobby owning the RTC Room to leave */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	FEIK_LobbyId LobbyId;

	/** The Product User ID of the local user leaving the RTC room */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	FEIK_ProductUserId LocalUserId;

	FEIK_Lobby_LeaveRTCRoomOptions()
	{
	}

	EOS_Lobby_LeaveRTCRoomOptions ToEOSOptions()
	{
		EOS_Lobby_LeaveRTCRoomOptions Options;
		Options.ApiVersion = EOS_LOBBY_LEAVERTCROOM_API_LATEST;
		Options.LobbyId = LobbyId.Ref;
		Options.LocalUserId = LocalUserId.GetValueAsEosType();
		return Options;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIK_Lobby_LeaveRTCRoomDelegate, const TEnumAsByte<EEIK_Result>&, Result, const FEIK_LobbyId&, LobbyId);

/**
 * Blueprint async action for leaving the RTC room associated with a lobby
 * New in EOS SDK 1.17 - Manual RTC Room Management
 *
 * This allows manual control over RTC room participation, separate from lobby membership.
 * You can leave the voice chat while remaining a member of the lobby.
 */
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Lobby_LeaveRTCRoom : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	/**
	 * Manually leaves the RTC room associated with a specific lobby without leaving the lobby itself.
	 *
	 * This function gives you explicit control over when to disconnect from the lobby's voice chat
	 * while maintaining lobby membership. The user will remain in the lobby and continue to receive
	 * lobby updates, but will no longer be connected to voice chat.
	 *
	 * Use cases:
	 * - Temporarily mute all voice communication while staying in lobby
	 * - Disconnect from voice chat due to privacy concerns
	 * - Manage voice chat connection lifecycle independently from lobby membership
	 * - Handle microphone permission denial on mobile platforms
	 *
	 * @param Options Structure containing the lobby ID and local user information
	 * @return Async action object with OnCallback delegate
	 *
	 * @see https://dev.epicgames.com/docs/game-services/lobbies#manual-rtc-room-control
	 * @see EIK_Lobby_JoinRTCRoom
	 */
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_LeaveRTCRoom")
	static UEIK_Lobby_LeaveRTCRoom* EIK_Lobby_LeaveRTCRoom(FEIK_Lobby_LeaveRTCRoomOptions Options);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	FEIK_Lobby_LeaveRTCRoomDelegate OnCallback;

private:
	static void EOS_CALL OnLeaveRTCRoomComplete(const EOS_Lobby_LeaveRTCRoomCallbackInfo* Data);
	virtual void Activate() override;
	FEIK_Lobby_LeaveRTCRoomOptions Var_Options;
};
