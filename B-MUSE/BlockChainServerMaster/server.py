from flask import Flask, request, jsonify
import hashlib
from io import BytesIO

from web3 import HTTPProvider, Web3
import json
from web3.middleware import geth_poa_middleware

contract_instance = None

def DeployContract(contract_path='/home/node4/yangxu/chensifan/testRPC/build/contracts/Storage.json'):

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

app = Flask(__name__)

@app.route('/getD', methods=['GET'])
def getD():
    # 获取请求中的整数和字符串
    data = request.get_json()
    # userId = (int)(data.get('userId'))
    w = data.get('w')

    retrieved_numbers = contract_instance.functions.get(w).call()

    bytes_str = ""
    for num in retrieved_numbers:
        bytes_str += "{0:b}".format(num).zfill(256)

    # 将位数组返回为JSON格式
    return jsonify({'bits_array': bytes_str})

@app.route('/setD', methods=['POST'])
def setD():
    print("setD")

    data = request.get_json()
    # print(data)
    # userId = (int)(data.get('userId'))
    w = data.get('w')
    D_bit_array = data.get('bit_array')

    #在D_bit_array右侧填充0，直到总长度为256的倍数
    # 计算当前字符串的长度
    current_length = len(D_bit_array)
    # 计算需要填充的长度
    padding_length = (256 - (current_length % 256)) % 256
    # 使用 ljust 填充字符串
    D_bit_array = D_bit_array.ljust(current_length + padding_length, '0')

    D_int_array = []
    
    # 将 bit 数组转换为整数数组
    for i in range(0, len(D_bit_array), 256):
        byte_value = int(D_bit_array[i:i + 256], 2)
        D_int_array.append(byte_value)
    tx_hash = contract_instance.functions.set( w, D_int_array).transact({
            "from": from_account,
            "gas": 3000000,
            "gasPrice": 0,
    })
    w3.eth.waitForTransactionReceipt(tx_hash)

    return "OK"

if __name__ == '__main__':
    app.run(debug=True,host='192.168.1.101',port=12348)

