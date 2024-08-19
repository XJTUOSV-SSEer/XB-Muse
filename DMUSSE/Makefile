App_Cpp_Files := $(wildcard CryptoTestingApp/*.cpp)
App_Include_Paths := -IInclude -IApp -I/usr/include -I/usr/include/openssl

App_C_Flags := -fPIC -Wno-attributes $(App_Include_Paths)

App_C_Flags += -DNDEBUG -UEDEBUG -UDEBUG

App_Cpp_Flags := $(App_C_Flags) -std=c++11
App_Link_Flags := -lssl -lcrypto

App_Cpp_Objects := $(App_Cpp_Files:.cpp=.o)

App_Name := cryptoTestingApp

.PHONY: all run

all: $(App_Name) $(Signed_CryptoEnclave_Name)

run: all
CryptoTestingApp/%.o: CryptoTestingApp/%.cpp
	@$(CXX) $(App_Cpp_Flags) -c $< -o $@
	@echo "CXX  <=  $<"
$(App_Name): $(App_Cpp_Objects)
	@$(CXX) $^ -o $@ $(App_Link_Flags)
	@echo "LINK =>  $@"

clean:
	@rm -f $(App_Name) $(CryptoEnclave_Name) $(Signed_CryptoEnclave_Name) $(App_Cpp_Objects) CryptoTestingApp/CryptoEnclave_u.* $(CryptoEnclave_Cpp_Objects) CryptoEnclave/CryptoEnclave_t.*
