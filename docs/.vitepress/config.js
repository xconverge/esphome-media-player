export default {
  base: '/esphome-media-player/',
  title: 'ESPHome Media Player',
  description: 'A media controller for Home Assistant',
  ignoreDeadLinks: true,
  themeConfig: {
    socialLinks: [
      { icon: 'github', link: 'https://github.com/jtenniswood/esphome-media-player' },
    ],
    sidebar: [
      {
        text: 'About',
        link: '/',
      },
      {
        text: 'Installation',
        link: '/installation',
      },
      {
        text: 'Devices',
        items: [
          { text: 'ESP32-S3 4848S040 (4")', link: '/devices/esp32-s3-4848s040' },
          { text: 'ESP32-P4 JC8012P4A1 (10.1")', link: '/devices/esp32-p4-jc8012p4a1' },
        ],
      },
      {
        text: 'Features',
        items: [
          { text: 'Firmware Updates', link: '/features/firmware-updates' },
          { text: 'Speaker Grouping', link: '/features/speaker-grouping' },
          { text: 'TV Source', link: '/features/tv-source' },
          { text: 'Settings', link: '/features/settings' },
        ],
      },
      {
        text: 'Advanced',
        items: [
          { text: 'Manual Setup', link: '/advanced/manual-setup' },
          { text: 'Troubleshooting', link: '/advanced/troubleshooting' },
          { text: 'Raising an Issue', link: '/advanced/raising-an-issue' },
        ],
      },
    ],
  },
}
