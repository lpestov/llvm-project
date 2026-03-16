# `FunctionInstrumentationPass`



## Файлы
- `llvm/include/llvm/Transforms/Utils/FunctionInstrumentation.h`
- `llvm/lib/Transforms/Utils/FunctionInstrumentation.cpp`
- `llvm/lib/Passes/PassRegistry.def`
- `llvm/lib/Passes/PassBuilder.cpp`
- `llvm/lib/Transforms/Utils/CMakeLists.txt`
- `llvm/test/Transforms/Utils/function-instrumentation.ll`

## Запуск из `~/llvm-project`

1. Пересобрать `opt`:

   `cmake --build build --target opt -j 4`

2. Запустить тест:

   `./build/bin/llvm-lit -sv llvm/test/Transforms/Utils/function-instrumentation.ll`

3. Если хочется посмотреть результат вручную:

   `./build/bin/opt -S -passes=function-instrumentation llvm/test/Transforms/Utils/function-instrumentation.ll`