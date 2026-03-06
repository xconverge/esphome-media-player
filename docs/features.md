# Features

Overview of what the media controller does. Many features are user-configurable from the device page in Home Assistant (**Settings → Devices & Services → ESPHome** → your device) — no YAML or reflashing needed. See [Settings](/configurable-settings) for the full reference.

## Album art display

Full-screen album art is loaded from your Home Assistant instance. When a new track starts, the current artwork dims to 40% while the new image downloads, then fades back to full brightness. If artwork doesn't appear, see [Troubleshooting](/troubleshooting#the-artwork-isnt-loading).

## Accent color

The dominant color is extracted from the album art and applied to the UI background and play/pause button. The color is also exposed as an **Accent Color** RGB light entity in Home Assistant, so you can use it in automations (e.g. syncing room lights to match the album art).

## Now playing info

The screen shows song title, artist, elapsed and remaining time, and a progress bar. The bar updates every second with smooth interpolation between position updates from Home Assistant. Tap the time label to toggle between elapsed/remaining and elapsed/total duration display. The default mode is set by the **Playback: Show Remaining Time** switch in [Settings](/configurable-settings).

## Auto-hide track info (4" only)

On the 4" panel, when a new track is requested, the overlay (title, artist, time, play/pause) appears automatically if the duration is greater than 0. The hide timer starts when artwork has finished loading (or when playback starts); the default is 5 seconds. Set **Playback: Track Info Duration** to **0** in [Settings](/configurable-settings) to never show track info when a new track is requested. After the overlay auto-hides, tap the screen to show it again; it then hides only when you tap again or when a new track is requested. When playback is paused, track info is shown by default; the hide timer starts when playback resumes.

The 10.1" panel uses a side-panel layout where the track info is always visible alongside the album art, so this setting does not apply.

## TV source mode (optional)

If your speaker has a "TV" source (e.g. a home theater setup), the controller can optionally show now-playing info from the TV's media player (e.g. Apple TV, Chromecast) when the speaker's source changes to "TV". This feature is entirely optional and the controller works without it.

- **Automatic switching** — when the primary media player's source becomes "TV", the UI shows title, artist, artwork, and progress from the secondary TV media player. When the source changes back, the UI reverts to the primary player.
- **Idle state** — when the TV player is idle, off, or on standby, the screen displays "TV" on a black background with playback controls hidden. Controls reappear when the TV player starts playing again.
- **Routed controls** — play/pause, next, and previous are automatically sent to whichever player is active (music or TV).

To enable this, set the optional **Sonos Tv Source** field in the device's configuration to the entity ID of your TV media player (e.g. `media_player.apple_tv`). Leave it empty to disable. See [Settings](/configurable-settings#tv-source) for details.

## Speaker grouping

Group and ungroup multi-room speakers directly from the touchscreen. A speaker icon appears on the main screen — tap it to open a panel listing all your speakers with toggle switches to group or ungroup them. Changes happen instantly via the standard Home Assistant `media_player.join` and `media_player.unjoin` services.

This feature works with any speaker platform that supports grouping in Home Assistant (e.g. Sonos, Google Cast, HEOS, MusicCast, LinkPlay, Bluesound). Requires a one-time setup of a template helper in Home Assistant — see [Speaker Grouping](/speaker-grouping) for instructions.

## Touch controls

- **Play / Pause** — button in the bottom-right toggles playback.
- **Next / Previous** — swipe left or right to change tracks.
- **Volume** — swipe down to open the settings panel with an arc dial. Drag the knob or use + / − for 1% steps. Swipe up to close.
- **Hide / Show UI** — tap the screen during playback to hide or show the overlay (4" only).

## Screensaver

When playback is paused, the device uses a two-stage screensaver:

1. After **Timeout: Dimming** elapses, the screen dims to **Day/Night: Dim Brightness**.
2. After **Timeout: Screen Off** elapses, the screen turns off (unless disabled by the **Day/Night: Screen Saver** switch, in which case it stays dimmed).

Active brightness (**Day/Night: Active Brightness**) adjusts automatically between day and night based on the `sun.sun` entity in Home Assistant. All of these settings are configurable from the device page in Home Assistant (see [Settings](/configurable-settings)).
