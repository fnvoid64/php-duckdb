--TEST--
Check if class initiates
--FILE--
<?php
$duckdb = new DuckDB\DuckDB('test.db');
$duckdb->registerFunction('php_test', fn (string $x, string $y): string => strtoupper($x) . " " . $y);
$result = $duckdb->query("SELECT php_test('hello', 'world') as test1");

foreach ($result->iterate() as $row) {
    print_r($row);
}

?>
--EXPECTF--
stdClass Object
(
    [%s] => 1
)