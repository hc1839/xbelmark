list(
  APPEND
  HDR_NAMES

  winshell/winshell.h
)

list(
  APPEND
  SRC_NAMES

  winshell/winshell.cc
)

set(HDR_NAMES ${HDR_NAMES} PARENT_SCOPE)
set(SRC_NAMES ${SRC_NAMES} PARENT_SCOPE)
