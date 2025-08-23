--TEST--
Test User Defined Functions
--FILE--
<?php
$db = new Fnvoid\DuckDB\DuckDB(config: [
    'threads' => 1
]);

$db->query("CREATE TABLE test_tbl (id INT, name VARCHAR, age INT)");
$appender = $db->createAppender('test_tbl');

for ($i = 1; $i <= 10; $i++) {
    $appender->appendRow([
        $i,
        "Name-{$i}",
        20
    ]);
}

$appender->flush();

$db->registerFunction('php_name', fn (string $name): string => strtoupper($name));

$result = $db->query("SELECT *, php_name(name) as from_php FROM test_tbl");
var_dump($result->fetchAll());
?>
--EXPECT--
array(10) {
  [0]=>
  object(stdClass)#5 (4) {
    ["id"]=>
    int(1)
    ["name"]=>
    string(6) "Name-1"
    ["age"]=>
    int(20)
    ["from_php"]=>
    string(6) "NAME-1"
  }
  [1]=>
  object(stdClass)#6 (4) {
    ["id"]=>
    int(2)
    ["name"]=>
    string(6) "Name-2"
    ["age"]=>
    int(20)
    ["from_php"]=>
    string(6) "NAME-2"
  }
  [2]=>
  object(stdClass)#7 (4) {
    ["id"]=>
    int(3)
    ["name"]=>
    string(6) "Name-3"
    ["age"]=>
    int(20)
    ["from_php"]=>
    string(6) "NAME-3"
  }
  [3]=>
  object(stdClass)#8 (4) {
    ["id"]=>
    int(4)
    ["name"]=>
    string(6) "Name-4"
    ["age"]=>
    int(20)
    ["from_php"]=>
    string(6) "NAME-4"
  }
  [4]=>
  object(stdClass)#9 (4) {
    ["id"]=>
    int(5)
    ["name"]=>
    string(6) "Name-5"
    ["age"]=>
    int(20)
    ["from_php"]=>
    string(6) "NAME-5"
  }
  [5]=>
  object(stdClass)#10 (4) {
    ["id"]=>
    int(6)
    ["name"]=>
    string(6) "Name-6"
    ["age"]=>
    int(20)
    ["from_php"]=>
    string(6) "NAME-6"
  }
  [6]=>
  object(stdClass)#11 (4) {
    ["id"]=>
    int(7)
    ["name"]=>
    string(6) "Name-7"
    ["age"]=>
    int(20)
    ["from_php"]=>
    string(6) "NAME-7"
  }
  [7]=>
  object(stdClass)#12 (4) {
    ["id"]=>
    int(8)
    ["name"]=>
    string(6) "Name-8"
    ["age"]=>
    int(20)
    ["from_php"]=>
    string(6) "NAME-8"
  }
  [8]=>
  object(stdClass)#13 (4) {
    ["id"]=>
    int(9)
    ["name"]=>
    string(6) "Name-9"
    ["age"]=>
    int(20)
    ["from_php"]=>
    string(6) "NAME-9"
  }
  [9]=>
  object(stdClass)#14 (4) {
    ["id"]=>
    int(10)
    ["name"]=>
    string(7) "Name-10"
    ["age"]=>
    int(20)
    ["from_php"]=>
    string(7) "NAME-10"
  }
}