find_program(
  GBAFIX_BINARY
  gbafix
  DOC "gbafix binary to make a valid GBA ROM header"
  REQUIRED
)

find_program(
  GENROMFS_BINARY
  genromfs
  DOC "genromfs binary to build the romfs for the system"
  REQUIRED
)
