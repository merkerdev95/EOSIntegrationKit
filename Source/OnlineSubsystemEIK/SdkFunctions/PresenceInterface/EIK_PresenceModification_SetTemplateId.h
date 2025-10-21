// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "eos_presence_localized_types.h"
#include "EIK_PresenceModification_SetTemplateId.generated.h"

/**
 * Blueprint wrapper for EOS_PresenceModification_SetTemplateId
 * Sets the Rich Presence template ID to use for localized presence.
 * New in EOS SDK 1.18 - Localized Presence Support
 */
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_PresenceModification_SetTemplateId : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Sets the Rich Presence template ID to use for localized presence.
	 * This allows you to configure presence strings that are localized per-player based on their language settings.
	 * Setting this value will prevent SetRawRichText from being used on this handle.
	 *
	 * @param PresenceModificationHandle The presence modification handle
	 * @param TemplateId The RichPresence Template ID configured in your EOS Developer Portal
	 * @return EOS_Success if the template ID was set successfully, otherwise an error code
	 *
	 * @see https://dev.epicgames.com/docs/epic-account-services/eos-presence-interface#localized-presence
	 */
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Presence Interface", DisplayName="EOS_PresenceModification_SetTemplateId")
	static TEnumAsByte<EEIK_Result> EIK_PresenceModification_SetTemplateId(FEIK_HPresenceModification PresenceModificationHandle, const FString& TemplateId);
};
