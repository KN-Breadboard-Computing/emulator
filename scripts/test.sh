cmake --build build
curl -L https://raw.githubusercontent.com/KN-Breadboard-Computing/computer/main/config/instructions.json -o ./build/instructions.json
(cd build/tests && ./baseTest)
(cd build/tests && ./moveTest)
(cd build/tests && ./aluTest)
