target_include_directories("${PROJECT_NAME}" PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_precompile_headers(
	"${PROJECT_NAME}"
	PRIVATE
		"PCH.h"
)
