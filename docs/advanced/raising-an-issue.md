# Raising an Issue

If something isn't working as expected, you can open an issue on GitHub so it can be investigated and fixed.

## Before you open an issue

A few things to check first:

1. **Read the [Troubleshooting](/advanced/troubleshooting) page** — it covers the most common problems and their solutions.
2. **Flash the latest firmware** — use the [web installer](/installation) to reflash your device with the newest build. Bugs are fixed regularly, and the issue you're seeing may already be resolved in a newer version.
3. **Search [existing issues](https://github.com/jtenniswood/esphome-media-player/issues)** — someone may have already reported the same problem. If so, add a comment or reaction to that issue instead of opening a duplicate.

## How to open an issue

1. Go to the [Issues page on GitHub](https://github.com/jtenniswood/esphome-media-player/issues).
2. Click **New issue**.
3. Give it a clear, descriptive title (e.g. "Album artwork not loading for Spotify via Sonos").
4. In the description, include the details listed below.
5. Submit the issue.

::: tip
You'll need a [GitHub account](https://github.com/signup) to open an issue. It's free.
:::

## What to include

The more detail you provide, the faster the problem can be diagnosed. Please include:

| Detail | Example |
|--------|---------|
| **Device** | Guition ESP32-S3 4848S040 |
| **Firmware version** | Latest from web installer, or commit hash if using manual setup |
| **Media player type** | Sonos, Google Cast, Music Assistant, etc. |
| **Home Assistant version** | 2026.3.0 |
| **What you expected** | Album artwork should display |
| **What actually happened** | Artwork area stays blank |
| **Steps to reproduce** | 1. Play a track on Spotify via Sonos 2. Check the display |

If relevant, also attach:

- **Screenshots or photos** of the display showing the problem.
- **Device logs** from the ESPHome dashboard or Home Assistant logs (Settings → System → Logs).
- **ESPHome YAML** if you're using a custom configuration via [Manual Setup](/advanced/manual-setup).

## Feature requests

Feature requests are welcome too. Open an issue with **[Feature request]** in the title and describe what you'd like to see and why it would be useful.

You can also browse [existing feature requests](https://github.com/jtenniswood/esphome-media-player/issues?q=is%3Aissue+is%3Aopen+feature+request) to upvote ideas you'd like to see implemented.
