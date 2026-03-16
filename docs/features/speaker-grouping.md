# Speaker Grouping

Control multi-room speaker groups directly from the touchscreen panel. The settings panel (swipe down) shows a speaker list on the right side where you can add or remove speakers from the group and adjust individual volumes.

![Speaker grouping panel](/images/guition-esp32-p4-jc8012p4a1-multi-speaker.jpg)

> [!WARNING]
> This feature is in **beta**. If you encounter any issues, please [open an issue on GitHub](https://github.com/jtenniswood/esphome-media-player/issues).

## How it works

- The panel auto-discovers all speakers from the configured integration in your Home Assistant instance
- The currently selected speaker is shown first
- The main volume adjusts all grouped speakers proportionally, keeping their relative balance
- Per-speaker `−` / `+` buttons let you fine-tune individual volumes within the group
- The toggle states update automatically when the group changes

## Compatibility

This feature relies on Home Assistant's `media_player.join` and `media_player.unjoin` services. These are only available on speaker platforms that support grouping. If your speakers don't support these services, the grouping controls will not work.


| Platform             | Supported    | Integration name       |
| -------------------- | ------------ | ---------------------- |
| Sonos                | Yes          | `sonos`                |
| Google Cast          | Yes          | `cast`                 |
| HEOS (Denon/Marantz) | Yes          | `heos`                 |
| Yamaha MusicCast     | Yes          | `yamaha_musiccast`     |
| LinkPlay             | Yes          | `linkplay`             |
| Bluesound            | Yes          | `bluesound`            |
| Bang & Olufsen       | Yes          | `bang_olufsen`         |
| Music Assistant      | Experimental | `mass`                 |

> [!TIP] Music Assistant
> Music Assistant support is **experimental** — it has not been fully tested and may not work with all MA player providers. Only same-type players (e.g. all Sonos or all Cast) can be grouped via sync groups. Cross-provider "universal groups" are managed within Music Assistant's own UI. If you encounter issues, please [open an issue on GitHub](https://github.com/jtenniswood/esphome-media-player/issues).


## Setup the Speaker Group sensor

Add a template sensor to your Home Assistant `configuration.yaml` that discovers speakers from your integration. Choose the section below that matches your setup.

### Standard integrations

For Sonos, Google Cast, HEOS, and other integrations listed in the compatibility table, add the following sensor, replacing `sonos` with the integration name from the table:

```yaml
template:
  - sensor:
      - name: "Speaker Group"
        unique_id: speaker_group
        state: >
          {%- set s = integration_entities("sonos") | select("match", "media_player") | list -%}
          {{ s | count }}
        attributes:
          data: >
            {%- set s = integration_entities("sonos") | select("match", "media_player") | list -%}
            {{ s | map("replace", "media_player.", "") | join(",") }}|{{ s | map("state_attr", "friendly_name") | join(",") }}|{{ s | map("state_attr", "volume_level") | join(",") }}
```

### Music Assistant (Experimental)

> [!WARNING]
> Music Assistant support is **experimental**. The template below filters out MA group entities so only individual players appear in the speaker list. If grouping does not work correctly with your player providers, please [open an issue](https://github.com/jtenniswood/esphome-media-player/issues).

Music Assistant creates group entities alongside individual players. The `reject` filters below exclude these so only physical speakers appear in the panel:

```yaml
template:
  - sensor:
      - name: "Speaker Group"
        unique_id: speaker_group
        state: >
          {%- set s = integration_entities("mass")
              | select("match", "media_player")
              | reject("is_state_attr", "mass_player_type", "group")
              | reject("is_state_attr", "mass_player_type", "sync_group")
              | list -%}
          {{ s | count }}
        attributes:
          data: >
            {%- set s = integration_entities("mass")
                | select("match", "media_player")
                | reject("is_state_attr", "mass_player_type", "group")
                | reject("is_state_attr", "mass_player_type", "sync_group")
                | list -%}
            {{ s | map("replace", "media_player.", "") | join(",") }}|{{ s | map("state_attr", "friendly_name") | join(",") }}|{{ s | map("state_attr", "volume_level") | join(",") }}
```

### Verify the sensor

Restart Home Assistant after saving. To verify, check **Developer Tools → States** and look for `sensor.speaker_group` — the state should show the number of speakers and the `data` attribute should contain the pipe-delimited speaker info.

> **Migrating from the old UI helper?** If you previously created a `Speaker Group` helper via **Settings → Helpers**, delete it first to avoid a naming conflict.

## Behavior

- The speaker list appears in the settings panel (swipe down) when there are at least two speakers from the configured integration

## Music Assistant notes

Music Assistant support is experimental. Keep the following in mind:

- **Same-type grouping only** — `media_player.join` creates sync groups, which require all players to use the same provider (e.g. all Sonos or all Cast). Attempting to group players from different providers will fail silently.
- **Universal groups** — to group speakers across different providers, use Music Assistant's own UI to create a universal group. These do not provide perfectly synchronized audio.
- **`group_members` format** — MA should expose this attribute in the same comma-separated format as other integrations. If the toggle states in the speaker panel do not update correctly after joining or unjoining, the format may differ for your setup — please [report it](https://github.com/jtenniswood/esphome-media-player/issues).