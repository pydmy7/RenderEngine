add_library(nlohmannjson OBJECT src/json.cpp)
target_include_directories(nlohmannjson PUBLIC include)
add_library(nlohmann::json ALIAS nlohmannjson)
