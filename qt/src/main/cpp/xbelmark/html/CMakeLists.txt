list(
  APPEND
  HDR_NAMES

  html/info_retriever.h
)

list(
  APPEND
  SRC_NAMES

  html/info_retriever.cc
)

set(HDR_NAMES ${HDR_NAMES} PARENT_SCOPE)
set(SRC_NAMES ${SRC_NAMES} PARENT_SCOPE)
