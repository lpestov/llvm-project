set(LLVM_OPTIONAL_SOURCES
  SampleInput.cpp
)

add_llvm_library(TypeInfoPrinter MODULE TypeInfoPrinter.cpp PLUGIN_TOOL clang)

if(WIN32 OR CYGWIN)
  set(LLVM_LINK_COMPONENTS
    Support
  )
  clang_target_link_libraries(TypeInfoPrinter PRIVATE
    clangAST
    clangBasic
    clangFrontend
  )
endif()
