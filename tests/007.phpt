--TEST--
Test Fetch as Associative Array
--FILE--
<?php
$db = new Fnvoid\DuckDB\DuckDB();
$db->query("CREATE TABLE test_tbl (id INT, name VARCHAR, age INT)");

for ($i = 1; $i <= 10; $i++) {
    $db->query("INSERT INTO test_tbl (id, name, age) VALUES ({$i}, 'Name-{$i}', 20)");
}

$result = $db->query("SELECT * FROM test_tbl");
var_dump($result->fetchOne(true));

$result = $db->query("SELECT * FROM test_tbl");
var_dump($result->fetchAll(true));

$result = $db->query("SELECT * FROM test_tbl");
foreach ($result->iterate(true) as $row) {
    var_dump($row);
}

?>
--EXPECT--
array(3) {
  ["id"]=>
  int(1)
  ["name"]=>
  string(6) "Name-1"
  ["age"]=>
  int(20)
}
array(10) {
  [0]=>
  array(3) {
    ["id"]=>
    int(1)
    ["name"]=>
    string(6) "Name-1"
    ["age"]=>
    int(20)
  }
  [1]=>
  array(3) {
    ["id"]=>
    int(2)
    ["name"]=>
    string(6) "Name-2"
    ["age"]=>
    int(20)
  }
  [2]=>
  array(3) {
    ["id"]=>
    int(3)
    ["name"]=>
    string(6) "Name-3"
    ["age"]=>
    int(20)
  }
  [3]=>
  array(3) {
    ["id"]=>
    int(4)
    ["name"]=>
    string(6) "Name-4"
    ["age"]=>
    int(20)
  }
  [4]=>
  array(3) {
    ["id"]=>
    int(5)
    ["name"]=>
    string(6) "Name-5"
    ["age"]=>
    int(20)
  }
  [5]=>
  array(3) {
    ["id"]=>
    int(6)
    ["name"]=>
    string(6) "Name-6"
    ["age"]=>
    int(20)
  }
  [6]=>
  array(3) {
    ["id"]=>
    int(7)
    ["name"]=>
    string(6) "Name-7"
    ["age"]=>
    int(20)
  }
  [7]=>
  array(3) {
    ["id"]=>
    int(8)
    ["name"]=>
    string(6) "Name-8"
    ["age"]=>
    int(20)
  }
  [8]=>
  array(3) {
    ["id"]=>
    int(9)
    ["name"]=>
    string(6) "Name-9"
    ["age"]=>
    int(20)
  }
  [9]=>
  array(3) {
    ["id"]=>
    int(10)
    ["name"]=>
    string(7) "Name-10"
    ["age"]=>
    int(20)
  }
}
array(3) {
  ["id"]=>
  int(1)
  ["name"]=>
  string(6) "Name-1"
  ["age"]=>
  int(20)
}
array(3) {
  ["id"]=>
  int(2)
  ["name"]=>
  string(6) "Name-2"
  ["age"]=>
  int(20)
}
array(3) {
  ["id"]=>
  int(3)
  ["name"]=>
  string(6) "Name-3"
  ["age"]=>
  int(20)
}
array(3) {
  ["id"]=>
  int(4)
  ["name"]=>
  string(6) "Name-4"
  ["age"]=>
  int(20)
}
array(3) {
  ["id"]=>
  int(5)
  ["name"]=>
  string(6) "Name-5"
  ["age"]=>
  int(20)
}
array(3) {
  ["id"]=>
  int(6)
  ["name"]=>
  string(6) "Name-6"
  ["age"]=>
  int(20)
}
array(3) {
  ["id"]=>
  int(7)
  ["name"]=>
  string(6) "Name-7"
  ["age"]=>
  int(20)
}
array(3) {
  ["id"]=>
  int(8)
  ["name"]=>
  string(6) "Name-8"
  ["age"]=>
  int(20)
}
array(3) {
  ["id"]=>
  int(9)
  ["name"]=>
  string(6) "Name-9"
  ["age"]=>
  int(20)
}
array(3) {
  ["id"]=>
  int(10)
  ["name"]=>
  string(7) "Name-10"
  ["age"]=>
  int(20)
}