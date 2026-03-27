#pragma once

#include <string>

namespace esphome {
namespace online_image {

/// Rewrite known CDN artwork URLs to cap the requested resolution and
/// force a decodable format (JPEG).
///
/// Apple Music artwork URLs come in two forms:
///   1. Concrete: /3000x3000bb.jpg  (high-res, can OOM the ESP32)
///   2. Template: /{w}x{h}{c}.{f}   (HA proxy resolves {f} to HEIC)
///
/// The HA media_player_proxy serves whatever format the integration
/// provides — it does NOT honour the URL extension or Accept header.
/// For Apple Music CDN URLs (mzstatic.com) we bypass the proxy entirely
/// and fetch directly from the CDN with a concrete JPEG path, since
/// the CDN is public and DOES respect the filename format.
///
/// URLs that don't match a known pattern are returned unchanged.
inline std::string cap_artwork_url(const std::string &url, int max_dim = 600) {
  std::string dim = std::to_string(max_dim) + "x" + std::to_string(max_dim);

  // Apple Music via HA proxy: extract CDN URL from cache= parameter,
  // rewrite template to concrete JPEG, and fetch directly from CDN.
  auto cache_pos = url.find("cache=");
  if (cache_pos != std::string::npos) {
    std::string cdn_url = url.substr(cache_pos + 6);
    if (cdn_url.find("mzstatic.com") != std::string::npos) {
      auto tpl = cdn_url.find("{w}x{h}");
      if (tpl != std::string::npos) {
        auto slash = cdn_url.rfind('/', tpl);
        if (slash != std::string::npos) {
          return cdn_url.substr(0, slash + 1) + dim + "bb.jpg";
        }
      }
      // Concrete CDN URL inside cache= (no template) — cap dimensions
      auto bb = cdn_url.rfind("bb.");
      if (bb != std::string::npos) {
        auto x = cdn_url.rfind('x', bb);
        if (x != std::string::npos) {
          auto sl = cdn_url.rfind('/', x);
          if (sl != std::string::npos) {
            return cdn_url.substr(0, sl + 1) + dim + cdn_url.substr(bb);
          }
        }
      }
    }
  }

  // Standalone template URL (not behind HA proxy)
  auto tpl_pos = url.find("{w}x{h}");
  if (tpl_pos != std::string::npos) {
    auto slash = url.rfind('/', tpl_pos);
    if (slash != std::string::npos) {
      return url.substr(0, slash + 1) + dim + "bb.jpg";
    }
  }

  // Concrete Apple Music CDN — /{W}x{H}bb.{ext}
  auto bb_pos = url.rfind("bb.");
  if (bb_pos == std::string::npos)
    return url;

  auto x_pos = url.rfind('x', bb_pos);
  if (x_pos == std::string::npos || x_pos == 0)
    return url;

  auto slash_pos = url.rfind('/', x_pos);
  if (slash_pos == std::string::npos)
    return url;

  std::string w_str = url.substr(slash_pos + 1, x_pos - slash_pos - 1);
  std::string h_str = url.substr(x_pos + 1, bb_pos - x_pos - 1);

  if (w_str.empty() || h_str.empty())
    return url;
  for (char c : w_str) {
    if (c < '0' || c > '9') return url;
  }
  for (char c : h_str) {
    if (c < '0' || c > '9') return url;
  }

  int w = std::stoi(w_str);
  int h = std::stoi(h_str);

  if (w <= max_dim && h <= max_dim)
    return url;

  return url.substr(0, slash_pos + 1) + dim + url.substr(bb_pos);
}

}  // namespace online_image
}  // namespace esphome
