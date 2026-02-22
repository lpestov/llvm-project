Запуск: ( из ~/llvm-project )
./build/bin/clang++ -std=c++17 -fsyntax-only ./clang/examples/TypeInfoPrinter/SampleInput.cpp -fplugin=./build/lib/TypeInfoPrinter.so

Тесты:
/build/bin/llvm-lit -sv clang/test/Frontend/plugin-type-info-printer.cpp