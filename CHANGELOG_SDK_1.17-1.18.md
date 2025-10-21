# EOSIntegrationKit - EOS SDK 1.17-1.18 Update Changelog

## Overview
This document details all updates made to the EOSIntegrationKit plugin to support new features and improvements introduced in Epic Online Services (EOS) SDK versions 1.17.x through 1.18.0.4.

**Current SDK Version:** 1.18.0.4
**Update Date:** October 2025
**Plugin Version:** 4.85+

---

## Major New Features

### 1. Localized Presence Support (EOS SDK 1.18)
Enables localized Rich Presence strings that display in each player's preferred language.

#### New Blueprint Functions:
- **EIK_PresenceModification_SetTemplateId**
  - Location: `Source/OnlineSubsystemEIK/SdkFunctions/PresenceInterface/EIK_PresenceModification_SetTemplateId.h`
  - Sets the Rich Presence template ID configured in your EOS Developer Portal
  - Replaces raw text presence with localized templates

- **EIK_PresenceModification_SetTemplateData**
  - Location: `Source/OnlineSubsystemEIK/SdkFunctions/PresenceInterface/EIK_PresenceModification_SetTemplateData.h`
  - Sets template parameter values (supports INT and STRING types)
  - Available in two variants: parameter-based and struct-based

#### Usage Example:
```cpp
// Create presence modification handle
FEIK_HPresenceModification PresenceHandle;
EIK_Presence_CreatePresenceModification(LocalUserId, PresenceHandle);

// Set template ID
EIK_PresenceModification_SetTemplateId(PresenceHandle, "MyPresenceTemplate");

// Set template data
EIK_PresenceModification_SetTemplateData(
    PresenceHandle,
    "score",           // Key
    1500,              // Integer value
    "",                // String ID (empty for INT type)
    EEIK_PresenceModificationTemplateType::PMT_INT
);

// Apply presence
EIK_Presence_SetPresence(LocalUserId, PresenceHandle);

// Release handle
EIK_PresenceModification_Release(PresenceHandle);
```

**Documentation:** https://dev.epicgames.com/docs/epic-account-services/eos-presence-interface#localized-presence

---

### 2. Manual Lobby RTC Room Management (EOS SDK 1.17)
Provides explicit control over voice chat connections independent of lobby membership.

#### New Blueprint Functions:
- **EIK_Lobby_JoinRTCRoom**
  - Location: `Source/OnlineSubsystemEIK/SdkFunctions/LobbyInterface/EIK_Lobby_JoinRTCRoom.h`
  - Manually joins the RTC room for an existing lobby membership
  - Enables delayed voice chat connection (e.g., after microphone permissions granted)

- **EIK_Lobby_LeaveRTCRoom**
  - Location: `Source/OnlineSubsystemEIK/SdkFunctions/LobbyInterface/EIK_Lobby_LeaveRTCRoom.h`
  - Leaves voice chat while remaining in the lobby
  - Useful for temporary muting or privacy controls

#### Use Cases:
1. **Mobile Microphone Permissions:** Ask for permissions before connecting to voice
2. **Delayed Voice Connection:** Join lobby first, connect to voice later
3. **Privacy Controls:** Disconnect from voice while staying in lobby
4. **Selective Participation:** Choose when to participate in voice chat

#### Usage Example:
```cpp
// User joins a lobby
EIK_Lobby_JoinLobby(JoinOptions);

// Later, when ready for voice chat (e.g., after permissions granted)
FEIK_Lobby_JoinRTCRoomOptions RTCOptions;
RTCOptions.LobbyId = CurrentLobbyId;
RTCOptions.LocalUserId = LocalProductUserId;
EIK_Lobby_JoinRTCRoom(RTCOptions);

// To leave voice chat without leaving lobby
FEIK_Lobby_LeaveRTCRoomOptions LeaveRTCOptions;
LeaveRTCOptions.LobbyId = CurrentLobbyId;
LeaveRTCOptions.LocalUserId = LocalProductUserId;
EIK_Lobby_LeaveRTCRoom(LeaveRTCOptions);
```

**Known Issues (Mobile):**
- On iOS/Android, always ask for microphone permissions before joining RTC room
- Provide non-null `ClientData` when calling the RTC room functions
- See release notes for recommended initialization flows

**Documentation:** https://dev.epicgames.com/docs/game-services/lobbies#manual-rtc-room-control

---

### 3. Ecom Mobile Checkout Support (EOS SDK 1.17)
Epic Games Store In-App Purchase (IAP) checkout now available on mobile platforms.

#### Updated Functions:
- **EIK_Ecom_Checkout**
  - Location: `Source/OnlineSubsystemEIK/SdkFunctions/EcomInterface/EIK_Ecom_Checkout.h`
  - Enhanced documentation for mobile platform support
  - Opens mobile-optimized checkout UI in embedded browser

- **EIK_Ecom_QueryOwnership**
  - Location: `Source/OnlineSubsystemEIK/SdkFunctions/EcomInterface/EIK_Ecom_QueryOwnership.h`
  - Enhanced documentation for durable item ownership queries
  - Required for durable items (consumables use QueryEntitlements)

#### Important Breaking Change:
- **QueryEntitlements** now returns `EOS_InvalidRequest` for durable items
- Use **QueryOwnership** for durable item ownership validation
- Use **QueryEntitlements** only for consumable purchases
- This change ensures proper server-side validation with Ecom Web API v4

#### Platform-Specific Notes:
- **iOS:** Requires iOS 15+ (Xcode 16 requirement)
- **iOS 16+:** Screen orientation handling may need engine-specific configuration
- **Android:** Includes floating action button (movable with long-press)
- **Android:** 16KB page size support added (Google requirement effective Nov 2025)

**Availability:** Epic Games Store Mobile Early Adopter Program partners only

**Documentation:** https://dev.epicgames.com/docs/epic-games-store/tech-features-config/ecom/ecom-overview

---

## Platform Updates

### Android Platform
**Updated Files:**
- `Source/ThirdParty/EIKSDK/LibEIK_APL.xml`

**Changes:**
1. **Java 11 Requirement (Breaking Change)**
   - EOS SDK 1.18+ requires Java 11
   - Added `sourceCompatibility` and `targetCompatibility` set to `JavaVersion.VERSION_11`
   - Projects using lower JDK versions will fail to build

   ```gradle
   compileOptions {
       sourceCompatibility JavaVersion.VERSION_11
       targetCompatibility JavaVersion.VERSION_11
   }
   ```

2. **NDK r27c Support**
   - EOS SDK 1.18 built with Android NDK r27c
   - Use **Static CRT variant** of binaries for projects using EOS SDK 1.18+
   - Dynamic CRT variant deprecated, will be removed in future release

3. **16KB Page Size Support**
   - Added support for Google's 16KB page size requirement (effective Nov 2025)
   - Ensures compatibility with upcoming Android requirements

### iOS Platform
**SDK Updates:**
- EOS SDK now available as **XCFramework** (`EOSSDK.xcframework`)
- Previous `EOSSDK.framework` retained for compatibility (may be deprecated)
- Minimum iOS version: **iOS 15.0** (due to Xcode 16 requirement)

**Build System:**
- The plugin's `EIKSDK.Build.cs` already supports XCFramework
- Both `.framework` and `.xcframework` binaries included

**Migration Note:**
- If using custom includes, update from `#import <EOSSDK/eos_*.h>` format
- XCFramework uses angled brackets: `#import <EOSSDK/eos_*.h>`

---

## Console Platform Updates

### PlayStation (PS4/PS5)
**SDK Support:**
- PlayStation 4 SDK: 12.508.001
- PlayStation 5 SDK: 11.00.00.40

**Bug Fixes:**
- Fixed issue where Connect user auth token was incorrectly marked expired after resume
- Fixed main thread blocking during lobby/session creation when checking online play permission (PS5)
- Fixed PlayStation friend name truncation in Social Overlay

### Xbox (GDK)
**SDK Support:**
- Microsoft Xbox GDK: April 2025 QFE0 (250400)

**Changes:**
- P2P relay requirement changed to **recommended** (was required)
- Aligns with GDK TRC changes
- Fixed pending invite processing when NetworkStatus set to Online before game startup

### Nintendo Switch
**New Platform Support:**
- Added **Nintendo Switch 2** support
- SDK versions supported: 19.3.5 (Switch 1) and 20.5.6 (Switch 2)

**Bug Fixes:**
- Fixed launch invite handling (EOS_IPMF_DisableSDKManagedSessions flag)
- Provided proper message and handle types for SDK

**C# Wrapper Updates:**
- New types: `SwitchNotificationMessage` and `SwitchUserHandle`
- Replaces direct use of `nn.oe.Message` and `nn.account.UserHandle`

---

## General SDK Improvements

### Voice Interface
**Stability Improvements:**
- Fixed crash when RTC platform initialization fails (now returns error)
- Fixed crash on PlayStation when using web sockets (memory leak fixed)
- Fixed audio device pool change handling (PlayStation)
- Fixed device reconnection issues
- Fixed hissing sounds on iOS devices
- Fixed audio updates with NotListening status
- Improved Nintendo Switch audio handling (default device selection, flow handling)

**API Updates (PlayStation):**
- New APIs for uniform audio device pool change handling
- `AudioDevicesChanged` notifications support

### Anti-Cheat Interface
**Improvements:**
- Added log message to `EOS_AntiCheatClient_EndSession`
- Improved anti-cheat bootstrapper logging with persistent logs (7-day retention)
- Fixed `CompetitionType` serialization in `EOS_AntiCheatCommon_LogGameRoundStartOptions`
- Increased `EOS_ANTICHEATCOMMON_LOGPLAYERUSEWEAPON_WEAPONNAME_MAX_LENGTH` from 16 to 32
- Windows-on-Arm support for x64 emulated binaries

### P2P Interface
**Changes:**
- Xbox GDK: Relay changed from required to recommended
- Fixed `OnClosedConnection` handler not called during unexpected network shutdown
- Reduced threading overhead for multiple peer connections

### Lobbies Interface
**Bug Fixes:**
- Fixed Join Lobby callbacks not executing in certain scenarios
- Fixed sanctioned players being able to join sanction-enforced sessions

---

## Breaking Changes Summary

### EOS Core
1. **Version Format Change**
   - Old: `x.x.x` (e.g., 1.17.0)
   - New: `x.x.x.x` (e.g., 1.18.0.4)
   - **Action Required:** Update any tools/systems that parse SDK version numbers

### Android
2. **Java 11 Requirement**
   - **Action Required:** Update `build.gradle` to Java 11
   - Projects targeting JDK < 11 will not build

3. **NDK r27c**
   - **Action Required:** Use Static CRT variant binaries
   - Dynamic CRT variant deprecated

### Ecom
4. **Durable Item Queries**
   - **Action Required:** Use `QueryOwnership` for durables, not `QueryEntitlements`
   - `QueryEntitlements` now returns `EOS_InvalidRequest` for durable items

### C# SDK (If using C# wrapper)
5. **Constants Renamed**
   - All constants now uppercase
   - **Action Required:** Update constant references

6. **Directory Reorganization**
   - Removed `Overrides` directory → replaced with `Core/<Feature>`
   - Removed `Config` class → constants moved to `Common`
   - **Action Required:** Delete old SDK, download new version, update imports

---

## Implementation Checklist

### For All Projects:
- [ ] Review new wrapper functions and determine if your project can benefit
- [ ] Test existing functionality after plugin update
- [ ] Update any custom code that parses SDK version strings

### For Android Projects:
- [ ] Update `build.gradle` to Java 11 (`sourceCompatibility` and `targetCompatibility`)
- [ ] Verify NDK version (r27c recommended)
- [ ] Test on devices with 16KB page size

### For iOS Projects:
- [ ] Verify minimum iOS version is 15.0+
- [ ] Test on iOS 16+ devices for screen orientation handling
- [ ] Update Xcode to version 16+

### For Ecom Users:
- [ ] Update durable item queries to use `QueryOwnership`
- [ ] Update consumable item queries to use `QueryEntitlements`
- [ ] Test mobile checkout flow (if in Early Adopter program)

### For Lobby + Voice Chat Users:
- [ ] Review manual RTC room control for improved UX
- [ ] Implement microphone permission handling (especially mobile)
- [ ] Test delayed voice connection flow

### For Presence Users:
- [ ] Configure localized presence templates in EOS Developer Portal
- [ ] Implement template-based presence using new functions
- [ ] Test presence display in different languages

---

## Testing Recommendations

1. **Presence System:**
   - Test localized presence with multiple language settings
   - Verify template parameters display correctly
   - Test fallback behavior for missing translations

2. **Lobby RTC Control:**
   - Test manual join/leave RTC room flow
   - Verify voice chat works after delayed connection
   - Test microphone permission denial handling (mobile)

3. **Ecom Checkout:**
   - Test mobile checkout UI on target devices
   - Verify durable item ownership queries
   - Test `EOS_Ecom_PurchaseProcessing` handling

4. **Platform-Specific:**
   - Android: Test on Java 11, NDK r27c
   - iOS: Test on iOS 15-17 devices
   - Console: Verify platform-specific bug fixes

---

## Known Issues & Workarounds

### macOS
**Issue:** Samples may not build without CMakeLists.txt changes
**Workaround:** Install CMake v3 or update `cmake_minimum_required(VERSION 3.10.2)`

### Windows Samples
**Issue:** Debug_SDL/Release_SDL builds fail to load fonts with long file paths
**Workaround:** Move EOS SDK directory to shorter path

### iOS Info.plist
**Issue:** SDK returns "does not support minimum OS Version" error
**Workaround:** Replace Info.plist content (see EOS SDK release notes for template)

### Ecom Clawbacks
**Issue:** `EOS_Ecom_CatalogItemId` callback data may point to invalid memory
**Workaround:** Ignore callback triggers with invalid memory

---

## Additional Resources

- **EOS SDK Documentation:** https://dev.epicgames.com/docs/
- **Presence Interface:** https://dev.epicgames.com/docs/epic-account-services/eos-presence-interface
- **Lobbies Interface:** https://dev.epicgames.com/docs/game-services/lobbies
- **Ecom Interface:** https://dev.epicgames.com/docs/epic-games-store/tech-features-config/ecom/ecom-overview
- **EOS SDK Release Notes:** https://dev.epicgames.com/docs/epic-online-services/release-notes

---

## File Summary

### New Files Created:
1. `Source/OnlineSubsystemEIK/SdkFunctions/PresenceInterface/EIK_PresenceModification_SetTemplateId.h`
2. `Source/OnlineSubsystemEIK/SdkFunctions/PresenceInterface/EIK_PresenceModification_SetTemplateId.cpp`
3. `Source/OnlineSubsystemEIK/SdkFunctions/PresenceInterface/EIK_PresenceModification_SetTemplateData.h`
4. `Source/OnlineSubsystemEIK/SdkFunctions/PresenceInterface/EIK_PresenceModification_SetTemplateData.cpp`
5. `Source/OnlineSubsystemEIK/SdkFunctions/LobbyInterface/EIK_Lobby_JoinRTCRoom.h`
6. `Source/OnlineSubsystemEIK/SdkFunctions/LobbyInterface/EIK_Lobby_JoinRTCRoom.cpp`
7. `Source/OnlineSubsystemEIK/SdkFunctions/LobbyInterface/EIK_Lobby_LeaveRTCRoom.h`
8. `Source/OnlineSubsystemEIK/SdkFunctions/LobbyInterface/EIK_Lobby_LeaveRTCRoom.cpp`

### Modified Files:
1. `Source/OnlineSubsystemEIK/SdkFunctions/EcomInterface/EIK_Ecom_Checkout.h` (documentation)
2. `Source/OnlineSubsystemEIK/SdkFunctions/EcomInterface/EIK_Ecom_QueryOwnership.h` (documentation)
3. `Source/ThirdParty/EIKSDK/LibEIK_APL.xml` (Java 11 requirement)

---

## Support

For issues or questions regarding this update:
- **Plugin Issues:** https://github.com/betidestudio/EOSIntegrationKit (if public repo exists)
- **EOS SDK Issues:** https://dev.epicgames.com/community
- **Discord:** https://discord.gg/betidestudio

---

**Last Updated:** October 21, 2025
**Maintained by:** Betide Studio
**License:** MIT
