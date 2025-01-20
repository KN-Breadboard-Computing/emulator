# Emulator

## Building and running 
```bash
mkdir build
cd build
cmake ..
cmake --build .
```
You can find the CliApp executable in
```bash
./build/cli_app/EmulatorCliApp
```

You can test emulator running script
```bash
./scripts/test.sh
```

## Contributing
Before pushing any changes run this script
```bash
./scripts/format.sh
```