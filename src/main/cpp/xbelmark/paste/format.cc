#include "xbelmark/paste/format.h"

#include <array>
#include <map>
#include <stdexcept>
#include <string>

namespace xbelmark {
namespace enumeration {

using xbelmark::paste::Format;

template <>
std::string EnumNameOf(Format enumerator) {
  static const std::map<Format, std::string> mapping = {
    { Format::URL, "URL" },
    { Format::XBEL, "XBEL" }
  };

  return mapping.at(enumerator);
}

template <>
Format EnumValueOf(const std::string &name) {
  static const std::array<Format, 2> enumerators = {
    Format::URL,
    Format::XBEL
  };

  for (const auto &enumerator : enumerators) {
    if (EnumNameOf(enumerator) == name) {
      return enumerator;
    }
  }

  throw std::out_of_range("Invalid enumerator name: " + name);
}

} // namespace enumeration
} // namespace xbelmark
