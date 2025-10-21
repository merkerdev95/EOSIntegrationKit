// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#include "EIK_PresenceModification_SetTemplateData.h"
#include "OnlineSubsystemEOS.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"
#include "eos_presence.h"

TEnumAsByte<EEIK_Result> UEIK_PresenceModification_SetTemplateData::EIK_PresenceModification_SetTemplateData(
	FEIK_HPresenceModification PresenceModificationHandle,
	const FString& Key,
	int32 IntValue,
	const FString& StringIdValue,
	EEIK_PresenceModificationTemplateType ValueType)
{
	if (PresenceModificationHandle.Ref == nullptr)
	{
		UE_LOG(LogEIK, Error, TEXT("EIK_PresenceModification_SetTemplateData: PresenceModificationHandle is invalid"));
		return EEIK_Result::EOS_InvalidParameters;
	}

	if (Key.IsEmpty())
	{
		UE_LOG(LogEIK, Error, TEXT("EIK_PresenceModification_SetTemplateData: Key cannot be empty"));
		return EEIK_Result::EOS_InvalidParameters;
	}

	EOS_PresenceModification_SetTemplateDataOptions Options = {};
	Options.ApiVersion = EOS_PRESENCEMODIFICATION_SETTEMPLATEDATA_API_LATEST;
	Options.Key = TCHAR_TO_UTF8(*Key);
	Options.ValueType = static_cast<EOS_EPresenceModificationTemplateType>(ValueType);

	// Set the appropriate value based on type
	if (ValueType == EEIK_PresenceModificationTemplateType::PMT_INT)
	{
		Options.Value.AsInt32 = IntValue;
	}
	else if (ValueType == EEIK_PresenceModificationTemplateType::PMT_STRING)
	{
		Options.Value.AsStringId = TCHAR_TO_UTF8(*StringIdValue);
	}

	EOS_EResult Result = EOS_PresenceModification_SetTemplateData(PresenceModificationHandle.Ref, &Options);

	if (Result != EOS_EResult::EOS_Success)
	{
		UE_LOG(LogEIK, Warning, TEXT("EIK_PresenceModification_SetTemplateData: Failed with result: %s"), *FString(EOS_EResult_ToString(Result)));
	}

	return static_cast<EEIK_Result>(Result);
}

TEnumAsByte<EEIK_Result> UEIK_PresenceModification_SetTemplateData::EIK_PresenceModification_SetTemplateDataStruct(
	FEIK_HPresenceModification PresenceModificationHandle,
	const FEIK_PresenceModification_TemplateData& TemplateData)
{
	return EIK_PresenceModification_SetTemplateData(
		PresenceModificationHandle,
		TemplateData.Key,
		TemplateData.IntValue,
		TemplateData.StringIdValue,
		TemplateData.ValueType
	);
}
