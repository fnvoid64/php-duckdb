--TEST--
Test extension loaded
--EXTENSIONS--
duckdb
--FILE--
<?php
echo 'The extension "duckdb" is available';
?>
--EXPECT--
The extension "duckdb" is available
