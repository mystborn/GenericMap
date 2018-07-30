add_library(fnv1a fnv1a.c fnv1a.h)
add_executable(demo main.c map.c map.h)
target_link_libraries(demo fnv1a GenericMap)