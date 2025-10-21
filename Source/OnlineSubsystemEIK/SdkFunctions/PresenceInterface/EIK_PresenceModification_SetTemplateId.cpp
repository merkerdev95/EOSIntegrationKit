// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#include "EIK_PresenceModification_SetTemplateId.h"
#include "OnlineSubsystemEOS.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"
#include "eos_presence.h"

TEnumAsByte<EEIK_Result> UEIK_PresenceModification_SetTemplateId::EIK_PresenceModification_SetTemplateId(
	FEIK_HPresenceModification PresenceModificationHandle, const FString& TemplateId)
{
	if (PresenceModificationHandle.Ref == nullptr)
	{
		UE_LOG(LogEIK, Error, TEXT("EIK_PresenceModification_SetTemplateId: PresenceModificationHandle is invalid"));
		return EEIK_Result::EOS_InvalidParameters;
	}

	EOS_PresenceModification_SetTemplateIdOptions Options = {};
	Options.ApiVersion = EOS_PRESENCEMODIFICATION_SETTEMPLATEID_API_LATEST;
	Options.TemplateId = TCHAR_TO_UTF8(*TemplateId);

	EOS_EResult Result = EOS_PresenceModification_SetTemplateId(PresenceModificationHandle.Ref, &Options);

	if (Result != EOS_EResult::EOS_Success)
	{
		UE_LOG(LogEIK, Warning, TEXT("EIK_PresenceModification_SetTemplateId: Failed with result: %s"), *FString(EOS_EResult_ToString(Result)));
	}

	return static_cast<EEIK_Result>(Result);
}
