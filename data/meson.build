desktop_file = i18n.merge_file(
  input: 'org.sonic3air.Sonic3AIR.desktop.in',
  output: 'org.sonic3air.Sonic3AIR.desktop',
  type: 'desktop',
  po_dir: '../po',
  install: true,
  install_dir: get_option('datadir') / 'applications'
)

desktop_utils = find_program('desktop-file-validate', required: false)
if desktop_utils.found()
  test('Validate desktop file', desktop_utils,
    args: [desktop_file]
  )
endif

install_data('org.sonic3air.Sonic3AIR.png',
  install_dir: get_option('datadir') / 'icons/hicolor/256x256/apps'
)

appstream_file = i18n.merge_file(
  input: 'org.sonic3air.Sonic3AIR.appdata.xml.in',
  output: 'org.sonic3air.Sonic3AIR.appdata.xml',
  po_dir: '../po',
  install: true,
  install_dir: get_option('datadir') / 'appdata'
)

appstream_util = find_program('appstream-util', required: false)
if appstream_util.found()
  test('Validate appstream file', appstream_util,
    args: ['validate', appstream_file]
  )
endif

install_data('org.sonic3air.Sonic3AIR.gschema.xml',
  install_dir: get_option('datadir') / 'glib-2.0/schemas'
)

compile_schemas = find_program('glib-compile-schemas', required: false)
if compile_schemas.found()
  test('Validate schema file', compile_schemas,
    args: ['--strict', '--dry-run', meson.current_source_dir()]
  )
endif
