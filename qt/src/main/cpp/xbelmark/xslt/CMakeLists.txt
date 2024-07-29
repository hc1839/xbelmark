list(
  APPEND
  HDR_NAMES

  xslt/cmd_args.h
  xslt/cmd_args_parser.h
  xslt/ext/date_time.h
  xslt/xslt.h
)

list(
  APPEND
  SRC_NAMES

  xslt/cmd_args_parser.cc
  xslt/ext/date_time.cc
  xslt/xslt.cc
)

set(HDR_NAMES ${HDR_NAMES} PARENT_SCOPE)
set(SRC_NAMES ${SRC_NAMES} PARENT_SCOPE)
