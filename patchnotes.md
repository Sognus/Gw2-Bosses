## Version 1.5.0.0

### Added
- Preferences are now being loaded on addon load and saved on addon unload. 
- Button to reset 

### Changed
- You can now show world bosses up to 6h hours in future (Please note that visibility of larger amount of events may result in degraded performance). If you want to extend this futher, you can manually edit settings.json additionalNotifyOffsetIndex `{minutes, "tooltip"}` 

## Version 1.4.4.0

### Added
- Added Bava Nisos meta event timer. 

## Version 1.4.3.0

### Fixed
- Fixed incorrect location of Svanir Shaman core world boss (this fix should be applied automatically, but if not, delete `addons\World bosses\data.json`)


## Version 1.4.2.0

### Fixed 
- Fixed Elon riverland meta events not being rendered 
- Fixed incorrect offset for janthir wild convergences (this fix should be applied automatically, but if not, delete `addons\World bosses\data.json`)

### Known issues
- Calculation of event render position is slightly wrong for larger interface sizes, especially when changing the map view. This affects older versions as well and has no easy solution at the moment. 
- Change of DPI scale is slightly delayed after checked/unchecked in Guild Wars 2 settings, this is not a bug.
- It is much more easier to select events to edit when zoomed in, it is supposed to be just slight QoL, not main functionality. To implement this, custom WND proc was needed and it is not perfect.


## Version 1.4.1.0

### Added
- Added Janthir Wilds convergence timer 
  - _(Note: choosed Lowlands shore as event location despite map not having meta event. - You can move it in editor if you desire; example: `[37791.0, 14218.0]`)_

### Known issues
- Calculation of event render position is slightly wrong for larger interface sizes, especially when changing the map view. This affects older versions as well and has no easy solution at the moment. 
- Change of DPI scale is slightly delayed after checked/unchecked in Guild Wars 2 settings, this is not a bug.
- It is much more easier to select events to edit when zoomed in, it is supposed to be just slight QoL, not main functionality. To implement this, custom WND proc was needed and it is not perfect.

## Version 1.4.0.0

### Fixed
- Editor save button should now correctly export data to data.json

### Added
- Janthir Syntri meta event timer
- Option to disable event render in  **Nexus options -> Addons -> World bosses -> Editor**, currently edited event is still visible and it has red outline to indicate event render is disabled

### Changed
- Data loading: Fallback load is always being called, base events that are missing in data.json will be automatically added. If you wish to remove event, disable it in editor

### Known issues
- Calculation of event render position is slightly wrong for larger interface sizes, especially when changing the map view. This affects older versions as well and has no easy solution at the moment. 
- Change of DPI scale is slightly delayed after checked/unchecked in Guild Wars 2 settings, this is not a bug.
- It is much more easier to select events to edit when zoomed in, it is supposed to be just slight QoL, not main functionality. To implement this, custom WND proc was needed and it is not perfect.

## Version 1.3.1.0

### Fixed
- Fixed compatibility issue of Convergences event tooltip for linux users (incompatibility with windows' _mkgmtime)

### Added
- Experimental usage of Nexus' alerts for core world boss events (enable/disable in options)

### Changed
- Core world boss event that is not active and is currently being edited is shown as green cog on the map. 
- Debug version now have special debug controls

### Known issues
- Calculation of event render position is slightly wrong for larger interface sizes, especially when changing the map view. This affects older versions as well and has no easy solution at the moment. 
- Change of DPI scale is slightly delayed after checked/unchecked in Guild Wars 2 settings, this is not a bug.
- It is much more easier to select events to edit when zoomed in, it is supposed to be just slight QoL, not main functionality. To implement this, custom WND proc was needed and it is not perfect.

## Version 1.3.0.0

### Added
- Added **Nexus options -> Addons -> World bosses -> Editor** that allows users to change properties of events (currently only location can be changed)  
- Added feature of ctrl+click to select event for **Nexus options -> Addons -> World bosses -> Editor**
- Added light green outline for editor currently selected event
- Addon render is currently DPI aware (see known issues)
- Addon will now match ingame settings `Use DPI scale`
- Added addon options **Nexus options -> Addons -> World bosses -> General** to manually override system's API scale setting. Value 1.0 means 100% DPI (96 on windows). 

### Improved
- Reordered and re-conditioned render calls to skip render if its not desired (performance and input handling improvemenents)

### Changed
- Changed to use Nexus-recommended Imgui submodule for project building
- Changed to use Nexus API revision 3, instead of revision 1

### Known issues
- Calculation of event render position is slightly wrong for larger interface sizes, especially when changing the map view. This affects older versions as well and has no easy solution at the moment. 
- Change of DPI scale is slightly delayed after checked/unchecked in Guild Wars 2 settings, this is not a bug.
- It is much more easier to select events to edit when zoomed in, it is supposed to be just slight QoL, not main functionality. To implement this, custom WND proc was needed and it is not perfect.

## Version 1.2.1.0

### Improved
- Improved performance of meta events circular render 

## Version 1.2.0.0

### Added
- Added addon options to Nexus options -> Addons -> World bosses (options currently limited and not persistent)

### Improved
- Improved upcoming notification box to allow up to 2 hours of upcoming core world bosses via new option in Nexus options -> Addons -> World Bosses
- Improved upcoming and in progress notification boxes to show countdown
- Improved notification map render tooltip to included countdown
- Improved build and release process to read addon version from one location instead having to change version at multiple places


### Changed
- Changed core boss events to deque backend (might increase memory usage)
- Switched from batch to python based pre-build script
- Any core world bosses upcoming for more than 15 minutes are rendered with gray icon

---

## Version 1.1.2.0

### Fixed
- Fixed a bug that caused core world bosses map notification to be rendered when map was not opened
- Fixed a bug that caused core world bosses map notification to be rendered outside gameplay
- Reference cleanup update: Addon shortcut unregister corrected to unregister simple shortcut. 

## Version 1.1.1.0

### Fixed
- Reference cleanup: Addon shortcut unregister on addon unload

## Version 1.1.0.0

### Added
- Core world bosses notification locations are shown on map as well

### Changed
- Core world bosses notification box default behavior is hidden on start.  

---

## Version 1.0.0.0:

It is recommended to delete data.json in addon's folder if it exist to avoid any issues. 

### Added
- Added notifications
- Added core bosses support (notification only)
- Added keybind to toggle event render, currently CTRL+SHIFT+B
- Added keybind to toggle notifications render, currently CTRL+SHIFT+O
### Improved
- Improved interface scaling for events map render
- Improved performance of event render by culling events outside visible area
- Improved events backend to support event customization through data.json modification
### Fixed
- Fixed wrong event color for Serpent's Ire
- Fixed Convergences timer being misaligned because event is on 3h cooldown and not 2h

---

## Version 0.9.1.0:
- Fixed NexusLink crash
- Fixed an issue when events were rendered in non-gameplay

---

## Version 0.9.0.1:
- Initial public release of addon