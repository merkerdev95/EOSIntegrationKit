// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "eos_presence_localized_types.h"
#include "EIK_PresenceModification_SetTemplateData.generated.h"

/**
 * Enum representing the types of template data that can be passed for localized presence
 */
UENUM(BlueprintType)
enum class EEIK_PresenceModificationTemplateType : uint8
{
	/** Unset/invalid type */
	PMT_UNSET = 0 UMETA(DisplayName = "Unset"),
	/** 32-bit integer type */
	PMT_INT = 1 UMETA(DisplayName = "Integer"),
	/** UTF8 String as an identifier */
	PMT_STRING = 2 UMETA(DisplayName = "String")
};

/**
 * Structure for template data used with localized presence
 */
USTRUCT(BlueprintType)
struct FEIK_PresenceModification_TemplateData
{
	GENERATED_BODY()

	/** Key for the named template parameter */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit")
	FString Key;

	/** Integer value (used when ValueType is PMT_INT) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit")
	int32 IntValue;

	/** String ID value (used when ValueType is PMT_STRING) - references a StringId in the backend */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit")
	FString StringIdValue;

	/** Type of value stored */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit")
	EEIK_PresenceModificationTemplateType ValueType;

	FEIK_PresenceModification_TemplateData()
		: IntValue(0)
		, ValueType(EEIK_PresenceModificationTemplateType::PMT_INT)
	{
	}
};

/**
 * Blueprint wrapper for EOS_PresenceModification_SetTemplateData
 * Sets template data for localized presence strings.
 * New in EOS SDK 1.18 - Localized Presence Support
 */
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_PresenceModification_SetTemplateData : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Sets template data parameters for localized presence.
	 * Use this to fill in template variables defined in your presence template.
	 * You must call SetTemplateId before calling this function.
	 *
	 * @param PresenceModificationHandle The presence modification handle
	 * @param Key The key for the named template parameter
	 * @param IntValue The integer value (if using PMT_INT type)
	 * @param StringIdValue The string ID value (if using PMT_STRING type) - references a StringId configured in your EOS Developer Portal
	 * @param ValueType The type of value being set
	 * @return EOS_Success if the template data was set successfully, otherwise an error code
	 *
	 * @see https://dev.epicgames.com/docs/epic-account-services/eos-presence-interface#localized-presence
	 */
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Presence Interface", DisplayName="EOS_PresenceModification_SetTemplateData")
	static TEnumAsByte<EEIK_Result> EIK_PresenceModification_SetTemplateData(
		FEIK_HPresenceModification PresenceModificationHandle,
		const FString& Key,
		int32 IntValue,
		const FString& StringIdValue,
		EEIK_PresenceModificationTemplateType ValueType);

	/**
	 * Sets template data parameters for localized presence using a struct.
	 * Use this to fill in template variables defined in your presence template.
	 * You must call SetTemplateId before calling this function.
	 *
	 * @param PresenceModificationHandle The presence modification handle
	 * @param TemplateData The template data containing key-value pair
	 * @return EOS_Success if the template data was set successfully, otherwise an error code
	 *
	 * @see https://dev.epicgames.com/docs/epic-account-services/eos-presence-interface#localized-presence
	 */
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Presence Interface", DisplayName="EOS_PresenceModification_SetTemplateData (Struct)")
	static TEnumAsByte<EEIK_Result> EIK_PresenceModification_SetTemplateDataStruct(
		FEIK_HPresenceModification PresenceModificationHandle,
		const FEIK_PresenceModification_TemplateData& TemplateData);
};
