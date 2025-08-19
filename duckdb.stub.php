<?php

/**
 * @generate-class-entries
 * @undocumentable
 */

namespace DuckDB {
    final class DuckDB 
    {
        public function __construct(?string $db_file = null)
        {
            
        }

        public function query(string $sql): ?Result
        {

        }

        public function prepare(string $sql): ?Statement
        {
            
        }

        public function registerFunction(string $name, callable $func): bool
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
        public function execute(array $values): ?Result
        {

        }
    }

    class Exception extends \Exception
    {

    }
}
