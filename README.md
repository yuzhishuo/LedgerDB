<!--
 * @Author: Leo
 * @Date: 2022-01-19 18:04:26
 * @LastEditTime: 2022-02-08 22:56:00
 * @LastEditors: Leo
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE

 * @FilePath: /example-authority-cpp/README.md
-->

# LedgerDB


# Feature

## User


## Leadger

|opeartor | method | finished |
| :-----| :---- | :----: |
| Create | Create(ledger_uri, enum, op_metadata) | ✔️ |
| Append | GrantRole(ledger_uri, member_id, role) | ❌ |


## install

```shell

mkdir build && cd build

cmake ..

make

```

Yang, Xinying, et al. "LedgerDB: a centralized ledger database for universal audit and verification." Proceedings of the VLDB Endowment 13.12 (2020): 3138-3151.