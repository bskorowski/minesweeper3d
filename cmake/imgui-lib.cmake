add_library(imgui STATIC
   ./vendor/imgui/imgui.h
   ./vendor/imgui/imgui.cpp
   ./vendor/imgui/imgui_draw.cpp
   ./vendor/imgui/imgui_internal.h
   ./vendor/imgui/imgui_tables.cpp
   ./vendor/imgui/imgui_widgets.cpp
   ./vendor/imgui/imstb_rectpack.h
   ./vendor/imgui/imstb_textedit.h
   ./vendor/imgui/imstb_truetype.h
   # OpenGL backend
   ./vendor/imgui/backends/imgui_impl_glfw.cpp
   ./vendor/imgui/backends/imgui_impl_glfw.h
   ./vendor/imgui/backends/imgui_impl_opengl3.cpp
   ./vendor/imgui/backends/imgui_impl_opengl3.h
)

target_include_directories(imgui SYSTEM PUBLIC
   vendor/imgui
   vendor/imgui/backends
)
target_compile_definitions(imgui PUBLIC IMGUI_USER_CONFIG="imconfig_overrides.h")

target_link_libraries(imgui PUBLIC glfw)
