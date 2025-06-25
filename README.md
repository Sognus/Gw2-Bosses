[![](https://discordapp.com/api/guilds/410828272679518241/widget.png?style=banner2)](https://discord.gg/Mvk7W7gjE4)
[![](https://discordapp.com/api/guilds/372890476082364427/widget.png?style=banner2)](https://discord.gg/hardstuck)

![](https://img.shields.io/github/downloads/Sognus/GW2-Bosses/total?style=for-the-badge&labelColor=%23131519&color=%230F79AA)
![](https://img.shields.io/github/v/release/Sognus/GW2-Bosses?style=for-the-badge&labelColor=%23131519&color=%230F79AA)

---

# GW2 Bosses

GW2 Addon that adds metaevent and core world bosses notification timers to in-game map. 

## Installation
Install via the [Nexus](https://raidcore.gg/Nexus) Addon Library or download the latest `GW2-Bosses.dll` from the [Releases](https://github.com/Sognus/GW2-Bosses/releases) and place in `<Guild Wars 2>/addons`

## Keybinds
`CTRL+SHIFT+B` - Toggle visibility of events on ingame map

`CTRL+SHIFT+O` - Toggle visibility of notification box

## Settings

Limited settings are available through `<GW2 Folder>\addons\World Bosses\settings.json`.

- `showDebugCrosshair` - if enabled: renders red cross in debug build, used mostly for debugging
- `enableDPIScaleOverride` - use DPI scaling specified by `DPIScaleOverride` instead of the system one to render Events
- `DPIScaleOverride` - DPI based scaling compared to 1.0 (100%) = 96 DPI
- `useNexusNotifications` - enable experimental nexus alerts for certain events
- `render` - show events on map
- `showNotifications` - show notification box for core world boss events  
- `additionalNotifyOffsetIndex` - choice selected from `additionalOffsetChoices` (indexed from 0)
- `additionalOffsetChoices` -  available choices of showing events into future in format `{offset minutes, "tooltip"}`

## Customization

Currently there are not many options to customize rendered events. Limited editor is available.

### Event editor and selecting event to edit

You can find event editor in **Nexus options -> Addons -> World bosses -> Editor**. 

- You can either select an event from the dropdown menu (Note: If the event is numbered, the number indicates its order of occurrence from midnight, starting at 1)
- Alternatively, you can select an event by pressing CTRL+CLICK on it in the map.
- An event selected for editing that is enabled will have a green outline to indicate selection.
- An event selected for editing that is disabled will have a red outline to indicate selection.
- You can change its location by adjusting the numerical values in the form.
- You can enable or disable its rendering using **enabled** dropdown.
- Changing the base color for an event is currently not implemented.
- Press the save button to apply changes (unsaved changes will be discarded).


## Troubleshooting and feature requests

If you have any issue, want something to change or you desire completely new feature; please always create new github issue and please do that even if I give you OK on other communication platform (like Discord). 

If you happen to encounter any bugs or crashes, before you submit an issue, please try to reset addon's database by deleting its `<GW2 Folder>\addons\World Bosses\data.json`,
should issue persist after reset, please also provide following:
- `<GW2 Folder>\addons\Nexus.log`
- `<GW2 Folder>\addons\arcdps\arcdps.log` (if arcdps is also installed)
- `<GW2 Folder>\addons\arcdps\arcdps_lastcrash.log` (if arcdps is also installed)

## Example - Heart of Thorns
![](https://i.imgur.com/JMjTHhX.png)

## Example - Secrets of the Obscure
![](https://i.imgur.com/3nGfwVY.png)