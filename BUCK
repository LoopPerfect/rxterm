include_defs('//BUCKAROO_DEPS')

cxx_binary(
  name = 'rxterm',
  header_namespace = 'rxterm',
  srcs = glob([
    'rxterm/src/**/*.cpp',
  ]),
  headers = subdir_glob([
    ('rxterm/include', '**/*.hpp'),
    ('rxterm/include', '**/*.h'),
  ]),
  deps = BUCKAROO_DEPS,
)

