# i3-wwm

prepare
```shell
# visual studio (generates vs specific project files)
cmake --preset vs
# other (generates clangd lsp relevant files, requires Ninja)
cmake --preset ninja
```

build
```shell
cmake --build build
```

run
```shell
.\bin\<depends>
```
