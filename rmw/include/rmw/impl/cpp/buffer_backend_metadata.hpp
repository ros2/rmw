// Copyright 2026 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef RMW__IMPL__CPP__BUFFER_BACKEND_METADATA_HPP_
#define RMW__IMPL__CPP__BUFFER_BACKEND_METADATA_HPP_

#include <algorithm>
#include <cstddef>
#include <map>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace rmw
{
namespace impl
{
namespace cpp
{

inline constexpr char kBufferBackendMetadataPrefix[] = "backends:";

inline std::string
escape_buffer_backend_metadata_field(const std::string & input)
{
  std::string out;
  out.reserve(input.size());
  for (char c : input) {
    switch (c) {
      case '%':
        out += "%25";
        break;
      case ';':
        out += "%3B";
        break;
      case ':':
        out += "%3A";
        break;
      case '/':
        out += "%2F";
        break;
      default:
        out += c;
        break;
    }
  }
  return out;
}

inline int
buffer_backend_metadata_hex_value(char c)
{
  if (c >= '0' && c <= '9') {
    return c - '0';
  }
  if (c >= 'A' && c <= 'F') {
    return 10 + (c - 'A');
  }
  if (c >= 'a' && c <= 'f') {
    return 10 + (c - 'a');
  }
  return -1;
}

inline std::string
unescape_buffer_backend_metadata_field(const std::string_view input)
{
  std::string out;
  out.reserve(input.size());
  for (size_t i = 0; i < input.size(); ++i) {
    if (input[i] == '%' && i + 2 < input.size()) {
      int hi = buffer_backend_metadata_hex_value(input[i + 1]);
      int lo = buffer_backend_metadata_hex_value(input[i + 2]);
      if (hi >= 0 && lo >= 0) {
        out += static_cast<char>((hi << 4) | lo);
        i += 2;
        continue;
      }
    }
    out += input[i];
  }
  return out;
}

template<typename MapT>
std::string
serialize_buffer_backend_metadata(const MapT & metadata)
{
  if (metadata.empty()) {
    return {};
  }

  std::vector<std::pair<std::string, std::string>> entries;
  entries.reserve(metadata.size());
  for (const auto & pair : metadata) {
    if (!pair.first.empty()) {
      entries.emplace_back(pair.first, pair.second);
    }
  }
  if (entries.empty()) {
    return {};
  }

  std::sort(
    entries.begin(), entries.end(),
    [](const auto & lhs, const auto & rhs) {
      return lhs.first < rhs.first;
    });

  std::string output{kBufferBackendMetadataPrefix};
  for (size_t i = 0; i < entries.size(); ++i) {
    output += escape_buffer_backend_metadata_field(entries[i].first);
    output += ":";
    output += escape_buffer_backend_metadata_field(entries[i].second);
    if (i + 1 < entries.size()) {
      output += ";";
    }
  }
  return output;
}

inline std::map<std::string, std::string>
parse_buffer_backend_metadata(const char * serialized_metadata)
{
  std::map<std::string, std::string> result;
  if (!serialized_metadata || serialized_metadata[0] == '\0') {
    return result;
  }

  std::string_view input{serialized_metadata};
  constexpr std::string_view prefix{kBufferBackendMetadataPrefix};
  if (input.rfind(prefix, 0) != 0) {
    return result;
  }

  std::string_view body = input.substr(prefix.size());
  if (body.empty()) {
    return result;
  }

  size_t start = 0;
  while (start <= body.size()) {
    size_t sep = body.find(';', start);
    std::string_view entry = body.substr(
      start, sep == std::string_view::npos ? std::string_view::npos : sep - start);
    if (!entry.empty()) {
      size_t colon = entry.find(':');
      std::string name = unescape_buffer_backend_metadata_field(entry.substr(0, colon));
      std::string metadata;
      if (colon != std::string_view::npos) {
        metadata = unescape_buffer_backend_metadata_field(entry.substr(colon + 1));
      }
      if (!name.empty()) {
        result[std::move(name)] = std::move(metadata);
      }
    }
    if (sep == std::string_view::npos) {
      break;
    }
    start = sep + 1;
  }
  return result;
}

}  // namespace cpp
}  // namespace impl
}  // namespace rmw

#endif  // RMW__IMPL__CPP__BUFFER_BACKEND_METADATA_HPP_
