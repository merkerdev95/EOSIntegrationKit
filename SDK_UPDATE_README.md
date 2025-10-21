# EOS SDK 1.17-1.18 Update - Quick Start Guide

## What's New

This update brings **EOS SDK 1.18.0.4** support to the EOSIntegrationKit plugin with three major new features:

### 🌍 1. Localized Presence
Display Rich Presence in each player's preferred language using templates configured in your EOS Developer Portal.

**New Functions:**
- `EIK_PresenceModification_SetTemplateId` - Set presence template
- `EIK_PresenceModification_SetTemplateData` - Fill template parameters

### 🎙️ 2. Manual Lobby Voice Chat Control
Control when players connect to voice chat independently from lobby membership.

**New Functions:**
- `EIK_Lobby_JoinRTCRoom` - Join voice chat manually
- `EIK_Lobby_LeaveRTCRoom` - Leave voice chat while staying in lobby

**Perfect for:** Mobile microphone permissions, delayed voice connection, privacy controls

### 💰 3. Mobile Ecom Support
Epic Games Store In-App Purchases now work on iOS and Android (Early Adopter program).

**Updated Functions:**
- `EIK_Ecom_Checkout` - Now supports mobile checkout UI
- `EIK_Ecom_QueryOwnership` - Required for durable items

---

## ⚠️ Breaking Changes

### Android (Required)
```gradle
// Update your build.gradle
compileOptions {
    sourceCompatibility JavaVersion.VERSION_11  // Was: VERSION_1_8
    targetCompatibility JavaVersion.VERSION_11  // Was: VERSION_1_8
}
```
- **Java 11** is now required (was Java 8)
- **NDK r27c** recommended
- Already updated in `LibEIK_APL.xml`

### iOS
- Minimum iOS version: **15.0** (was 11.0)
- Xcode 16+ required

### Ecom API Changes
```cpp
// OLD - No longer works for durable items
EIK_Ecom_QueryEntitlements(DurableItems);  // Returns EOS_InvalidRequest

// NEW - Use for durable items
EIK_Ecom_QueryOwnership(DurableItems);     // Correct

// QueryEntitlements still works for consumables
EIK_Ecom_QueryEntitlements(ConsumableItems);  // Still correct
```

---

## 🚀 Quick Start Examples

### Example 1: Localized Presence
```cpp
// Create modification handle
FEIK_HPresenceModification PresenceHandle;
EIK_Presence_CreatePresenceModification(LocalUserId, PresenceHandle);

// Set template (configured in Dev Portal)
EIK_PresenceModification_SetTemplateId(PresenceHandle, "InGameTemplate");

// Set parameters: "Playing {game_mode} with score {score}"
EIK_PresenceModification_SetTemplateData(
    PresenceHandle, "game_mode", 0, "deathmatch",
    EEIK_PresenceModificationTemplateType::PMT_STRING
);
EIK_PresenceModification_SetTemplateData(
    PresenceHandle, "score", 1500, "",
    EEIK_PresenceModificationTemplateType::PMT_INT
);

// Apply
EIK_Presence_SetPresence(LocalUserId, PresenceHandle);
EIK_PresenceModification_Release(PresenceHandle);
```

### Example 2: Manual Voice Chat Control
```cpp
// Join lobby first
EIK_Lobby_JoinLobby(JoinOptions);

// On mobile: Request microphone permission here
// ...

// Then join voice chat when ready
FEIK_Lobby_JoinRTCRoomOptions RTCOptions;
RTCOptions.LobbyId = CurrentLobbyId;
RTCOptions.LocalUserId = LocalProductUserId;
EIK_Lobby_JoinRTCRoom(RTCOptions);

// Later: Leave voice but stay in lobby
FEIK_Lobby_LeaveRTCRoomOptions LeaveOptions;
LeaveOptions.LobbyId = CurrentLobbyId;
LeaveOptions.LocalUserId = LocalProductUserId;
EIK_Lobby_LeaveRTCRoom(LeaveOptions);
```

### Example 3: Mobile Ecom Checkout
```cpp
// Query durable item ownership (NEW requirement)
FEIK_Ecom_QueryOwnershipOptions OwnershipOptions;
OwnershipOptions.LocalUserId = LocalEpicAccountId;
OwnershipOptions.CatalogItemIds = { ItemId };
auto* QueryAction = EIK_Ecom_QueryOwnership(OwnershipOptions);
QueryAction->OnCallback.AddDynamic(this, &UMyClass::OnOwnershipQueried);

// Checkout still works the same
FEIK_Ecom_CheckoutOptions CheckoutOptions;
CheckoutOptions.LocalUserId = LocalEpicAccountId;
CheckoutOptions.Entries = { CheckoutEntry };
auto* CheckoutAction = EIK_Ecom_Checkout(CheckoutOptions);
CheckoutAction->OnCallback.AddDynamic(this, &UMyClass::OnCheckoutComplete);
```

---

## 📋 Implementation Checklist

### All Projects
- [ ] Update plugin to latest version
- [ ] Review [CHANGELOG_SDK_1.17-1.18.md](./CHANGELOG_SDK_1.17-1.18.md) for full details
- [ ] Test existing functionality

### Android Projects
- [ ] Verify Java 11 in build.gradle (auto-updated in APL)
- [ ] Test on devices
- [ ] Update NDK to r27c (recommended)

### iOS Projects
- [ ] Update Xcode to 16+
- [ ] Update minimum iOS deployment target to 15.0
- [ ] Test on iOS 16+ for orientation handling

### Using Ecom
- [ ] Update durable item queries to `QueryOwnership`
- [ ] Keep consumable queries using `QueryEntitlements`
- [ ] Test checkout flow

### Using Lobbies + Voice
- [ ] Consider implementing manual RTC control
- [ ] Add microphone permission handling (mobile)
- [ ] Test delayed voice connection

### Using Presence
- [ ] Configure templates in EOS Developer Portal
- [ ] Implement localized presence
- [ ] Test in multiple languages

---

## 📁 New Files

### Presence Interface
- `EIK_PresenceModification_SetTemplateId.h/.cpp`
- `EIK_PresenceModification_SetTemplateData.h/.cpp`

### Lobby Interface
- `EIK_Lobby_JoinRTCRoom.h/.cpp`
- `EIK_Lobby_LeaveRTCRoom.h/.cpp`

### Documentation
- `CHANGELOG_SDK_1.17-1.18.md` - Full changelog
- `SDK_UPDATE_README.md` - This file

### Modified Files
- `LibEIK_APL.xml` - Java 11 requirement
- `EIK_Ecom_Checkout.h` - Mobile documentation
- `EIK_Ecom_QueryOwnership.h` - Durable items documentation

---

## 🆘 Need Help?

**Full Documentation:** See [CHANGELOG_SDK_1.17-1.18.md](./CHANGELOG_SDK_1.17-1.18.md)

**Resources:**
- EOS SDK Docs: https://dev.epicgames.com/docs/
- Plugin Docs: https://eik.betide.studio/
- Support Discord: https://discord.gg/betidestudio

**Common Issues:**
- **Android build fails:** Check Java 11 in build.gradle
- **iOS build fails:** Update Xcode to 16+, set iOS 15.0 min version
- **Ecom returns EOS_InvalidRequest:** Use QueryOwnership for durables
- **Voice chat not working on mobile:** Request microphone permissions first

---

## 🎯 Platform Support

| Platform | Status | Notes |
|----------|--------|-------|
| Windows | ✅ Fully Supported | No changes required |
| Android | ✅ Fully Supported | **Java 11 required** |
| iOS | ✅ Fully Supported | **iOS 15.0+ required** |
| macOS | ✅ Fully Supported | No changes required |
| Linux | ✅ Fully Supported | No changes required |
| PlayStation 4 | ✅ Fully Supported | SDK 12.508.001 |
| PlayStation 5 | ✅ Fully Supported | SDK 11.00.00.40 |
| Xbox (GDK) | ✅ Fully Supported | April 2025 QFE0 |
| Nintendo Switch | ✅ Fully Supported | SDK 19.3.5 |
| Nintendo Switch 2 | ✅ **NEW** | SDK 20.5.6 |

---

**Version:** EOS SDK 1.18.0.4
**Plugin:** EOSIntegrationKit 4.85+
**Updated:** October 2025
**License:** MIT
