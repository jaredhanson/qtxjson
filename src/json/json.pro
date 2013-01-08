include(../../modules/qtx_json.pri)
include(../../mkspecs/features/qtx_config.prf)
include(../../mkspecs/features/qtx_module.prf)


include(json.pri)

macx {
  LIBS += $$(PWD)/../../vendor/lib/osx/x86_64/libyajl_s.a
}
