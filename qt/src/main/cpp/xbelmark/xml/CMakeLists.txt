list(
  APPEND
  HDR_NAMES

  xml/writer.h
  xml/xpath/xpath.h
)

list(
  APPEND
  SRC_NAMES

  xml/writer.cc
  xml/xpath/xpath.cc
)

set(HDR_NAMES ${HDR_NAMES} PARENT_SCOPE)
set(SRC_NAMES ${SRC_NAMES} PARENT_SCOPE)
