--TEST--
Test Prepared Statements
--FILE--
<?php

$db = new Fnvoid\DuckDB\DuckDB();
$stmt = $db->prepare("SELECT ? as current_time");
var_dump(($stmt->execute([date('c')])->fetchOne()));
?>
--EXPECTF--
object(stdClass)#%d (1) {
  ["current_time"]=>
  string(25) "%s"
}