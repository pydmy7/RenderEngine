set(target demo)

addExecutable(${target})
target_link_libraries(${target} PRIVATE hps::hps)
copyDll(${target})
