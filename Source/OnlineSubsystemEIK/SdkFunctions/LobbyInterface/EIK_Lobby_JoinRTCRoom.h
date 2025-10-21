// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "eos_lobby_types.h"
#include "EIK_Lobby_JoinRTCRoom.generated.h"

/**
 * Options for joining an RTC room associated with a lobby
 * New in EOS SDK 1.17 - Manual RTC Room Management
 */
USTRUCT(BlueprintType)
struct FEIK_Lobby_JoinRTCRoomOptions
{
	GENERATED_BODY()

	/** The ID of the lobby to join the RTC Room of */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	FEIK_LobbyId LobbyId;

	/** The Product User ID of the local user joining the RTC room */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	FEIK_ProductUserId LocalUserId;

	/**
	 * (Optional) Set this value to override the default local options for the RTC Room.
	 * Set this to NULL if you would like to use the default settings.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	FEIK_Lobby_LocalRTCOptions LocalRTCOptions;

	FEIK_Lobby_JoinRTCRoomOptions()
	{
	}

	EOS_Lobby_JoinRTCRoomOptions ToEOSOptions()
	{
		EOS_Lobby_JoinRTCRoomOptions Options;
		Options.ApiVersion = EOS_LOBBY_JOINRTCROOM_API_LATEST;
		Options.LobbyId = LobbyId.Ref;
		Options.LocalUserId = LocalUserId.GetValueAsEosType();
		EOS_Lobby_LocalRTCOptions VarTemp = LocalRTCOptions.GetValueAsEosType();
		Options.LocalRTCOptions = &VarTemp;
		return Options;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIK_Lobby_JoinRTCRoomDelegate, const TEnumAsByte<EEIK_Result>&, Result, const FEIK_LobbyId&, LobbyId);

/**
 * Blueprint async action for joining the RTC room associated with a lobby
 * New in EOS SDK 1.17 - Manual RTC Room Management
 *
 * This allows manual control over RTC room joining, separate from lobby membership.
 * Useful for scenarios where you want to join a lobby but delay voice chat connection,
 * or leave voice chat while remaining in the lobby.
 */
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Lobby_JoinRTCRoom : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	/**
	 * Manually joins the RTC room associated with a specific lobby the local user already belongs to.
	 *
	 * This function gives you explicit control over when to connect to the lobby's voice chat.
	 * The lobby must have an RTC room enabled and the user must already be a member of the lobby.
	 *
	 * Use cases:
	 * - Delayed voice chat connection (join lobby first, voice chat later)
	 * - Selective voice chat participation
	 * - Managing microphone permissions on mobile before connecting
	 *
	 * @param Options Structure containing the lobby ID and local user information
	 * @return Async action object with OnCallback delegate
	 *
	 * @see https://dev.epicgames.com/docs/game-services/lobbies#manual-rtc-room-control
	 * @see EIK_Lobby_LeaveRTCRoom
	 */
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_JoinRTCRoom")
	static UEIK_Lobby_JoinRTCRoom* EIK_Lobby_JoinRTCRoom(FEIK_Lobby_JoinRTCRoomOptions Options);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	FEIK_Lobby_JoinRTCRoomDelegate OnCallback;

private:
	static void EOS_CALL OnJoinRTCRoomComplete(const EOS_Lobby_JoinRTCRoomCallbackInfo* Data);
	virtual void Activate() override;
	FEIK_Lobby_JoinRTCRoomOptions Var_Options;
};
