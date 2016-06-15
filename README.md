RDB-Parser
================
RDB-Parser enables users to parse a RDB file(Redis Snapshot file) into XML-formatted file.

#### 1. What about this tool to do?

> use lua to parse rdb file to user's format data, like aof file, json etc.


#### 2. How to use?
```shell
$ cd rdbtools/src
$ make
$ ./rdbtools -f ../tests/dump2.8.rdb -s ../scripts/example.lua
```

#### 3. Options

```shell
USAGE: ./rdbtools [-f file] -V -h
    -V --version
    -h --help show usage
    -f --file specify which rdb file would be parsed.
    -s --file specify which lua script, default is ../scripts/example.lua
```

If you want to handle key-value in rdb file, you can use `-s your_script.lua`, and lua function `handle` will be callbacked.

Example can be found in `scripts/example.lua`, and it just print the key-value.


##### Json format example

`cat scripts/json_exapmle.lua`

```lua
local cjson = require "cjson"

function handle(item)
     print(cjson.encode(item))
end     
```

#### 4. Contact me?

>```Mail```: [weekend27@163.com](mailto:weekend27@163.com)

>```Blog```: [www.hulkdev.com](http://www.hulkdev.com)

any bugs? send mail, and I will appreciate your help.
