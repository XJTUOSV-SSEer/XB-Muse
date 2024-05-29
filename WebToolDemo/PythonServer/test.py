from flask import Flask, request, jsonify
import hashlib
from io import BytesIO

from web3 import HTTPProvider, Web3
import json
from web3.middleware import geth_poa_middleware

contract_instance = None

def DeployContract(contract_path='../build/contracts/Storage.json'):

    global contract_instance

    w3 = Web3(Web3.HTTPProvider("HTTP://127.0.0.1:8545"))
 
    with open(contract_path, 'r', encoding='utf-8') as contract_json_file:

        if contract_json_file is None:
            raise Exception("complied contract not found at " + contract_path)

        contract_json = json.load(contract_json_file)
        contract = w3.eth.contract(abi=contract_json['abi'], bytecode=contract_json['bytecode'])
        account = w3.eth.accounts[0]
        tx_hash = contract.constructor().transact({'from': w3.eth.accounts[0]})
        tx_receipt = w3.eth.wait_for_transaction_receipt(tx_hash)
        contractAddress = tx_receipt.contractAddress
        contract_instance = w3.eth.contract(address=contractAddress, abi=contract_json['abi'])

        return contract_instance, account, w3

contract_instance, from_account, w3 = DeployContract()

# # 设置一个新的数组
# user_id = 12345
# word = "hello"
# numbers = [115792089237316195423570985008687907853269984665640564039457584007913129639935, 2, 3, 4, 5]
# tx_hash = contract_instance.functions.set(user_id, word, numbers).transact({
#             "from": from_account,
#             "gas": 300000,
#             "gasPrice": 0,
# })
# w3.eth.waitForTransactionReceipt(tx_hash)
# print(f"Set {numbers} for user {user_id} and word '{word}'")

# # 获取数组
# retrieved_numbers = contract_instance.functions.get(user_id, word).call()
# print(f"Retrieved numbers for user {user_id} and word '{word}': {retrieved_numbers}")

app = Flask(__name__)

@app.route('/getD', methods=['GET'])
def getD():
    # 获取请求中的整数和字符串
    data = request.get_json()
    userId = (int)(data.get('userId'))
    w = data.get('w')


    # print("userId:"+str(userId))
    # print("user_id:" + userId + "\ntype:" + type(userId)+"\n")
    # print("w:" + w + "\ntypre: "+type(w))

    retrieved_numbers = contract_instance.functions.get(userId, w).call()
    # print("retrieved_numbers : " + str(retrieved_numbers))
    byte_stream = BytesIO()
    for num in retrieved_numbers:
        byte_stream.write(num.to_bytes(256, byteorder='big', signed=True))
    bytes_arr = byte_stream.getvalue()
    print("bytes : " + str(bytes_arr))
    # 将位数组返回为JSON格式
    return jsonify({'bits_array': (str)(bytes_arr)})

@app.route('/setD', methods=['POST'])
def setD():

    data = request.get_json()
    # print(data)
    userId = (int)(data.get('userId'))
    w = data.get('w')
    D_bit_array = data.get('bit_array')

    D_int_array = []
    print("userId:"+str(userId))
    print("w:" + w)
    print("set : " + str(type(D_bit_array))) 
    
    # 将 bit 数组转换为整数数组
    for i in range(0, len(D_bit_array), 256):
        byte_value = int(D_bit_array[i:i + 256], 2)
        D_int_array.append(byte_value)
    print(D_int_array)
    tx_hash = contract_instance.functions.set(userId, w, D_int_array).transact({
            "from": from_account,
            "gas": 300000,
            "gasPrice": 0,
    })
    w3.eth.waitForTransactionReceipt(tx_hash)

    return "OK"

if __name__ == '__main__':
    app.run(debug=True,host='192.168.1.109',port=12346)

