sonic3_sources = [
  'main.c',
  'sonic3-rom.c',
  'sonic3-sound.c',
  'sonic3-window.c',
]

sonic3_deps = [
  dependency('gio-2.0', version: '>= 2.50'),
  dependency('gtk4', version: '>= 4.0'),
  dependency('gstreamer-1.0', version: '>= 1.6.0'),
  dependency('libarchive'),
  dependency('libadwaita-1', version: '>= 1.0'),
]

gnome = import('gnome')

sonic3_sources += gnome.compile_resources('sonic3-resources',
  'sonic3.gresource.xml',
  c_name: 'sonic3'
)

executable('sonic3air-launcher', sonic3_sources,
  dependencies: sonic3_deps,
  install: true,
)
