# Define target name
set (TARGET_NAME UrhoEditor)

# Find Urho3D library
find_package (Urho3D REQUIRED)
include_directories (${URHO3D_INCLUDE_DIRS})

file(GLOB SOURCE_FILES_A "./*.cpp")
file(GLOB SOURCE_FILES_B "./**/*.cpp")
file(GLOB SOURCE_FILES_VIEWS "./views/**/*.cpp")
set(SOURCE_FILES ${SOURCE_FILES_A} ${SOURCE_FILES_B} ${SOURCE_FILES_VIEWS} ./imgui/tolua/imgui_lua_bindings.cpp ./pkgs/gen/EditorLuaAPI.cpp )

include_directories(./)
include_directories(./imgui)
include_directories("./lib/glfw/include")
if (WIN32)
	link_libraries(${CMAKE_SOURCE_DIR}/Source/UrhoEditor/lib/glfw/lib/win32Mingw/libglfw3.a)
endif()

set (LIBS Assimp)
set (INCLUDE_DIRS ../ThirdParty/Assimp/include ${CMAKE_CURRENT_BINARY_DIR}/../ThirdParty/Assimp/include)

# Setup target with resource copying
setup_main_executable ()

