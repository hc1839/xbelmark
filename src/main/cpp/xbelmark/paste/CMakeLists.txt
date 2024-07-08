list(
  APPEND
  HDR_NAMES

  paste/cmd_args.h
  paste/cmd_args_parser.h
  paste/format.h
  paste/paste.h
)

list(
  APPEND
  SRC_NAMES

  paste/cmd_args_parser.cc
  paste/format.cc
  paste/paste.cc
)

set(HDR_NAMES ${HDR_NAMES} PARENT_SCOPE)
set(SRC_NAMES ${SRC_NAMES} PARENT_SCOPE)
