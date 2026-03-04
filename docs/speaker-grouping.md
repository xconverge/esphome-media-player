# Speaker Grouping

Control multi-room Sonos speaker groups directly from the touchscreen panel. When the selected media player is a Sonos speaker, the settings panel (swipe down) shows a speaker list on the right side where you can add or remove speakers from the group and adjust individual volumes.

![Speaker grouping panel](./images/speaker-grouping.png)

## How it works

- The panel auto-discovers all Sonos speakers in your Home Assistant instance
- The currently selected speaker is shown first with its toggle locked on (it's the group leader)
- Toggle other speakers on to group them, or off to remove them
- Grouped speakers are sorted to the top of the list
- The main volume dial adjusts all grouped speakers proportionally, keeping their relative balance
- Per-speaker `−` / `+` buttons let you fine-tune individual volumes within the group
- Changes take effect immediately via the standard `media_player.join` and `media_player.unjoin` services
- The toggle states update automatically when the group changes (from the panel, the HA app, or voice commands)

## Setup

Three template sensor helpers need to be created in Home Assistant. This is done entirely through the UI — no YAML editing required.

### 1. Create the speaker IDs sensor

1. Go to **Settings → Devices & Services → Helpers** tab
2. Click **+ Create Helper** → **Template** → **Template a sensor**
3. Fill in the following:

| Field | Value |
|-------|-------|
| Name | `Sonos Speaker IDs` |
| State template | `{{ integration_entities("sonos") \| select("match", "media_player") \| join(",") }}` |

Leave all other fields as default and click **Submit**.

### 2. Create the speaker names sensor

1. Click **+ Create Helper** → **Template** → **Template a sensor**
2. Fill in the following:

| Field | Value |
|-------|-------|
| Name | `Sonos Speaker Names` |
| State template | `{{ integration_entities("sonos") \| select("match", "media_player") \| map("state_attr", "friendly_name") \| join(",") }}` |

Leave all other fields as default and click **Submit**.

### 3. Create the speaker volumes sensor

1. Click **+ Create Helper** → **Template** → **Template a sensor**
2. Fill in the following:

| Field | Value |
|-------|-------|
| Name | `Sonos Speaker Volumes` |
| State template | `{{ integration_entities("sonos") \| select("match", "media_player") \| map("state_attr", "volume_level") \| join(",") }}` |

Leave all other fields as default and click **Submit**.

### Verify

Go to **Developer Tools → States** and search for the new sensors:

- `sensor.sonos_speaker_ids` — should show a comma-separated list of entity IDs, e.g. `media_player.office,media_player.kitchen,media_player.living_room`
- `sensor.sonos_speaker_names` — should show matching friendly names, e.g. `Office,Kitchen,Living Room`
- `sensor.sonos_speaker_volumes` — should show matching volume levels, e.g. `0.45,0.6,0.3`

The ESPHome panel subscribes to these sensors automatically at boot. No device restart is needed after creating the helpers.

## Behavior

- The speaker list appears in the settings panel (swipe down) when the selected media player is a Sonos device and there are at least two Sonos speakers
- If you change the selected media player to a non-Sonos device, the speaker list hides automatically
- Up to 8 speakers are supported

## State template reference

If you prefer YAML over the UI, you can add these to your `configuration.yaml` or a `packages/` file:

```yaml
template:
  - sensor:
      - name: "Sonos Speaker IDs"
        unique_id: esphome_sonos_speaker_ids
        state: >-
          {{ integration_entities("sonos")
             | select("match", "media_player")
             | join(",") }}

      - name: "Sonos Speaker Names"
        unique_id: esphome_sonos_speaker_names
        state: >-
          {{ integration_entities("sonos")
             | select("match", "media_player")
             | map("state_attr", "friendly_name")
             | join(",") }}

      - name: "Sonos Speaker Volumes"
        unique_id: esphome_sonos_speaker_volumes
        state: >-
          {{ integration_entities("sonos")
             | select("match", "media_player")
             | map("state_attr", "volume_level")
             | join(",") }}
```
