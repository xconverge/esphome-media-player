# TV Source

![TV Source](../images/guition-esp32-p4-jc8012p4a1-tvsource.jpg)

If your speaker has a "TV" source (e.g. a home theater setup), the controller can show now-playing info from the TV's media player (e.g. Apple TV, Chromecast) when the speaker's source changes to "TV". This feature is entirely optional — the controller works without it.

> [!WARNING]
> This feature is in **beta**. If you encounter any issues, please [open an issue on GitHub](https://github.com/jtenniswood/esphome-media-player/issues).

## How it works

- **Automatic switching** — when the primary media player's source becomes "TV", the UI shows title, artist, artwork, and progress from the secondary TV media player. When the source changes back, the UI reverts to the primary player.
- **Idle state** — when the TV player is idle, off, or on standby, the screen displays "TV" on a black background with playback controls hidden. Controls reappear when the TV player starts playing again.
- **Routed controls** — play/pause, next, and previous are automatically sent to whichever player is active (music or TV).

## Setup

On the device page in Home Assistant (**Settings → Devices & Services → ESPHome** → your device), set the **Sonos Tv Source** field to the entity ID of your TV media player (e.g. `media_player.apple_tv`).

Leave this field empty to disable the feature.

## Compatibility

This feature has been tested with:

- Sonos speakers with a TV source (Sonos Beam, Sonos Arc)
- Apple TV as the secondary media player
- Chromecast as the secondary media player

It should work with any speaker that exposes a "TV" source attribute and any TV media player entity in Home Assistant.
