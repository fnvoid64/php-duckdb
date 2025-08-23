--TEST--
Test Query & Lazy Result Iteration
--FILE--
<?php
$db = new Fnvoid\DuckDB\DuckDB();
$db->query("CREATE TABLE test_tbl (id INT, name VARCHAR, age INT)");

for ($i = 1; $i <= 10; $i++) {
    $db->query("INSERT INTO test_tbl (id, name, age) VALUES ({$i}, 'Name-{$i}', 20)");
}

$result = $db->query("SELECT * FROM test_tbl");
foreach ($result->iterate() as $row) {
    print_r($row);
}
?>
--EXPECT--
stdClass Object
(
    [id] => 1
    [name] => Name-1
    [age] => 20
)
stdClass Object
(
    [id] => 2
    [name] => Name-2
    [age] => 20
)
stdClass Object
(
    [id] => 3
    [name] => Name-3
    [age] => 20
)
stdClass Object
(
    [id] => 4
    [name] => Name-4
    [age] => 20
)
stdClass Object
(
    [id] => 5
    [name] => Name-5
    [age] => 20
)
stdClass Object
(
    [id] => 6
    [name] => Name-6
    [age] => 20
)
stdClass Object
(
    [id] => 7
    [name] => Name-7
    [age] => 20
)
stdClass Object
(
    [id] => 8
    [name] => Name-8
    [age] => 20
)
stdClass Object
(
    [id] => 9
    [name] => Name-9
    [age] => 20
)
stdClass Object
(
    [id] => 10
    [name] => Name-10
    [age] => 20
)