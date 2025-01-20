cmake --build build
cp ../computer/config/instructions.json build/instructions.json 
(cd build/tests && ./baseTest)
(cd build/tests && ./moveTest)
(cd build/tests && ./aluTest)
