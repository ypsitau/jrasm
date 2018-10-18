add_executable(jrpcgedit
	jrpcgedit.cpp
	Document.cpp
	ErrorDialog.cpp
	FrameMain.cpp
	PanelMain.cpp
	PCGBrowser.cpp
	PCGDataInfo.cpp
	PCGEditor.cpp
	PCGPageInfo.cpp
)

include_directories(
	${PROJECT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}/guest/wxWidgets-3.1.1/lib/wx/include/osx_cocoa-unicode-static-3.1/
	${PROJECT_SOURCE_DIR}/guest/wxWidgets-3.1.1/include/
)

add_definitions(
	-D_FILE_OFFSET_BITS=64
	-D__WXMAC__
	-D__WXOSX__
	-D__WXOSX_COCOA__	
)

set(CMAKE_EXE_LINKER_FLAGS "-framework IOKit -framework Carbon -framework Cocoa")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework System -framework OpenGL")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework WebKit -framework Security")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L${PROJECT_SOURCE_DIR}/guest/wxWidgets-3.1.1/lib")

target_link_libraries(jrpcgedit
	jrasmcore
	wx_baseu-3.1
	wx_baseu_net-3.1
	wx_baseu_xml-3.1
	wx_osx_cocoau_adv-3.1
	wx_osx_cocoau_aui-3.1
	wx_osx_cocoau_core-3.1
	wx_osx_cocoau_gl-3.1
	wx_osx_cocoau_html-3.1
	wx_osx_cocoau_media-3.1
	wx_osx_cocoau_propgrid-3.1
	wx_osx_cocoau_qa-3.1
	wx_osx_cocoau_ribbon-3.1
	wx_osx_cocoau_richtext-3.1
	wx_osx_cocoau_stc-3.1
	wx_osx_cocoau_webview-3.1
	wx_osx_cocoau_xrc-3.1
	wxjpeg-3.1
	wxpng-3.1
	wxregexu-3.1
	wxscintilla-3.1
	wxtiff-3.1
	lzma
	expat
	z
	pthread
	iconv 
)

install(TARGETS jrpcgedit DESTINATION bin)
