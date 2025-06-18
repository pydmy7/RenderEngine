if (NOT DEFINED folderPath OR NOT DEFINED destPath)
    message(FATAL_ERROR "folderPath and destPath must be defined.")
endif()

file(GLOB toml_files "${folderPath}/*.toml")

file(WRITE "${destPath}" "")
foreach(toml IN LISTS toml_files)
    file(READ "${toml}" content)
    file(APPEND "${destPath}" "${content}")
endforeach()
