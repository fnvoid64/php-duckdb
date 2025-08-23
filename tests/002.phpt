--TEST--
Test DuckDB Connection & Library
--FILE--
<?php
$db = new Fnvoid\DuckDB\DuckDB(); // Test mem
$db = new Fnvoid\DuckDB\DuckDB('test.db'); // Test file
var_dump($db->duckDBVersion());

?>
--EXPECTF--
string(%d) "%s"