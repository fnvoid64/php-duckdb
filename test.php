<?php
$db = new Fnvoid\DuckDB\DuckDB(null);

$db->registerFunction('test_func', fn ($name): string => strtoupper($name));

$res = $db->query("CREATE TABLE IF NOT EXISTS test_table (id INTEGER, name VARCHAR, age INTEGER)");
$appender = $db->createAppender("test_table");

for ($i = 1; $i <= 10000; $i++) {
    $appender->appendRow([$i, "Name: {$i}", mt_rand(18,40)]);
}

$appender->flush();

$res = $db->query("SELECT *, test_func(name) as name_test FROM test_table");

foreach ($res->iterate() as $row) {
    print_r($row);
}