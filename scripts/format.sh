git ls-files | grep -E '\.(c|h)$' | grep -v '^external/' | xargs clang-format -i
