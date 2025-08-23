--TEST--
Test Query
--FILE--
<?php
$db = new Fnvoid\DuckDB\DuckDB();
$result = $db->query("SELECT NOW() as time_now");
var_dump($result->fetchOne());
?>
--EXPECTF--
object(stdClass)#%d (1) {
  ["time_now"]=>
  string(%d) "%s"
}