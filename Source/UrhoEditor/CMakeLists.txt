# Define target name
set (TARGET_NAME UrhoEditor)
macro(source_group_by_dir source_files)
    if(MSVC)
        set(sgbd_cur_dir ${CMAKE_CURRENT_SOURCE_DIR})
        foreach(sgbd_file ${${source_files}})
            string(REGEX REPLACE ${sgbd_cur_dir}/\(.*\) \\1 sgbd_fpath ${sgbd_file})
            string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
            string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
            string(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})
            if(sgbd_nogroup)
                set(sgbd_group_name "\\")
            endif(sgbd_nogroup)
            source_group(${sgbd_group_name} FILES ${sgbd_file})
        endforeach(sgbd_file)
    endif(MSVC)
endmacro(source_group_by_dir)

# Find Urho3D library
find_package (Urho3D REQUIRED)
include_directories (${URHO3D_INCLUDE_DIRS})

file(GLOB SOURCE_FILES_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h )
#mvc
#ctrl
file(GLOB SOURCE_FILES_CMD ${CMAKE_CURRENT_SOURCE_DIR}/mvc/ctrl/cmd/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/mvc/ctrl/cmd/*.h)
file(GLOB SOURCE_FILES_DELEGATE ${CMAKE_CURRENT_SOURCE_DIR}/mvc/ctrl/delegate/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/mvc/ctrl/delegate/*.h)
file(GLOB SOURCE_FILES_EVT ${CMAKE_CURRENT_SOURCE_DIR}/mvc/ctrl/event/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/mvc/ctrl/event/*.h)
file(GLOB SOURCE_FILES_RES ${CMAKE_CURRENT_SOURCE_DIR}/mvc/ctrl/res/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/mvc/ctrl/res/*.h)
file(GLOB SOURCE_FILES_SCENE ${CMAKE_CURRENT_SOURCE_DIR}/mvc/ctrl/scene/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/mvc/ctrl/scene/*.h)

#view
file(GLOB SOURCE_FILES_VIEWS ${CMAKE_CURRENT_SOURCE_DIR}/mvc/view/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/mvc/view/*.h)
file(GLOB SOURCE_FILES_INSPECTOR ${CMAKE_CURRENT_SOURCE_DIR}/mvc/view/inspectors/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/mvc/view/inspectors/*.h)
file(GLOB SOURCE_FILES_GIZMO ${CMAKE_CURRENT_SOURCE_DIR}/mvc/view/gizmo/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/mvc/view/gizmo/*.h)
file(GLOB SOURCE_FILES_IMGUI ${CMAKE_CURRENT_SOURCE_DIR}/mvc/view/imgui/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/mvc/view/imgui/*.h)
if(WIN32)
  file(GLOB SOURCE_FILES_NDF ${CMAKE_CURRENT_SOURCE_DIR}/mvc/view/ndf/nfd_win.cpp)
elseif(APPLE)
  option(NFD_USE_ALLOWEDCONTENTTYPES_IF_AVAILABLE "Use allowedContentTypes for filter lists on macOS >= 11.0" ON)
  find_library(APPKIT_LIBRARY AppKit)
  if(NFD_USE_ALLOWEDCONTENTTYPES_IF_AVAILABLE)
    include(CheckCXXSourceCompiles)
    check_cxx_source_compiles(
      "
      #include <Availability.h>
      #if !defined(__MAC_OS_X_VERSION_MIN_REQUIRED) || !defined(__MAC_11_0) || __MAC_OS_X_VERSION_MIN_REQUIRED < __MAC_11_0
      static_assert(false);
      #endif
      int main() { return 0; }
      "
      NFD_USE_ALLOWEDCONTENTTYPES
    )
    if(NFD_USE_ALLOWEDCONTENTTYPES)
      find_library(UNIFORMTYPEIDENTIFIERS_LIBRARY UniformTypeIdentifiers)
      if(NOT UNIFORMTYPEIDENTIFIERS_LIBRARY)
        message(FATAL_ERROR "UniformTypeIdentifiers framework is not available even though we are targeting macOS >= 11.0")
      endif()
    endif()
  endif()
  file(GLOB SOURCE_FILES_NDF ${CMAKE_CURRENT_SOURCE_DIR}/mvc/view/ndf/nfd_cocoa.m)
elseif(UNIX AND NOT APPLE)
  # for Linux, we support GTK3 and xdg-desktop-portal
  option(NFD_PORTAL "Use xdg-desktop-portal instead of GTK" OFF)
  if(NOT NFD_PORTAL)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    message("Using GTK version: ${GTK3_VERSION}")
	file(GLOB SOURCE_FILES_NDF ${CMAKE_CURRENT_SOURCE_DIR}/mvc/view/ndf/nfd_gtk.cpp)
  else()
    pkg_check_modules(DBUS REQUIRED dbus-1)
    message("Using DBUS version: ${DBUS_VERSION}")
	file(GLOB SOURCE_FILES_NDF ${CMAKE_CURRENT_SOURCE_DIR}/mvc/view/ndf/nfd_portal.cpp)
  endif()
endif()

#model
file(GLOB SOURCE_FILES_MODEL ${CMAKE_CURRENT_SOURCE_DIR}/mvc/model/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/mvc/model/*.h)

list (APPEND SOURCE_FILES ${SOURCE_FILES_MAIN})
list (APPEND SOURCE_FILES ${SOURCE_FILES_CMD} ${SOURCE_FILES_RES} ${SOURCE_FILES_SCENE} ${SOURCE_FILES_DELEGATE} ${SOURCE_FILES_EVT} ${SOURCE_FILES_GIZMO})
list (APPEND SOURCE_FILES ${SOURCE_FILES_IMGUI} ${SOURCE_FILES_MODEL} ${SOURCE_FILES_VIEWS} ${SOURCE_FILES_INSPECTOR} ${SOURCE_FILES_NDF})
list (APPEND SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/mvc/view/imgui/tolua/imgui_lua_bindings.cpp )
list (APPEND SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/pkgs/gen/EditorLuaAPI.cpp)

source_group_by_dir(SOURCE_FILES)

include_directories(${CMAKE_SOURCE_DIR}/Source/UrhoEditor)
include_directories(${CMAKE_SOURCE_DIR}/Source/UrhoEditor/mvc)
include_directories(${CMAKE_SOURCE_DIR}/Source/UrhoEditor/mvc/view/imgui)
include_directories(${CMAKE_SOURCE_DIR}/Source/UrhoEditor/lib/glfw/include)
if (WIN32)
	if (MSVC)
		link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw/lib/x64/glfw3.lib)
	endif()
	if (MINGW)
		link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw/lib/win32Mingw/libglfw3.a)
	endif()
endif()

set (LIBS Assimp)
set (INCLUDE_DIRS ../ThirdParty/Assimp/include ${CMAKE_CURRENT_BINARY_DIR}/../ThirdParty/Assimp/include)

# Setup target with resource copying
setup_main_executable ()

