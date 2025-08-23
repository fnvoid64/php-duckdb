# PHP DuckDB Extension

In‑process analytics for PHP powered by [DuckDB](https://duckdb.org).\
This extension delivers fast on‑disk and in‑memory SQL, **prepared statements**, streaming fetch, bulk appends, and — most importantly — **first‑class PHP User‑Defined Functions (UDFs)** so you can call your own PHP code directly from SQL.

> **Highlights**
> - **PHP UDFs**: Register PHP callables and use them inside SQL.  
> - **Prepared statements** (positional parameters).  
> - **Streaming result** iteration over millions of rows.  
> - **Bulk inserts** via Appender (with `flush()` for immediate visibility).  
> - **Flexible fetching** (objects by default, associative arrays optional).  
> - **Clear exceptions** via `Fnvoid\DuckDB\Exception`.

---

## Table of Contents
- [Installation](#installation)
- [Creating a Connection](#creating-a-connection)
- [Query Execution](#query-execution)
- [Prepared Statements](#prepared-statements)
- [Fetching Data](#fetching-data)
- [Appender](#appender)
- [User Defined Functions (UDFs)](#user-defined-functions-udfs)
- [Errors & Exceptions](#errors--exceptions)
- [License](#license)

---

## Installation

I am working on a simple installation script.

## Creating a Connection

### In‑memory
```php
try {
    $db = new Fnvoid\DuckDB\DuckDB(); // In‑memory database
} catch (Fnvoid\DuckDB\Exception $e) {
    echo $e->getMessage();
}
```

### File‑backed
```php
try {
    $db = new Fnvoid\DuckDB\DuckDB('/path/to/db');
} catch (Fnvoid\DuckDB\Exception $e) {
    echo $e->getMessage();
}
```

### With Configuration
You can pass DuckDB configuration either as the second argument or via a named `array $config`.

```php
try {
    $db = new Fnvoid\DuckDB\DuckDB('/path/to/db', [
        'threads' => 8,
    ]);

    // or using a named argument:
    $db = new Fnvoid\DuckDB\DuckDB(config: [
        'threads' => 8,
    ]);
} catch (Fnvoid\DuckDB\Exception $e) {
    echo $e->getMessage();
}
```

---

## Query Execution
`Fnvoid\DuckDB\DuckDB::query()` returns a `Fnvoid\DuckDB\Result` object that you can fetch from.

```php
try {
    $db = new Fnvoid\DuckDB\DuckDB(); // In‑memory
    $res = $db->query("CREATE TABLE test_table (id INT, name VARCHAR, age INT)");
} catch (Fnvoid\DuckDB\Exception $e) {
    echo $e->getMessage();
}
```

---

## Prepared Statements
Prepared statements currently support **positional parameters only**.  
`Fnvoid\DuckDB\DuckDB::prepare()` returns a `Fnvoid\DuckDB\Statement`.

```php
try {
    $db = new Fnvoid\DuckDB\DuckDB();
    $stmt = $db->prepare("SELECT * FROM test_table WHERE id = ?");
    $res  = $stmt->execute([1]);
    $row  = $res->fetchOne(); // Fetch a single row
} catch (Fnvoid\DuckDB\Exception $e) {
    echo $e->getMessage();
}
```

---

## Fetching Data

### 1) Stream with `iterate()` (recommended)
Efficient, lazy iteration suitable for very large result sets.

```php
try {
    $db = new Fnvoid\DuckDB\DuckDB();
    $stmt = $db->prepare("SELECT * FROM test_table WHERE status = ?");
    $res  = $stmt->execute([1]);

    foreach ($res->iterate() as $row) {
        // Process row
    }
} catch (Fnvoid\DuckDB\Exception $e) {
    echo $e->getMessage();
}
```

### 2) Get everything with `fetchAll()`
Best for small result sets.

```php
$res = $stmt->execute([1])->fetchAll();
```

### 3) Get a single row with `fetchOne()`
```php
$res = $stmt->execute([1])->fetchOne();
```

### Fetch modes
All fetchers (`iterate()`, `fetchAll()`, `fetchOne()`) accept an optional boolean `$assoc`:
- `false` (default) → **objects**
- `true` → **associative arrays**

---

## Appender
Use the **Appender** for bulk inserts. Values are positional and must match the table schema order.  
Data is **not immediately visible** to queries until you call `flush()`.

```php
try {
    $db = new Fnvoid\DuckDB\DuckDB();
    $appender = $db->createAppender('table_name');

    foreach ($someLocalData as $row) {
        $appender->appendRow([
            $row->id,
            $row->name,
            $row->age,
        ]);
    }

    // Make appended data visible to queries
    $appender->flush();

    $res = $db->query("SELECT * FROM table_name");
} catch (Fnvoid\DuckDB\Exception $e) {
    echo $e->getMessage();
}
```

---

## User Defined Functions (UDFs)
Run your own **PHP code inside DuckDB SQL**. This is a major feature for many workloads (custom transforms, business rules, domain logic, etc.).

> **Requirements & Notes**
> - UDFs require **single‑threaded** execution: set `threads = 1` in config.  
> - Supported return types: `string`, `int`, `float`, `double`, `null`, `bool`.  
> - Register PHP functions by name; then call them from SQL.

```php
try {
    $db = new Fnvoid\DuckDB\DuckDB(config: [
        'threads' => 1, // Required for UDFs
    ]);

    // Register a PHP function
    $db->registerFunction('php_upper', fn (string $s): string => strtoupper($s));

    $stmt = $db->prepare("
        SELECT id, name, php_upper(name) AS name_upper
        FROM test_table
        WHERE status = ?
    ");

    $row = $stmt->execute([1])->fetchOne();
} catch (Fnvoid\DuckDB\Exception $e) {
    echo $e->getMessage();
}
```

---

## Errors & Exceptions
All failures throw `Fnvoid\DuckDB\Exception`. Wrap calls in `try/catch` to handle errors gracefully.

```php
try {
    $db = new Fnvoid\DuckDB\DuckDB('/invalid/path');
} catch (Fnvoid\DuckDB\Exception $e) {
    // Log / rethrow / surface message
    error_log($e->getMessage());
}
```

---

