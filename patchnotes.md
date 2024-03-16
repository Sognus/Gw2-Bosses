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