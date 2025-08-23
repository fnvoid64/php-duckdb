<?php

/**
 * @generate-class-entries
 * @undocumentable
 */

namespace Fnvoid\DuckDB {
    final class DuckDB 
    {
        public function __construct(?string $db_file = null, array $config = [])
        {
            
        }

        public function query(string $sql): Result
        {

        }

        public function prepare(string $sql): Statement
        {
            
        }

        public function registerFunction(string $name, callable $func): bool
        {
            
        }

        public function createAppender(string $table): Appender
        {

        }

        public function duckDBVersion(): string
        {

        }
    }

    final class Result
    {
        public function fetchAll(bool $assoc = false): array
        {

        }

        public function fetchOne(bool $assoc = false): object|array|null
        {

        }

        public function iterate(bool $assoc = false): ResultIterator
        {

        }
    }

    final class ResultIterator implements \Iterator
    {
        public function rewind(): void
        {
            
        }

        public function current(): array|object|null
        {
            
        }

        public function key(): int
        {
            
        }

        public function next(): void
        {
            
        }

        public function valid(): bool
        {
            
        }
    }

    final class Statement
    {
        public function execute(array $values): Result
        {

        }
    }

    final class Appender
    {
        public function appendRow(array $row): bool {

        }

        public function flush(): void
        {

        }
    }

    class Exception extends \Exception
    {

    }
}
