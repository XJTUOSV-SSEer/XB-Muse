// SPDX-License-Identifier: MIT

pragma solidity >=0.4.0 <0.9.0;

contract Storage {

    mapping(string => uint256[]) public map;

    /**
     * @dev Store value in variable
     * @param num value to store
     */
    function set(string memory w,uint256[] memory num) public {
        map[w] = num;
    }

    /**
     * @dev Return value 
     * @return value of 'number'
     */
    function get(string memory w) public view returns (uint256[] memory){
        return map[w];
    }
}