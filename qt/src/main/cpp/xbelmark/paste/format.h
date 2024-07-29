#ifndef XBELMARK_PASTE_FORMAT_H
#define XBELMARK_PASTE_FORMAT_H

#include <string>

#include "xbelmark/enumeration/name.h"

namespace xbelmark {
namespace paste {

/**
 *  Enumeration of the available formats of the output file from the `paste`
 *  subcommand.
 */
enum class Format : int {
  /**
   *  URL format used in Windows.
   */
  URL,

  /**
   *  XBEL format.
   */
  XBEL
};

} // namespace paste
} // namespace xbelmark

namespace xbelmark {
namespace enumeration {

template <>
std::string EnumNameOf(xbelmark::paste::Format enumerator);

template <>
xbelmark::paste::Format EnumValueOf(const std::string &name);

} // namespace enumeration
} // namespace xbelmark

#endif
